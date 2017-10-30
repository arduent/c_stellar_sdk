# Examples

## High-level (synchronous) API

---------------------------------------------------------------------------------------------------
#### View account balances (all assets)

```c
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
```

#### Send a payment with an account

```c
#include "stellar_sdk.h"

int main(){
  // 0. Build the transaction

  // 1. Sign the transaction with the secret seed

  // 2. Submit the transaction to the network
}
```


---------------------------------------------------------------------------------------------------
#### View all ledgers

###### Show the first ledger in the (returned) records

```c
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
```

###### Show all ledgers in the (returned) records

```c
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
```

#### View a single ledger

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 14085369,
  };

  xlm_ledger(&horizon, &response, &request_params);

  xlm_JSON* json_root = json_parse(response.raw);

  struct xlm_Ledger ledger;
  xlm_ledger_from_json(&ledger, json_root);
  m_sep();  xlm_ledger_show(&ledger);

  json_free(json_root);
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View transactions for a ledger

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 14085369,
  };

  xlm_ledger_transactions(&horizon, &response, &request_params);

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View operations for a ledger

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 14085369,
  };

  xlm_ledger_operations(&horizon, &response, &request_params);

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View effects for a ledger

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 14085369,
  };

  xlm_ledger_effects(&horizon, &response, &request_params);

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View payments for a ledger

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 14085369,
  };

  xlm_ledger_payments(&horizon, &response, &request_params);

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```


---------------------------------------------------------------------------------------------------
#### View all transactions

```c
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

  xlm_transactions(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  xlm_JSON* json_embedded = json_get(json_root, "_embedded");
  xlm_JSON* json_records = json_get(json_embedded, "records");
  json_show(json_records);

  uint n_records = json_get_array_len(json_records);

  m_sep();
  for(uint i=0; i<n_records; ++i){
    xlm_JSON* json_record = json_get_array_item(json_records, i);
    xlm_JSON* json_hash = json_get(json_record, "hash");
    xlm_JSON* json_ledger = json_get(json_record, "ledger");
    xlm_JSON* json_created_at = json_get(json_record, "created_at");
    m_puts();
    printf("%24s  ", "hash");  json_show(json_hash);
    printf("%24s  ", "ledger");  json_show(json_ledger);
    printf("%24s  ", "created_at");  json_show(json_created_at);
  }

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View a single transaction

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .id = "70dbba321418c3e21f6c05216ed3dadbfc1413672cc2a366a81ea4fbd2308c16",
  };

  xlm_transaction(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View operations for a transaction

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .id = "70dbba321418c3e21f6c05216ed3dadbfc1413672cc2a366a81ea4fbd2308c16",
  };

  xlm_transaction_operations(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View effects for a transaction

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .id = "70dbba321418c3e21f6c05216ed3dadbfc1413672cc2a366a81ea4fbd2308c16",
  };

  xlm_transaction_effects(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View payments for a transaction

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .id = "70dbba321418c3e21f6c05216ed3dadbfc1413672cc2a366a81ea4fbd2308c16",
  };

  xlm_transaction_payments(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```


---------------------------------------------------------------------------------------------------
#### View all operations

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60544972855709697,
    .order = "desc",
    .limit = 4,
  };

  xlm_operations(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View a single operation

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .id = "60544972855709697",
  };

  xlm_operation(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```


#### View effects for an operation

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .id = "60544972855709697",
  };

  xlm_operation_effects(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```


---------------------------------------------------------------------------------------------------
#### View all effects

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60544972855709697,
    .order = "desc",
    .limit = 4,
  };

  xlm_effects(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

---------------------------------------------------------------------------------------------------
#### View all payments

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60544972855709697,
    .order = "desc",
    .limit = 4,
  };

  xlm_payments(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

---------------------------------------------------------------------------------------------------
#### View all trades

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .cursor = 60544972855709697,
    .order = "desc",
    .limit = 4,
  };

  xlm_orderbook_trades(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View trades for an orderbook

```c
#include "stellar_sdk.h"

int main(){
}
```

#### View trades for an asset

```c
#include "stellar_sdk.h"

int main(){
}
```

#### View trades for an issuer

```c
#include "stellar_sdk.h"

int main(){
}
```


---------------------------------------------------------------------------------------------------
#### View metrics

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params;

  xlm_metrics(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```


---------------------------------------------------------------------------------------------------
#### View transactions for an account

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account_transactions(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View operations for an account

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account_operations(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View effects for an account

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account_effects(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View payments for an account

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account_payments(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View offers for an account

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account_offers(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```

#### View trades for an account

```c
#include "stellar_sdk.h"

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Or `XLM_HORIZON_TESTNET`

  struct xlm_Response response;
  xlm_response_init(&response);

  struct xlm_Request_params request_params = {
    .account_id = "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W",
  };

  xlm_account_trades(&horizon, &response, &request_params);  // Send request!

  xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  json_show(json_root);

  json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
```


---------------------------------------------------------------------------------------------------
#### Debug a request

```c
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
```

---------------------------------------------------------------------------------------------------
#### Get XLM/CNY price data

#### Find the [**value-at-risk (VaR)**](https://en.wikipedia.org/wiki/Value_at_risk) for the XLM/CNY price

#### Forecast the volatility of the XLM/CNY price with a [GARCH(1,1)](https://en.wikipedia.org/wiki/Autoregressive_conditional_heteroskedasticity#NGARCH) model

#### Do a [linear regression](https://en.wikipedia.org/wiki/Linear_regression) on XLM/CNY price

#### Price a purported [European call option](https://en.wikipedia.org/wiki/Option_style) on XLM under the [Black-Scholes pricing model](https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model)

#### Compute the implied volatility of a purported [European call option](https://en.wikipedia.org/wiki/Option_style) on XLM under the [Black-Scholes pricing model](https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model)
