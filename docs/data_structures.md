# Data structures

####  xlm_Horizon

```c
struct xlm_Horizon{
xlm_str root_url;
};

void xlm_horizon_init(struct xlm_Horizon* horizon, const char* root){
}

void xlm_horizon_free(struct xlm_Horizon* horizon){
}
```

#### xlm_Response

```c
struct xlm_Response{
CURL* curl;
CURLcode curl_code;
xlm_str raw;
xlm_str request_url;
};

void xlm_response_init(struct xlm_Response* response){
}

void xlm_response_free(struct xlm_Response* response){
}

// Print a request to console!
void xlm_show_request(struct xlm_Response* response){
}

// Print a response to console, as a raw string!
void xlm_show_raw(struct xlm_Response* response){
} 
```

#### xlm_Request_params

```c
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
```
