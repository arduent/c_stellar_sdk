// gcc test_xlm_0.c sds/sds.c cJSON/cJSON.c `curl-config --libs`

#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Also try `XLM_HORIZON_TESTNET`

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  struct xlm_Response response;
  xlm_response_init(&response);

  xlm_account(&horizon, &response, &request_params);  // Send the request to the Horizon server!
  m_sep();  xlm_show_request(&response);  // Show the request URL
  m_sep();  xlm_show_raw(&response);  // Show the response, as a raw string

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  xlm_JSON* json_balances = json_get(json_root, "balances");
  m_sep();  json_show(json_balances);

  m_sep();
  for(uint i=0; i<json_get_array_len(json_balances); ++i){
    xlm_JSON* json_balance_item = json_get_array_item(json_balances, i);
    xlm_JSON* json_balance = json_get(json_balance_item, "balance");
    xlm_JSON* json_asset_type = json_get(json_balance_item, "asset_type");
    printf("%s  %s\n", json_asset_type->valuestring, json_balance->valuestring);
  }

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
