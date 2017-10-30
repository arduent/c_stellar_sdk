#include "stellar_sdk.h"

int main(){
  // Create a Horizon object, to hold server data
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  // Create a Response object, to hold the response and other data
  struct xlm_Response response;
  xlm_response_init(&response);

  // Create a Request_params object, to specify request parameters.
  // You may also use the default constructor `XLM_REQUEST_PARAMS_DEFAULT`
  struct xlm_Request_params request_params = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 2,
  };

  xlm_ledgers(&horizon, &response, &request_params);  // Send request!

  // ---------------------
  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  xlm_JSON* json_embedded = json_get(json_root, "_embedded");
  xlm_JSON* json_records = json_get(json_embedded, "records");
  json_show(json_records);

  // Grab the first ledger from the records!
  xlm_JSON* json_record0 = json_get_array_item(json_records, 0);
  json_show(json_record0);

  // From the ledger JSON, initialize a proper C ledger object
  struct xlm_Ledger ledger0;
  xlm_ledger_from_json(&ledger0, json_record0);
  m_sep();
  xlm_ledger_show(&ledger0);  // Show the ledger object from its internal format

  // ---------------------
  // Clean up
  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
