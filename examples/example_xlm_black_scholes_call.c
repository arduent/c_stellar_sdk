#include "stellar_sdk.h"
#include <math.h>

// ----------------------------------------------------------------------
float rational_approx(float t){
    const float c[] = {2.515517, 0.802853, 0.010328};
    const float d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1]) * t + c[0]) / (((d[2]*t + d[1]) * t + d[0]) * t + 1.0);
}

// Given a probability p, find the real x
float inv_cumulative_normal_density(float p){
  if(p < .5)  return -rational_approx(sqrt(-2. * log(p)) );
  else        return  rational_approx(sqrt(-2. * log(1 - p)));
}

int gaussian_copula(){
  float* asset0_price;
  float* asset1_price;
  float* asset0_logreturns;
  float* asset1_logreturns;
  float correlation;
}

int VaR(){
  float days = 180;
  float confidence = .95;
}

int garch(float logreturn_lag, float variance_lag){
  float long_run_variance = 1;
  float logreturn_lagged = .05;
  float variance_lagged = .1;

  float gamma = .3;
  float alpha = .3;
  float beta = .4;
  return gamma * long_run_variance + alpha * logreturn_lagged + beta * variance_lagged;
}

// ----------------------------------------------------------------------
// Given a real x, find the probability p
float cumulative_normal_density(float x){
  return .5 * erfc(-x * M_SQRT1_2);
}

float blackscholes_d1(float asset_price, float asset_std, float strike_price, float days2expiration, float risk_free_rate){
  float top0 = log(asset_price / strike_price);
  float top1 = days2expiration * (risk_free_rate + .5 * asset_std * asset_std);
  float bottom = asset_std * sqrt(days2expiration);
  return (top0 + top1) / bottom;
}

float blackscholes_d2(float asset_price, float asset_std, float strike_price, float days2expiration, float risk_free_rate){
  float top0 = log(asset_price / strike_price);
  float top1 = days2expiration * (risk_free_rate - .5 * asset_std * asset_std);
  float bottom = asset_std * sqrt(days2expiration);
  return (top0 + top1) / bottom;
}

float blackscholes_call(float asset_price, float asset_std, float strike_price, float days2expiration, float risk_free_rate){
  float dsc_asset_price = asset_price * exp(-risk_free_rate * days2expiration);
  float d1 = blackscholes_d1(asset_std, asset_std, strike_price, days2expiration, risk_free_rate);
  float d2 = blackscholes_d2(asset_std, asset_std, strike_price, days2expiration, risk_free_rate);
  return strike_price * cumulative_normal_density(d1) - dsc_asset_price * cumulative_normal_density(d2);
}

int main(){
  struct xlm_Horizon horizon;
  xlm_horizon_init(&horizon, XLM_HORIZON_LIVE);  // Also try `XLM_HORIZON_TESTNET`

  struct xlm_Request_params request_params = {
    .cursor = 60544972855709697,
    .order = "desc",
    .limit = 4,
  };

  struct xlm_Response response;
  xlm_response_init(&response);

  // xlm_orderbook_trades(&horizon, &response, &request_params);  // Send the request!
  // xlm_JSON* json_root = json_parse(response.raw);  // Parse a raw JSON string!
  // json_show(json_root);

  // ---------------------
  float asset_price = .02;
  float asset_std = .05;
  float strike_price = .01;
  float days2expiration = 180;
  float risk_free_rate = .01;
  float call_price = blackscholes_call(asset_price, asset_std, strike_price, days2expiration, risk_free_rate);
  printf("Call option price:  %.6f\n", call_price);

  // ---------------------
  // json_free(json_root);  // Deleting the root takes care of everything
  xlm_response_free(&response);
  xlm_horizon_free(&horizon);
  m_exit_success();
}
