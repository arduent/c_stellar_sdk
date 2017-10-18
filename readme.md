# Stellar C SDK (unofficial)

This is a *community-maintained* C library to interface with the [**Stellar Horizon API server**](https://www.stellar.org/developers/horizon/reference/index.html).


# What is Stellar

Stellar is a **descentralized federated financial network** built on [**blockchain technology**](https://en.wikipedia.org/wiki/Blockchain) with a **consensus algorithm** based not on [proof-of-work](https://en.bitcoin.it/wiki/Proof_of_work) but on a **federated Byzantine agreement system** called the [**Stellar Consensus Protocol**](https://www.stellar.org/papers/stellar-consensus-protocol.pdf), by David Mazieres.

The Stellar network maintains a sequence of so-called [**ledgers**](https://www.stellar.org/developers/horizon/reference/resources/ledger.html), chained together by their hashes.


# Capabilities

The **Stellar C SDK** currently supports a limited subset of the full **Horizon API**. In particular, you can:

1. Query the state of the **network**: view
[accounts](https://www.stellar.org/developers/horizon/reference/resources/account.html),
[ledgers](https://www.stellar.org/developers/horizon/reference/resources/ledger.html),
[transactions](https://www.stellar.org/developers/horizon/reference/resources/transaction.html),
[operations](https://www.stellar.org/developers/horizon/reference/resources/operation.html),
[effects](https://www.stellar.org/developers/horizon/reference/resources/effect.html),
payments,
[orderbooks](https://www.stellar.org/developers/horizon/reference/resources/orderbook.html),
[offers](https://www.stellar.org/developers/horizon/reference/resources/offer.html)
1. Submit transactions to the network


# Examples

## View account balances


## View all ledgers

## View all transactions

## View all operations

## View all effects

## View all payments


## View ledgers for an account

## View transactions for an account

## View operations for an account

## View effects for an account

## View payments for an account

## View offers for an account

## View trades for an account


# Build

    git clone stellar-sdk-c
    cd stellar-sdk-c
    make


# Compile examples manually

    gcc xlm_example_0


# Run tests

    ./xlm_test_0
    ./xlm_test_1
    ./xlm_test_2


# TODOs

- Decode XDR blobs
- Build more complex transactions
- Perform more operations (eg. `change_trust`, `manage_offer`)
- Add async capabilities
- Subscribe to event streams

# License
