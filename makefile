CC = gcc
# CFLAGS = -Wall -Wextra -pedantic

SOURCE_SDS 	= sds/sds.c
SOURCE_JSON = cJSON/cJSON.c

LIB_CURL 		= `curl-config --libs`
LIB_MATH 		= -lm
LIB_PTHREAD = -lpthread

DIR_TESTS 		= tests
DIR_EXAMPLES 	= examples
DIR_DOCS 			= docs


OUT  			= xlm_test0 xlm_test2 xlm_test3 xlm_test4 xlm_test7
TESTS 		= test_xlm_0
EXAMPLES 	= example_curl_hello example_curl_async example_curl_pthreads
EXAMPLES += example_json_0 example_json_1
EXAMPLES += example_xlm_black_scholes_call example_xlm_account_balances
EXAMPLES += example_xlm_ledgers_first example_xlm_ledgers_all
EXAMPLES += example_xlm_metrics
EXAMPLES += example_xlm_debug_request


all: $(OUT) $(TESTS) $(EXAMPLES)
tests: $(TESTS)
examples: $(EXAMPLES)


xlm_test0: xlm_test0.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)

xlm_test2: xlm_test2.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)

xlm_test3: xlm_test3.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)

xlm_test4: xlm_test4.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)

xlm_test7: xlm_test7.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


test_xlm_0: $(DIR_TESTS)/test_xlm_0.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


example_curl_hello: $(DIR_EXAMPLES)/example_curl_hello.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(LIB_CURL)

example_curl_async: $(DIR_EXAMPLES)/example_curl_async.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(LIB_CURL)

example_curl_pthreads: $(DIR_EXAMPLES)/example_curl_pthreads.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(LIB_CURL) $(LIB_PTHREAD)


example_json_0: $(DIR_EXAMPLES)/example_json_0.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)

example_json_1: $(DIR_EXAMPLES)/example_json_1.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


example_xlm_black_scholes_call: $(DIR_EXAMPLES)/example_xlm_black_scholes_call.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL) $(LIB_MATH)

example_xlm_account_balances: $(DIR_EXAMPLES)/example_xlm_account_balances.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


example_xlm_ledgers_first: $(DIR_EXAMPLES)/example_xlm_ledgers_first.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)

example_xlm_ledgers_all: $(DIR_EXAMPLES)/example_xlm_ledgers_all.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


example_xlm_metrics: $(DIR_EXAMPLES)/example_xlm_metrics.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


example_xlm_debug_request: $(DIR_EXAMPLES)/example_xlm_debug_request.c
	$(CC) $^ -o $@ $(CFLAGS) $(SOURCE_SDS) $(SOURCE_JSON) $(LIB_CURL)


docs: $(DIR_DOCS)/*
	cp readme.md $(DIR_DOCS)/index.md && mkdocs build

docs_serve:
	mkdocs serve


clean:
	rm -f $(OUT)
	rm -f $(TESTS)
	rm -f $(EXAMPLES)

.PHONY: all clean tests examples
