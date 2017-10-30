#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 4,
  };

  xlm_ledgers(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  xlm_JSON* json_embedded = json_get(json_root, "_embedded");
  xlm_JSON* json_records = json_get(json_embedded, "records");
  json_show(json_records);

  uint n_records = json_get_array_len(json_records);

  m_sep();
  for(uint i=0; i<n_records; ++i){  // Grab all ledgers from the records!
    xlm_JSON* json_record = json_get_array_item(json_records, i);
    struct xlm_Ledger ledger;
    xlm_ledger_from_json(&ledger, json_record);
    puts("");  xlm_ledger_show(&ledger);  // Show the ledger object from its internal format
  }

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
