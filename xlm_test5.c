#include "stellar_sdk.h"


// ------------------------------------------------------------------------------------------------
struct xlm_Horizon{
  sds root_url;
};

struct xlm_Request_params{
  int64 cursor;
  char order[5];
  int64 limit;
};

struct xlm_Request_params XLM_REQUEST_PARAMS_DEFAULT = {
  .cursor = 0,
  .order = "asc",
  .limit = 10,
};

void xlm_server_init(struct xlm_Horizon* horizon, const char* root){
  horizon->root_url = sdsempty();
  horizon->root_url = sdscpy(horizon->root_url, root);
}

void xlm_server_free(struct xlm_Horizon* horizon){
  sdsfree(horizon->root_url);
}

void xlm_ledgers(struct xlm_Horizon* horizon, struct xlm_Request_params* request_params, struct xlm_Response* response){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGERS, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ------------------------------------------------------------------------------------------------
int main(void){
  // ----------------------------------------------------------------------
  // Create a Horizon object, to hold server data
  struct xlm_Horizon horizon;
  xlm_server_init(&horizon, XLM_HORIZON_LIVE);

  // Create a Request_params object, to specify request parameters.
  // You may also use the default constructor `XLM_REQUEST_PARAMS_DEFAULT`
  struct xlm_Request_params request_params = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 2,
  };

  // Create a Response object, to hold the response and other data
  struct xlm_Response response;
  xlm_response_init(&response);

  // Send the request to the Horizon server
  xlm_ledgers(&horizon, &request_params, &response);

  // ----------------------------------------------------------------------
  // xlm_show_request(&response);  // Show the request URL
  // xlm_show_response_raw(&response);  // Show the response, as a raw string

  // Show the response, as a JSON object
  // xlm_JSON* json_root = cJSON_Parse(response.response_raw);  // Parse a raw JSON string!
  // char* json_rendered = cJSON_Print(json_root);
  // puts(json_rendered);

  // ----------------------------------------------------------------------
  xlm_JSON* json_root = json_parse(response.response_raw);  // Parse a raw JSON string!
  xlm_JSON* json_embedded = json_get(json_root, "_embedded");
  xlm_JSON* json_records = json_get(json_embedded, "records");
  // json_show(json_records);

  xlm_JSON* json_record0 = json_get_array_item(json_records, 0);
  // json_show(json_record0);

  struct xlm_Ledger ledger0;
  xlm_ledger_from_json(&ledger0, json_record0);
  xlm_ledger_show(&ledger0);

  // ----------------------------------------------------------------------
  // Clean up
  xlm_response_free(&response);
  xlm_server_free(&horizon);
  json_free(json_root);  // Deleting the root takes care of everything

  // ----------------------------------------------------------------------
  m_exit_success();
}
