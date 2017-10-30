// Original example by Daniel Stenberg for libcurl
// Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>  // UNIX only!
#include <curl/multi.h>

static const char* URLS[] = {
  // "https://horizon.stellar.org/metrics",
  "https://horizon.stellar.org/accounts/GAOXVBKHGKH3UNAK4EIG3XVAWMA4B7Y3LF42EMIKJINVMLTNXYGHUQWM",
  "https://horizon.stellar.org/ledgers",
  // "https://horizon.stellar.org/ledgers/14044862",
  // "https://horizon.stellar.org/order_book/trades",
  // "http://ticker.stellar.org",
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
size_t write_callback(char* data, size_t size, size_t nmemb, void* user_ptr){
  puts(data);

  size_t nbytes = size * nmemb;
  struct Buffer* buffer = (struct Buffer*)user_ptr;
  buffer->data = realloc(buffer->data, buffer->size + nbytes + 1);
  memcpy(&(buffer->data[buffer->size]), data, nbytes);
  buffer->size += nbytes;
  buffer->data[buffer->size] = 0; // Null terminate?
  return nbytes;
}

static void requests_init(CURLM* curl_multi, struct Buffer* buffer, int idx){
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
  curl_easy_setopt(curl, CURLOPT_URL, URLS[idx]);
  curl_easy_setopt(curl, CURLOPT_PRIVATE, URLS[idx]);

  struct curl_slist* header_extra;
  header_extra = curl_slist_append(header_extra, "Accept: text/event-stream");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_extra);  // Set custom header!

  curl_multi_add_handle(curl_multi, curl);
}

// ----------------------------------------------------------------------
int main(void){
  struct timeval timeval0;

  CURLM* curl_multi = curl_multi_init();

  // We can optionally limit the total amount of connections this multi handle uses
  curl_multi_setopt(curl_multi, CURLMOPT_MAXCONNECTS, MAX);

  struct Buffer buffers[N_BUFFERS];
  for(uint i=0; i<N_BUFFERS; ++i)
    buffer_init(&buffers[i]);

  uint idx = 0;
  for(idx = 0; idx < N_BUFFERS; ++idx)
    requests_init(curl_multi, &buffers[idx], idx);

  int M, Q, index = -1;
  fd_set fd_set0, fd_set1, fd_set2;
  long L;
  while(index){
    // printf("index %d  %ld\n", index, clock());
    curl_multi_perform(curl_multi, &index);

    if(index){
      FD_ZERO(&fd_set0);
      FD_ZERO(&fd_set1);
      FD_ZERO(&fd_set2);

      if(curl_multi_fdset(curl_multi, &fd_set0, &fd_set1, &fd_set2, &M)){
        printf("FATAL  curl_multi_fdset\n");
        return EXIT_FAILURE;
      }

      if(curl_multi_timeout(curl_multi, &L)){
        printf("FATAL  curl_multi_timeout\n");
        return EXIT_FAILURE;
      }

      if(L == -1)  L = 100;

      if(M == -1){
        sleep(L / 1000);
      }
      else{
        timeval0.tv_sec = L / 1000;
        timeval0.tv_usec = (L % 1000) * 1000;
        // printf("%lu  %lu\n", timeval0.tv_sec, timeval0.tv_usec);

        if(0 > select(M + 1, &fd_set0, &fd_set1, &fd_set2, &timeval0)){
          printf("FATAL  select(%i,,,,%li): %i: %s\n", M + 1, L, errno, strerror(errno));
          return EXIT_FAILURE;
        }
      }
    }

    CURLMsg* msg;
    while((msg = curl_multi_info_read(curl_multi, &Q))){
      if(msg->msg == CURLMSG_DONE){
        char* url;
        CURL* e = msg->easy_handle;
        curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
        printf("response  %d  %s  %s\n", msg->data.result, url, curl_easy_strerror(msg->data.result));
        curl_multi_remove_handle(curl_multi, e);
        curl_easy_cleanup(e);
      }
      else  printf("FATAL  CURLMsg (%d)\n", msg->msg);

      if(idx < N_BUFFERS){
        requests_init(curl_multi, &buffers[idx], idx);
        idx++;
        index++;  // Just to prevent it from remaining at 0 if there are more URLS to get
      }

    }  // END inner while

  }  // END outer while

  // for(uint i=0; i<N_BUFFERS; ++i)
  //   buffer_show(&buffers[i]);

  curl_multi_cleanup(curl_multi);
  return EXIT_SUCCESS;
}
