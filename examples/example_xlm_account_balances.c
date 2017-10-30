/*
@description View account balances (all assets)
*/

#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  xlm_JSON* json_balances = json_get(json_root, "balances");
  json_show(json_balances);

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
