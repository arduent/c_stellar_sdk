/*
@description Debug a request by printing the request and the raw response to console
*/

#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 2,
  };

  // Assemble and send request!
  xlm_ledgers(&horizon, &response, &request_params);

  // Show the assembled request
  xlm_show_request(&response);

  // Show the response, as a raw string
  xlm_show_raw(&response);

  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
