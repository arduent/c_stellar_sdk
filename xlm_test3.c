#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>  // UNIX only!
#include <curl/multi.h>

static const char* URLS[] = {
  // "https://horizon.stellar.org/metrics",
  // "https://horizon.stellar.org/accounts/GAOXVBKHGKH3UNAK4EIG3XVAWMA4B7Y3LF42EMIKJINVMLTNXYGHUQWM",
  "https://horizon.stellar.org/ledgers",
  // "https://horizon.stellar.org/ledgers/14044862",
  "https://horizon.stellar.org/order_book/trades",
  "http://ticker.stellar.org",
};

#define MAX 4
#define N_BUFFERS sizeof(URLS) / sizeof(char*)

// ----------------------------------------------------------------------
struct Buffer{
  char* data;
  size_t size;
};

void buffer_init(struct Buffer* buffer){
  buffer->data = malloc(1);  /* will be grown as needed by the realloc above */ 
  buffer->size = 0;    /* no data at this point */ 
}

void buffer_show(struct Buffer* buffer){
  puts("\n----------------------------------------------------------------------");
  for(uint i=0; i < buffer->size; ++i)
    putchar(buffer->data[i]);
  puts("");
}

// ----------------------------------------------------------------------
size_t callback(char* data, size_t size, size_t nmemb, void* user_ptr){
  size_t nbytes = size * nmemb;
  struct Buffer* buffer = (struct Buffer*)user_ptr;
  buffer->data = realloc(buffer->data, buffer->size + nbytes + 1);
  memcpy(&(buffer->data[buffer->size]), data, nbytes);
  buffer->size += nbytes;
  buffer->data[buffer->size] = 0; // Null terminate?
  return size * nmemb;
}

static void requests_init(CURLM* curl_multi, struct Buffer* buffer, int idx){
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
  curl_easy_setopt(curl, CURLOPT_URL, URLS[idx]);
  curl_easy_setopt(curl, CURLOPT_PRIVATE, URLS[idx]);
  curl_multi_add_handle(curl_multi, curl);
}

// ----------------------------------------------------------------------
int main(void){
  int M, Q, U = -1;
  fd_set R, W, E;
  struct timeval timeval0;

  CURLM* cm = curl_multi_init();

  // We can optionally limit the total amount of connections this multi handle uses
  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, MAX);

  struct Buffer buffers[N_BUFFERS];
  for(uint i=0; i<N_BUFFERS; ++i)
    buffer_init(&buffers[i]);

  uint idx = 0;
  for(idx = 0; idx < N_BUFFERS; ++idx)
    requests_init(cm, &buffers[idx], idx);

  long L;
  while(U){
    curl_multi_perform(cm, &U);

    if(U){
      FD_ZERO(&R);
      FD_ZERO(&W);
      FD_ZERO(&E);

      if(curl_multi_fdset(cm, &R, &W, &E, &M)){
        printf("E: curl_multi_fdset\n");
        return EXIT_FAILURE;
      }

      if(curl_multi_timeout(cm, &L)){
        printf("E: curl_multi_timeout\n");
        return EXIT_FAILURE;
      }

      if(L == -1)  L = 100;

      if(M == -1)
        sleep((unsigned int)L / 1000);
      else{
        timeval0.tv_sec = L / 1000;
        timeval0.tv_usec = (L % 1000) * 1000;

        if(0 > select(M + 1, &R, &W, &E, &timeval0)){
          printf("E: select(%i,,,,%li): %i: %s\n", M + 1, L, errno, strerror(errno));
          return EXIT_FAILURE;
        }
      }
    }

    CURLMsg* msg;
    while((msg = curl_multi_info_read(cm, &Q))){
      if(msg->msg == CURLMSG_DONE){
        char* url;
        CURL* e = msg->easy_handle;
        curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
        printf("response  %d  %s  %s\n", msg->data.result, url, curl_easy_strerror(msg->data.result));
        curl_multi_remove_handle(cm, e);
        curl_easy_cleanup(e);
      }
      else  printf("E: CURLMsg (%d)\n", msg->msg);

      if(idx < N_BUFFERS){
        requests_init(cm, &buffers[idx], idx);
        idx++;
        U++;  // Just to prevent it from remaining at 0 if there are more URLS to get
      }

    }  // END inner while

  }  // END outer while

  for(uint i=0; i<N_BUFFERS; ++i)
    buffer_show(&buffers[i]);

  curl_multi_cleanup(cm);
  return EXIT_SUCCESS;
}
