# C Stellar SDK (unofficial)

`c_stellar_sdk` is an *unofficial* header-only lightweight pure C library to interface with the [**Stellar Horizon API server**](https://www.stellar.org/developers/horizon/reference/index.html), suitable for doing high-performance analytics on Stellar data.

It's intended to be a C port of [js-stellar-sdk](https://github.com/stellar/js-stellar-sdk), but
it also provides custom helpers for getting financials (eg. time series for price and volume).


## What is Stellar?

Stellar is a **descentralized federated financial network** built on [**blockchain technology**](https://en.wikipedia.org/wiki/Blockchain) with a **consensus algorithm** based (not on [proof-of-work](https://en.bitcoin.it/wiki/Proof_of_work) but) on a [**federated Byzantine agreement system**](https://www.stellar.org/blog/stellar-consensus-protocol-proof-code/) called the [**Stellar Consensus Protocol**](https://www.stellar.org/papers/stellar-consensus-protocol.pdf).

The Stellar network maintains a sequence of so-called [**ledgers**](https://www.stellar.org/developers/guides/concepts/ledger.html), chained together by their hashes.


## Capabilities

With `c_stellar_sdk` you can:

1. Get **data** from the Stellar descentralized exchange: prices, orders
1. Query the **state** of the network:
[accounts](https://www.stellar.org/developers/horizon/reference/resources/account.html),
[ledgers](https://www.stellar.org/developers/horizon/reference/resources/ledger.html),
[transactions](https://www.stellar.org/developers/horizon/reference/resources/transaction.html),
[operations](https://www.stellar.org/developers/horizon/reference/resources/operation.html),
[effects](https://www.stellar.org/developers/horizon/reference/resources/effect.html),
payments,
[orderbooks](https://www.stellar.org/developers/horizon/reference/resources/orderbook.html),
[offers](https://www.stellar.org/developers/horizon/reference/resources/offer.html)
1. Submit **transactions** to the network


## Implementation details

Under the hood, the C Stellar SDK uses
[**SDS**](https://github.com/antirez/sds) (of [Redis](https://github.com/antirez/redis) fame) to simplify and abstract *string* operations,
[**cJSON**](https://github.com/DaveGamble/cJSON) for *JSON* parsing, and
[**libcurl**](https://curl.haxx.se/libcurl/) for sync/async *HTTP requests*.

These libraries are wrapped in a thin abstraction layer, so that high-level functions remain oblivious to them,
and they may be replaced by similar libraries, provided one adapts the internals of the abstraction layer keeping the external interface consistent.


## Overview of the API

The Horizon API supports multiple types of requests, with varying degrees of complexity.
Very roughly, the complexity of requests can be classified as follows.

1. Requests that take no parameters. Example: Horizon metrics
1. Requests that take one set of parameters. Example: ledgers
1. Requests that take multiple sets of parameters. Example: transactions for account
1. Requests that take multiple sets of parameters and require authentication.

## Dependencies

1. [libcurl](https://curl.haxx.se/libcurl/) should be installed and linkable (eg. `gcc main.c -lcurl`)

On **Ubuntu**, run `sudo apt-get install libcurl4-openssl-dev`. On **OS X**, run `brew install curl`.


## Build

Assuming the dependencies are installed, you can run:

    git clone https://github.com/etale-cohomology/c_stellar_sdk.git
    cd c_stellar_sdk
    make


## Build the docs

The documentation is written in Markdown and rendered via [MkDocs](http://www.mkdocs.org/) (with the [Material theme](http://squidfunk.github.io/mkdocs-material/) and the [CodeHilite](http://squidfunk.github.io/mkdocs-material/extensions/codehilite/) extension). You can install these with `pip`:

    pip install mkdocs
    pip install mkdocs-material
    pip install pygments

Then, from the root directory, run

    make docs

to build the HTML/CSS/JS files into a `site/` directory, or run

    make docs_serve

to serve it locally on `http://127.0.0.1:8000`.


## Build examples manually

    gcc xlm_example_trades


## TODOs

- Decode XDR blobs
- Perform more operations (eg. `change_trust`, `manage_offer`)
- Write a high-level API for event streams (currently accessible by low-level routines)
- Write a high-level API for async requests (currently accessible by low-level routines)
- Write tests


## License

MIT
