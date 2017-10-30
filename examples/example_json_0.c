#include "stellar_sdk.h"
#include "sds/sds.h"
#include "sds/sdsalloc.h"
#include "cJSON/cJSON.h"
#include <curl/curl.h>

int main(void){
  sds response = sdsempty();

  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "https://horizon.stellar.org/ledgers/14085369");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, m_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  xlm_JSON* json_root = json_parse(response);  // Parse a raw JSON string!
  json_show(json_root);  // Show full JSON!

  json_free(json_root);
  sdsfree(response);
  m_exit_success();
}
