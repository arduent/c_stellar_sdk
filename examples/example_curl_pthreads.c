// Original example by Daniel Stenberg for libcurl
// Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.

#include <stdio.h>
#include <pthread.h>
#include <curl/curl.h>

#define N_THREADS 4

const char* const URLS[N_THREADS]= {
  "https://horizon.stellar.org/accounts/GAOXVBKHGKH3UNAK4EIG3XVAWMA4B7Y3LF42EMIKJINVMLTNXYGHUQWM",
  "https://horizon.stellar.org/ledgers/14044862",
};

void* request_get_sync(void* url){
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_perform(curl); /* ignores error */ 
  curl_easy_cleanup(curl);
  puts("");
}


int main(){
  pthread_t tid[N_THREADS];
  int status;

  for(uint i = 0; i<N_THREADS; ++i){
    status = pthread_create(&tid[i], NULL, request_get_sync, (void*)URLS[i]);
    printf("thread %d  status %d  url %s\n", i, status, URLS[i]);
  }

  // Wait for all threads to terminate
  for(uint i = 0; i<N_THREADS; i++){
    status = pthread_join(tid[i], NULL);
    printf("thread %d terminated  status %d\n", i, status);
  }

  printf("sizeof %ld\n", sizeof(URLS));
  return 0;
}
