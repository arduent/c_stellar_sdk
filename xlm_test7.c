#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 4,
  };

  // xlm_orderbook_trades(&horizon, &response, &request_params);

  xlm_JSON* json_root = json_parse(response.raw);
  json_show(json_root);

  json_free(json_root);
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
