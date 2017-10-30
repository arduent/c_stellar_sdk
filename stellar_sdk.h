// Standard library
#include <stdio.h>      // For printf & other I/O!
#include <stdlib.h>     // malloc, strings, rand, EXIT_SUCCESS, & other stuff!
#include <stdint.h>     // Bare minimum for uint32_t, int64_t et al.!

// String libraries
#include "sds/sds.h"
#include "sds/sdsalloc.h"

// JSON libraries
#include "cJSON/cJSON.h"

// Requests libraries
#include <curl/curl.h>


// ------------------------------------------------------------------------------------------------
// Custom type names!
typedef unsigned int uint;
typedef unsigned char uchar;
typedef long long unsigned llu;
typedef unsigned long long ull;

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;


// ------------------------------------------------------------------------------------------------
// Helper functions!

void m_sep(){
  puts("\n----------------------------------------------------------------------");
}

void m_puts(){
  puts(" ");
}

void m_exit_success(){
  puts("\n----------------------------------------------------------------------\nExit success!");
  exit(EXIT_SUCCESS);
}

void m_exit_failure(){
  puts("\n----------------------------------------------------------------------\nExit failure!");
  exit(EXIT_FAILURE);
}

void m_putint(int integer){
  printf("%d\n", integer);
}

// ------------------------------------------------------------------------------------------------
// Thin abstraction layer over the chosen string engine!

typedef sds xlm_str;

xlm_str str_init(xlm_str string){
  if(!string)   return sdsempty();
  else          return sdsnew(string);
}

void str_free(xlm_str string){
  sdsfree(string);
}


// ------------------------------------------------------------------------------------------------
// Thin abstraction layer over the chosen JSON engine!

typedef cJSON xlm_JSON;

void json_free(xlm_JSON* object){
  cJSON_Delete(object);
}

// @function_name. A null-terminated char array
xlm_JSON* json_error_check(xlm_JSON* object, const char* function_name){
  if(object)  return object;  // All kosher!
  printf("ERROR  JSON object returned NULL @ %s\n", function_name);
  return object;
}

// @raw. A null-terminated char array
xlm_JSON* json_parse(const char* raw){
  return json_error_check(cJSON_Parse(raw), "cJSON_Parse()");

}

// @key. A null-terminated char array
xlm_JSON* json_get(xlm_JSON* object, const char* key){
  return json_error_check(cJSON_GetObjectItemCaseSensitive(object, key), "cJSON_GetObjectItemCaseSensitive()");
}

// @key. A null-terminated char array
xlm_JSON* json_get_array_item(xlm_JSON* object, uint32 index){
  return json_error_check(cJSON_GetArrayItem(object, index), "cJSON_GetArrayItem()");
}

int32 json_get_array_len(xlm_JSON* array){
  return cJSON_GetArraySize(array);
}

void json_show(xlm_JSON* object){
  char* json_rendered = cJSON_Print(object);
  puts(json_rendered);
  // free(json_rendered);
}


// ------------------------------------------------------------------------------------------------
// Horizon endpoints!

#define XLM_HORIZON_LIVE    "https://horizon.stellar.org"
#define XLM_HORIZON_TESTNET "https://horizon-testnet.stellar.org"

#define XLM_HORIZON_LEDGERS             "/ledgers?cursor=%ld&order=%s&limit=%ld"
#define XLM_HORIZON_LEDGER              "/ledgers/%ld"
#define XLM_HORIZON_LEDGER_TRANSACTIONS XLM_HORIZON_LEDGER "/transactions"
#define XLM_HORIZON_LEDGER_OPERATIONS   XLM_HORIZON_LEDGER "/operations"
#define XLM_HORIZON_LEDGER_EFFECTS      XLM_HORIZON_LEDGER "/effects"
#define XLM_HORIZON_LEDGER_PAYMENTS     XLM_HORIZON_LEDGER "/payments"

#define XLM_HORIZON_TRANSACTIONS            "/transactions?cursor=%ld&order=%s&limit=%ld"
#define XLM_HORIZON_TRANSACTION             "/transactions/%s"
#define XLM_HORIZON_TRANSACTION_OPERATIONS  "/transactions/%s/operations"
#define XLM_HORIZON_TRANSACTION_EFFECTS     "/transactions/%s/effects"
#define XLM_HORIZON_TRANSACTION_PAYMENTS     "/transactions/%s/payments"

#define XLM_HORIZON_OPERATIONS        "/operations?cursor=%ld&order=%s&limit=%ld"
#define XLM_HORIZON_OPERATION         "/operations/%s"
#define XLM_HORIZON_OPERATION_EFFECTS "/operations/%s/effects"

#define XLM_HORIZON_EFFECTS   "/effects?cursor=%ld&order=%s&limit=%ld"

#define XLM_HORIZON_PAYMENTS  "/payments?cursor=%ld&order=%s&limit=%ld"

#define XLM_HORIZON_TRADES            "/order_book/trades?cursor=%ld&order=%s&limit=%ld"
#define XLM_HORIZON_ORDERBOOK_TRADES  "/order_book/trades/%s"

#define XLM_HORIZON_METRICS       "/metrics"

#define XLM_HORIZON_ACCOUNTS              "/accounts"
#define XLM_HORIZON_ACCOUNT               XLM_HORIZON_ACCOUNTS "/%s"
#define XLM_HORIZON_ACCOUNT_TRANSACTIONS  XLM_HORIZON_ACCOUNTS "/%s/transactions"
#define XLM_HORIZON_ACCOUNT_OPERATIONS    XLM_HORIZON_ACCOUNTS "/%s/operations"
#define XLM_HORIZON_ACCOUNT_EFFECTS       XLM_HORIZON_ACCOUNTS "/%s/effects"
#define XLM_HORIZON_ACCOUNT_PAYMENTS      XLM_HORIZON_ACCOUNTS "/%s/payments"
#define XLM_HORIZON_ACCOUNT_OFFERS        XLM_HORIZON_ACCOUNTS "/%s/offers"
#define XLM_HORIZON_ACCOUNT_TRADES        XLM_HORIZON_ACCOUNTS "/%s/trades"


// ------------------------------------------------------------------------------------------------
// Horizon API object definitions!

// ----------------------------------------------------------------------
struct xlm_Asset{
  xlm_str asset_type;  // Eg. "credit_alphanum4"
  xlm_str asset_code;  // Eg. "JPY"
  xlm_str asset_issuer;  // Eg. "GBVAOIACNSB7OVUXJYC5UE2D4YK2F7A24T7EE5YOMN4CE6GCHUTOUQXM".
};

// ----------------------------------------------------------------------
// @description An object of account flags.
// @source https://www.stellar.org/developers/horizon/reference/resources/account.html
struct xlm_Thresholds{
  int64 low_threshold; // Eg. 0
  int64 med_threshold; // Eg. 0
  int64 high_threshold; // Eg. 0
};

struct xlm_Flags{
  int64 auth_required;  // Eg. false
  int64 auth_revocable;  // Eg. false
};

struct xlm_Balance{
  xlm_str balance;  // Eg. "20.9998600"
  struct xlm_Asset asset;
  xlm_str limit;  // Eg. "922337203685.4775807"
};

// @source https://www.stellar.org/developers/horizon/reference/resources/account.html
struct xlm_Account{
  xlm_str id;  // Eg. "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W"
  xlm_str paging_token;
  xlm_str account_id;  // Eg. "GB7JKG66CJN3ACX5DX43FOZTTSOI7GZUP547I3BSXIJVUX3NRYUXHE6W"
  xlm_str sequence;  // Eg. "47265161609807619"
  int64 subentry_count;  // Eg. 99
  struct xlm_Thresholds thresholds;
  struct xlm_Flags flags;
  // balances;  // TODO. A dynamic array
  // signers;  // TODO.
  // data;  // TODO.
};

// @source https://www.stellar.org/developers/horizon/reference/resources/ledger.html
struct xlm_Ledger{
  xlm_str id; // Eg. "f3ec095326b7371ee002e145df06d964afb66d3f4e4570d66cc64487409997a5"
  xlm_str paging_token; // Eg. "60496199207092224"
  xlm_str hash; // Eg. "f3ec095326b7371ee002e145df06d964afb66d3f4e4570d66cc64487409997a5"
  xlm_str prev_hash; // Eg. "61808517dfd799b2732e918fc2c9e15f9cd169d46fa2b65292eb6ce034d1b619"
  int64 sequence; // Eg. 14085369
  int64 transaction_count; // Eg. 1
  int64 operation_count; // Eg. 1
  xlm_str closed_at; // Eg. "2017-10-19T03:59:05Z"
  xlm_str total_coins; // Eg. "103353513548.0858279"
  xlm_str fee_pool; // Eg. "1498714.2764072"
  int64 base_fee; // Eg. 100
  xlm_str base_reserve; // Eg. "10.0000000"
  int64 max_tx_set_size; // Eg. 50
};

struct xlm_Transaction{
  xlm_str id;
  xlm_str paging_token;
  xlm_str hash;
  int64 ledger;
  xlm_str created_at;
  xlm_str source_account;
  xlm_str source_account_sequence;
  int64 fee_paid;
  int64 operation_count;
  xlm_str envelope_xdr;
  xlm_str result_xdr;
  xlm_str result_meta_xdr;
  xlm_str fee_meta_xdr;
  xlm_str memo_type;
  // signatures;  // TODO. A dynamic array
};

// TODO
struct xlm_Operation{
  // id
  // paging_token
  // type
  // type_i
};

// TODO
// @source https://www.stellar.org/developers/horizon/reference/resources/effect.html
struct xlm_Effect{};

// ----------------------------------------------------------------------
struct xlm_Txsub_total{
  double rate_15m;
  double rate_1m;
  double rate_5m;
  double percent_75;
  double percent_95;
  double percent_99;
  double percent_99_9;
  double count;
  double max;
  double mean;
  double mean_rate;
  double median;
  double min;
  double stddev;
};

// TODO
struct xlm_Metrics{
  // "goroutines": {
  //   "value":  1772
  // },
  // "history.elder_ledger": {
  //   "value":  1
  // },
  // "history.latest_ledger":  {
  //   "value":  14114826
  // },
  // "history.open_connections": {
  //   "value":  4
  // },
  // "ingester.clear_ledger":  {
  //   "15m.rate": 0,
  //   "1m.rate":  0,
  //   "5m.rate":  0,
  //   "75%":  0,
  //   "95%":  0,
  //   "99%":  0,
  //   "99.9%":  0,
  //   "count":  0,
  //   "max":  0,
  //   "mean": 0,
  //   "mean.rate":  0,
  //   "median": 0,
  //   "min":  0,
  //   "stddev": 0
  // },
  // "ingester.ingest_ledger": {
  //   "15m.rate": 0.11812419376051035,
  //   "1m.rate":  0.1031234609370041,
  //   "5m.rate":  0.13794188318077175,
  //   "75%":  31216624.75,
  //   "95%":  84560980.849999949,
  //   "99%":  139409001.20000008,
  //   "99.9%":  461674420.631001,
  //   "count":  1170155,
  //   "max":  469703229,
  //   "mean": 23410114.964980546,
  //   "mean.rate":  0.11580706136556193,
  //   "median": 8083656,
  //   "min":  2580099,
  //   "stddev": 31656047.010356091
  // },
  // "logging.debug":  {
  //   "15m.rate": 0,
  //   "1m.rate":  0,
  //   "5m.rate":  0,
  //   "count":  0,
  //   "mean.rate":  0
  // },
  // "logging.error":  {
  //   "15m.rate": 0.0031681823005199707,
  //   "1m.rate":  0.015991127326053706,
  //   "5m.rate":  0.0037162654238973147,
  //   "count":  306611,
  //   "mean.rate":  0.030344457550965747
  // },
  // "logging.info": {
  //   "15m.rate": 72.80736018797873,
  //   "1m.rate":  81.640764872591447,
  //   "5m.rate":  74.94076569398905,
  //   "count":  481278708,
  //   "mean.rate":  47.6308424952946
  // },
  // "logging.panic":  {
  //   "15m.rate": 0,
  //   "1m.rate":  0,
  //   "5m.rate":  0,
  //   "count":  0,
  //   "mean.rate":  0
  // },
  // "logging.warning":  {
  //   "15m.rate": 4.44659081257122e-313,
  //   "1m.rate":  2.96439387504748e-314,
  //   "5m.rate":  1.48219693752374e-313,
  //   "count":  23,
  //   "mean.rate":  2.2762475047283152e-06
  // },
  // "requests.failed":  {
  //   "15m.rate": 7.9835239008266079,
  //   "1m.rate":  8.9169513874885027,
  //   "5m.rate":  8.285217093460183,
  //   "count":  27884369,
  //   "mean.rate":  2.7596400332936097
  // },
  // "requests.succeeded": {
  //   "15m.rate": 22.388710086792791,
  //   "1m.rate":  25.351983954425044,
  //   "5m.rate":  22.989628160512403,
  //   "count":  179806069,
  //   "mean.rate":  17.794916804351242
  // },
  // "requests.total": {
  //   "15m.rate": 36.379805206915,
  //   "1m.rate":  40.734613073616259,
  //   "5m.rate":  37.438559150085091,
  //   "75%":  8719379595.75,
  //   "95%":  59836397701,
  //   "99%":  60108464665.74,
  //   "99.9%":  104741018091.53702,
  //   "count":  240495292,
  //   "max":  104874822031,
  //   "mean": 11341362474.10214,
  //   "mean.rate":  23.801163857127197,
  //   "median": 20588796.5,
  //   "min":  1955203,
  //   "stddev": 21955377590.780598
  // },
  // "stellar_core.elder_ledger":  {
  //   "value":  1
  // },
  // "stellar_core.latest_ledger": {
  //   "value":  14114826
  // },
  // "stellar_core.open_connections":  {
  //   "value":  4
  // },
  // "txsub.buffered": {
  //   "value":  0
  // },
  // "txsub.failed": {
  //   "15m.rate": 0.0011674748031691219,
  //   "1m.rate":  9.5277724773780971e-09,
  //   "5m.rate":  0.0002093478827760968,
  //   "count":  47300,
  //   "mean.rate":  0.0046811524991456788
  // },
  // "txsub.open": {
  //   "value":  0
  // },
  // "txsub.succeeded":  {
  //   "15m.rate": 0.012961680822445859,
  //   "1m.rate":  0.0007403218376541721,
  //   "5m.rate":  0.0068250838900106364,
  //   "count":  351586,
  //   "mean.rate":  0.03479551125929272
  // },
  struct xlm_Txsub_total txsub_total;
};

// TODO
// @source https://www.stellar.org/developers/guides/attachment.html
struct xlm_Attachment{
  // nonce;
  // transaction.sender_info;
  // transaction.route;
  // transaction.note;
  // operations[i];
  // operations[i].sender_info;
  // operations[i].route;
  // operations[i].note;
};


// ------------------------------------------------------------------------------------------------
// Horizon API method definitions!

// @object. A JSON object that must conform to the definition of `struct xlm_Ledger`
void xlm_ledger_from_json(struct xlm_Ledger* ledger, xlm_JSON* object){
  xlm_JSON* json_id = json_get(object, "id");
  xlm_JSON* json_paging_token = json_get(object, "paging_token");
  xlm_JSON* json_hash = json_get(object, "hash");
  xlm_JSON* json_prev_hash = json_get(object, "prev_hash");
  xlm_JSON* json_sequence = json_get(object, "sequence");
  xlm_JSON* json_transaction_count = json_get(object, "transaction_count");
  xlm_JSON* json_operation_count = json_get(object, "operation_count");
  xlm_JSON* json_closed_at = json_get(object, "closed_at");
  xlm_JSON* json_total_coins = json_get(object, "total_coins");
  xlm_JSON* json_fee_pool = json_get(object, "fee_pool");
  xlm_JSON* json_base_fee = json_get(object, "base_fee");
  xlm_JSON* json_base_reserve = json_get(object, "base_reserve");
  xlm_JSON* json_max_tx_set_size = json_get(object, "max_tx_set_size");

  ledger->id = sdsnew(json_id->valuestring);
  ledger->paging_token = sdsnew(json_paging_token->valuestring);
  ledger->hash = sdsnew(json_hash->valuestring);
  ledger->prev_hash = sdsnew(json_prev_hash->valuestring);
  ledger->sequence = json_sequence->valueint;
  ledger->transaction_count = json_transaction_count->valueint;
  ledger->operation_count = json_operation_count->valueint;
  ledger->closed_at = sdsnew(json_closed_at->valuestring);
  ledger->total_coins = sdsnew(json_total_coins->valuestring);
  ledger->fee_pool = sdsnew(json_fee_pool->valuestring);
  ledger->base_fee = json_base_fee->valueint;
  ledger->base_reserve = sdsnew(json_base_reserve->valuestring);
  ledger->max_tx_set_size = json_max_tx_set_size->valueint;
}

void xlm_ledger_show(struct xlm_Ledger* ledger){
  printf("%24s  %s\n", "id", ledger->id);
  printf("%24s  %s\n", "paging_token", ledger->paging_token);
  printf("%24s  %s\n", "hash", ledger->hash);
  printf("%24s  %s\n", "prev_hash", ledger->prev_hash);
  printf("%24s  %ld\n", "sequence", ledger->sequence);
  printf("%24s  %ld\n", "transaction_count", ledger->transaction_count);
  printf("%24s  %ld\n", "operation_count", ledger->operation_count);
  printf("%24s  %s\n", "closed_at", ledger->closed_at);
  printf("%24s  %s\n", "total_coins", ledger->total_coins);
  printf("%24s  %s\n", "fee_pool", ledger->fee_pool);
  printf("%24s  %ld\n", "base_fee", ledger->base_fee);
  printf("%24s  %s\n", "base_reserve", ledger->base_reserve);
  printf("%24s  %ld\n", "max_tx_set_size", ledger->max_tx_set_size);
}


// ------------------------------------------------------------------------------------------------
struct xlm_Horizon{
  xlm_str root_url;
};

void xlm_horizon_init(struct xlm_Horizon* horizon, const char* root){
  horizon->root_url = sdsempty();
  horizon->root_url = sdscpy(horizon->root_url, root);
}

void xlm_horizon_free(struct xlm_Horizon* horizon){
  sdsfree(horizon->root_url);
}


// ------------------------------------------------------------------------------------------------
struct xlm_Response{
  CURL* curl;
  CURLcode curl_code;
  xlm_str raw;
  xlm_str request_url;
};

void xlm_response_init(struct xlm_Response* response){
  response->curl = curl_easy_init();
  response->raw = sdsempty();
  response->request_url = sdsempty();
}

void xlm_response_free(struct xlm_Response* response){
  sdsfree(response->raw);
  sdsfree(response->request_url);
  curl_easy_cleanup(response->curl);
}

// Print a request to console!
void xlm_show_request(struct xlm_Response* response){
  puts(response->request_url);
}

// Print a response to console, as a raw string!
void xlm_show_raw(struct xlm_Response* response){
  puts(response->raw);
}


size_t m_write_callback(void* extra_response, size_t size, size_t extra_chars, sds* response){
  uint extra_bytes = size * extra_chars;
  *response = sdscatlen(*response, extra_response, extra_bytes);
  return extra_bytes;
}

// @url must be a null-terminated string!
void m_requests_get(struct xlm_Response* response){
  curl_easy_setopt(response->curl, CURLOPT_URL, response->request_url);
  curl_easy_setopt(response->curl, CURLOPT_WRITEFUNCTION, m_write_callback);
  curl_easy_setopt(response->curl, CURLOPT_WRITEDATA, &(response->raw));
  response->curl_code = curl_easy_perform(response->curl);
}


// ------------------------------------------------------------------------------------------------
struct xlm_Request_params{
  // Generic parameters
  int64 cursor;
  char  order[8];
  int64 limit;

  char id[256];

  // Parameters for accounts
  char  account_id[64];

  // Parameters for orderbook trades
  char selling_asset_type[64];
  char selling_asset_code[64];
  char selling_asset_issuer[64];
  char buying_asset_type[64];
  char buying_asset_code[64];
  char buying_asset_issuer[64];
};

// Default constructor!
struct xlm_Request_params XLM_REQUEST_PARAMS_DEFAULT = {
  .cursor = 0,
  .order = "asc",
  .limit = 10,
};


// ------------------------------------------------------------------------------------------------
void xlm_account(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_account_transactions(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_account_operations(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_account_effects(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_account_payments(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_account_offers(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_account_trades(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_ledgers(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_ledger(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_ledger_transactions(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_ledger_operations(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_ledger_effects(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_ledger_payments(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_transactions(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_transaction(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_transaction_operations(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_transaction_effects(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_transaction_payments(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_operations(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_operation(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_operation_effects(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_effects(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_payments(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_orderbook_trades(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_orderbook_trades_asset(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);
void xlm_orderbook_trades_issuer(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

void xlm_metrics(struct xlm_Horizon*, struct xlm_Response*, struct xlm_Request_params*);

// ---------------------
/*
Get account general data.

@horizon
@response
@request_params

@return void
*/
void xlm_account(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_account_transactions(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT_TRANSACTIONS, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_account_operations(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT_OPERATIONS, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_account_effects(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT_EFFECTS, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_account_payments(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT_PAYMENTS, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_account_offers(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT_OFFERS, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_account_trades(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_ACCOUNT_TRADES, request_params->account_id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ---------------------
void xlm_ledgers(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGERS, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_ledger(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGER, request_params->cursor);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_ledger_transactions(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGER_TRANSACTIONS, request_params->cursor);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_ledger_operations(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGER_OPERATIONS, request_params->cursor);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_ledger_effects(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGER_EFFECTS, request_params->cursor);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_ledger_payments(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_LEDGER_PAYMENTS, request_params->cursor);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ---------------------
void xlm_transactions(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_TRANSACTIONS, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_transaction(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_TRANSACTION, request_params->id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_transaction_operations(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_TRANSACTION_OPERATIONS, request_params->id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_transaction_effects(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_TRANSACTION_EFFECTS, request_params->id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_transaction_payments(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_TRANSACTION_PAYMENTS, request_params->id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ---------------------
void xlm_operations(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_OPERATIONS, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_operation(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_OPERATION, request_params->id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_operation_effects(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_OPERATION_EFFECTS, request_params->id);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ---------------------
void xlm_effects(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_EFFECTS, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ---------------------
void xlm_payments(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_PAYMENTS, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

// ---------------------
void xlm_orderbook_trades(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscatprintf(request, XLM_HORIZON_TRADES, request_params->cursor, request_params->order, request_params->limit);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}

void xlm_orderbook_trades_asset(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){}
void xlm_orderbook_trades_issuer(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){}

// ---------------------
void xlm_metrics(struct xlm_Horizon* horizon, struct xlm_Response* response, struct xlm_Request_params* request_params){
  sds request = sdsnew(horizon->root_url);
  request = sdscat(request, XLM_HORIZON_METRICS);
  response->request_url = sdscpy(response->request_url, request);
  m_requests_get(response);
}
