#include "stellar_sdk.h"

// ------------------------------------------------------------------------------------------------
int main(void){
  // ----------------------------------------------------------------------
  // // Create a Response object, to hold the response.
  // struct xlm_Response response;
  // xlm_response_init(&response);

  // xlm_requests_get(&response, "https://horizon.stellar.org");
  // xlm_requests_show(&response);

  // xlm_requests_get(&response, "https://horizon.stellar.org/metrics");
  // xlm_requests_show(&response);

  // xlm_response_free(&response);

  // ----------------------------------------------------------------------
  // Create a Horizon object, to hold server data
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);

  // Create a Request_params object, to specify request parameters.
  // You may also use the default constructor `XLM_REQUEST_PARAMS_DEFAULT`
  struct xlm_Request_params request_params = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 2,
  };

  // Create a Response object, to hold the response
  struct xlm_Response response;
  xlm_response_init(&response);

  xlm_ledgers(&horizon, &response, &request_params);
  xlm_show_request(&response);  // Show the request URL
  xlm_show_raw(&response);  // Show the response, as a raw string

  xlm_response_free(&response);
  xlm_horizon_free(&horizon);

  // ----------------------------------------------------------------------
  return 0;
}
