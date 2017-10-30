// Original example by Daniel Stenberg for libcurl
// Copyright (C) 1998 - 2017, Daniel Stenberg, <daniel@haxx.se>, et al.

#include "sds/sds.h"
#include "sds/sdsalloc.h"
#include <curl/curl.h>

size_t write_callback(void* extra_response, size_t size, size_t extra_chars, sds* response){
  uint extra_size = size * extra_chars;
  *response = sdscatlen(*response, extra_response, extra_size);
  return extra_size;
}

int main(void){
  sds response = sdsempty();

  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "https://horizon.stellar.org/ledgers/14085369");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  puts(response);
  sdsfree(response);

  return 0;
}
