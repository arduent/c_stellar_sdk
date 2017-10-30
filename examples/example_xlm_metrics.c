#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  xlm_metrics(&horizon, &response, NULL);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
