#include "stellar_sdk.h"

#include <stdlib.h>
#include <curl/curl.h>
#include "sds/sds.h"
#include "sds/sdsalloc.h"


// ------------------------------------------------------------------------------------------------
int main(int argc, char** argv){
  uint8 TEMPLATE0[] = "Hello! %s %d";

  // ----------------------------------------------------------------------
  m_sep();
  uint8 OUTPUT_BUFFER[1<<10];
  sprintf(OUTPUT_BUFFER, TEMPLATE0, "lala", 123);
  puts(OUTPUT_BUFFER);

  // ----------------------------------------------------------------------
  m_sep();

  sds str0 = sdsnew("Hiya! ");
  printf("len %lu  ", sdslen(str0));
  puts(str0);
  sdsfree(str0);

  sds str1 = sdsnew("Hiya! ");
  str1 = sdscatprintf(str1, TEMPLATE0, "lala", 123);
  puts(str1);
  sdsfree(str1);

  // ----------------------------------------------------------------------
  m_sep();
  puts(XLM_HORIZON_LIVE);
  // puts(XLM_HORIZON_LEDGERS);

  // ----------------------------------------------------------------------
  m_sep();
  CURL* curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, XLM_HORIZON_LIVE);
  // curl_easy_setopt(curl, CURLOPT_URL, XLM_HORIZON_LEDGERS);
  curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  // ----------------------------------------------------------------------
  m_exit_success();
}
