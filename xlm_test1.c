#include "stellar_sdk.h"
#include "sds/sds.h"
#include "sds/sdsalloc.h"

struct xlm_Horizon{
  sds url;
};

struct xlm_Request_params{
  int64 cursor;
  uint8 order[5];
  int64 limit;
};

void xlm_server_init(struct xlm_Horizon* horizon, uint8* home){
  horizon->url = home;
}

void xlm_ledgers(struct xlm_Horizon* horizon, struct xlm_Ledgers_request* ledgers_request, struct m_Response* response){
  sds request = sdsempty();
  request = sdscatprintf(request, XLM_HORIZON_LEDGERS, ledgers_request->cursor, ledgers_request->order, ledgers_request->limit);
  sdscpy(response->request_url, request);
  // puts(response->request_url);
  m_requests_get(response);
}

// ------------------------------------------------------------------------------------------------
int main(void){
  // ----------------------------------------------------------------------
  // Create a Response object, to hold the response.
  // struct m_Response response;
  // m_requests_init(&response);

  // m_requests_get(&response, "https://horizon.stellar.org");
  // m_requests_show(&response);

  // m_requests_get(&response, "https://horizon.stellar.org/metrics");
  // m_requests_show(&response);

  // m_requests_free(&response);

  // ----------------------------------------------------------------------
  // Create a response object. This object will hold the response, too.
  struct m_Response response;
  m_requests_init(&response);

  // Create a Horizon object, to hold server data.
  struct xlm_Horizon horizon;
  xlm_server_init(&horizon, XLM_HORIZON_HOME);

  // xlm_Request_params request_params;
  struct xlm_Ledgers_request ledgers_request = {
    .cursor = 60496199207092224,
    .order = "desc",
    .limit = 2,
  };

  xlm_ledgers(&horizon, &ledgers_request, &response);
  m_requests_show(&response);

  // ----------------------------------------------------------------------
  m_requests_free(&response);
}
