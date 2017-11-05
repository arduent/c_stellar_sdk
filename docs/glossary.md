# Glossary


---------------------------------------------------------------------------------------------------
## Stellar

### Asset

An **asset** in Stellar is a *security* issued by a specific **account** that can be traded in the Stellar exchange.

Assets can be "cashed out" with the asset issuer for whatever it is that the asset is intended to represent.
Accounts that issue assets are informally called **anchors**.

*Any* account can issue *any* asset to the network and call it *anything* they like,
so you must **trust** an account before you can hold assets it has issued.

An **asset** is given by the following data:

- the asset **issuer**
- the asset **type**
- the asset **code**.

The only exception to this rule is the *native* asset of the network, called the **lumen**, which only has an asset type: **native**.

### Anchor

An **anchor** in Stellar is an **account** that issues assets and honors withdrawals for those assets.

By honoring withdrawals, anchors effectively serve as the link between the Stellar network and the physical world.
They're the entities that allow you, for instance,
to hold cows in the Stellar network (which are sequences of zeros and ones) and "cash them out" for actual cows (which produce milk).

Anchors have total control over:

- which accounts can hold their assets,
- who the holders of their assets can trade those assets with, and
- when the holders can withdraw those assets.

The notion of an anchor in the Stellar network is a *generalization of the notion of a bank in the physical world*.
A traditional bank holds cash and honors withdrawals for cash.
A Stellar anchor 

### Account

An **account** in Stellar is a special kind of entry in the ledger that can hold balances, issue assets, and trade assets.

An account is publicly identified by its **public key**.
*Control* over an account rests with whoever knows the **secret seed** or the **secret key**.

Example of a **public key**: `GAPR4YI2IPPL7OBA6KG24OCQ4N35WHKNJEJIDTFOLNHLJUPDHACRX5AU`
Example of a **secret seed**: `SBUNGVT2C4PHIC446T4QOFZ7LFD5NFXSJ46BIAME25R5DEXP24J765H5`

### Secret seed

The **secret seed** is a cryptographic object associated to an **account** from the which you can recover both the account's public key and private key.

In particular, this means that whoever knows the secret seed has control over the account.


---------------------------------------------------------------------------------------------------
## Trading

### Asset

In trading, an **asset** is anything that be bought and sold (ie. *traded*) in a **market**.

### Market

A **market** is a place (physical or digital) where buyers meet sellers.

A **market** for an **asset** `X` is a place (physical or digital) where buyers of `X` meet sellers of `X`.

### Limit order

A **limit order** is an order to trade at a *given* price, and it lives in the **orderbook** until a market order clears it.

Limit orders provide liquidity to the market, by exposing themselves to be fulfilled by market orders.

### Market order

A **market order** is an order (to buy or sell) that is to be executed immediately, regardless of price.

Market orders take liquidity away from the market, by fulfilling limit orders.

### Orderbook

The **orderbook**, also called the **limit orderbook** or the book of **limit orders**, is the list of all limit orders (both **bids** and **asks**).

### Bid

A **bid offer** is a **limit order** for *buying* an **asset**.

A **bid price** is the price of a bid offer, typically the lowest one.

### Ask

An **ask offer** is a **limit order** for *selling* an **asset**.

An **ask price** is the price of an ask offer, typically the highest one.

### Price

In a **market**, there's no such thing as *the* price of an asset.
There's only the **bid price**, the **ask price**, and the **last price** (which is the price at which the last market order cleared a limit order).

The problem of executing a large market order at the best *average price* given the available orderbook is known to be a [stochastic control problem](https://www.quantstart.com/articles/high-frequency-trading-iii-optimal-execution). This is a problem in the first place because large market orders tend to **walk the orderbook**.

### Walking the orderbook

If a **market order** is larger then the best matching limit order, the market order executes the best limit order first and then the limit orders that come above (or below) it, which have worse prices (for the issuer of the market order) than the best limit order.

This process of clearing worse and worse limit orders with a market order is known as **walking the orderbook**.

### Liquidity

Intuitively, **liquidity** is a measure of how easy/hard it is to trade an asset.

The amount of limit orders in the **orderbook** is a rough/naive measure of liquidity.

### Market depth


---------------------------------------------------------------------------------------------------
## Probability

### Random variable

A [**random variable**](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=0ahUKEwjBl4ar9oLXAhUMNSYKHVd_B8QQFgg1MAE&url=https%3A%2F%2Fen.wikipedia.org%2Fwiki%2FRandom_variable&usg=AOvVaw3j4GvKQznPn0hgvOaM98f-) is a **function** from the **sample space** to the **state space**.

Random variables are not variables in the sense of high-school algebra, but functions in the sense of calculus.

### Stochastic process

A **stochastic process** is a **function** whose values are **random variables**.

**Random variables** are to **stochastic processes** as **numbers** are to **functions**.  
One observation/sample of a random variable yields a (single) number.  
One observation/sample of a random process yields a (single) function.

### Copula

A [**copula**](https://en.wikipedia.org/wiki/Copula_(probability_theory)) is a [probability density](https://en.wikipedia.org/wiki/Probability_density_function) that measures dependence among [random variables](https://en.wikipedia.org/wiki/Random_variable).
A (2-dimensional) copula takes 2 marginal/unconditional densities and "glues" them together to produce a [joint cumulative density](https://en.wikipedia.org/wiki/Cumulative_distribution_function#Multivariate_case) that contains all dependence data between the densities.
That is, the copula of 2 random variables is a [joint cumulative density](https://en.wikipedia.org/wiki/Cumulative_distribution_function#Multivariate_case).


---------------------------------------------------------------------------------------------------
## Calculus

### Differential

The **differential** of function `f` at a given point `a` is a **linear function** `df_a` which best approximates `f` (among all other linear functions) **near** the point `a`.

**Differentials** are very important in [integral calculus](https://www.encyclopediaofmath.org/index.php/Integral_calculus) because you can only **integrate** differentials.
They're also very important in stochastic calculus because a continuous-time **stochastic process** is always [**nowhere differentiable**](https://en.wikipedia.org/wiki/Weierstrass_function), but it may have a **differential**, and so you may be able to integrate it.


---------------------------------------------------------------------------------------------------
## Stochastic calculus

### Ito integral

The **Ito integral** is the stochastic calculus version of the Riemann integral.

### Ito's lemma

**It's lemma** is the stochastic calculus version of the [chain rule](https://en.wikipedia.org/wiki/Chain_rule).

### Brownian motion

**Brownian motion** is the random motion of particles in certain media.

**Brownian motion** also refers to the mathematical machinery used to model such movement, in which case Brownian motion means **Wiener process**.

### Wiener process

The [**Wiener process**](https://en.wikipedia.org/wiki/Wiener_process) is a continuous-time **stochastic process** with independent Gaussian increments and continuous paths.

The Wiener process can be constructed as some [limit](https://en.wikipedia.org/wiki/Convergence_of_random_variables) of discrete-time stochastic processes, such as the [scaling limit](https://en.wikipedia.org/wiki/Scaling_limit) of a [random walk](https://en.wikipedia.org/wiki/Random_walk). This is possible due to [Donsker's theorem](https://en.wikipedia.org/wiki/Donsker%27s_theorem), an extension to the [central limit theorem](https://en.wikipedia.org/wiki/Central_limit_theorem) which involves [**convergence in distribution**](https://en.wikipedia.org/wiki/Convergence_of_random_variables#Convergence_in_distribution) of a sequence of stochastic processes.

The Wiener process underlies the [Black-Scholes model](https://en.wikipedia.org/wiki/Black%E2%80%93Scholes_model) used in European [options](https://en.wikipedia.org/wiki/Option_(finance)) pricing.

## Markov process

## Levy process

## Gaussian process

---------------------------------------------------------------------------------------------------
## Algebra

### Ring

A [**ring**](https://en.wikipedia.org/wiki/Ring_(mathematics)) is a [set](https://en.wikipedia.org/wiki/Set_(mathematics)) where you can always add, subtract, and multiply elements (but not necessarily divide).

Rings are very important in pure mathematics, cryptography, and computer science.

Some examples:

- The set of all [integers](https://en.wikipedia.org/wiki/Integer) forms a ring denoted `Z` (from the German word *Zahlen*).
- The set of all unsigned 32-bit integers forms a (finite) ring denoted `Z/4294967296Z`, with `4294967296` elements. Notice that the integer `4294967296` is the 32-nd power of 2.
- The set of all unsigned 64-bit integers forms a (finite) ring denoted `Z/18446744073709551616Z`, with `18446744073709551616` elements. Notice that the integer `18446744073709551616` is the 64-th power of 2.
- The set of all continuous functions from the real numbers to the real numbers forms a ring.

### Field

A [**field**](https://en.wikipedia.org/wiki/Field_(mathematics)) is a [set](https://en.wikipedia.org/wiki/Set_(mathematics)) where you can always add, subtract, and multiply elements (but not necessarily divide).

Fields are very important in pure mathematics, cryptography, and computer science.

Finite fields are important in cryptography because computing [logarithms](https://en.wikipedia.org/wiki/Discrete_logarithm) is thought to be very expensive in large finite fields.

Some examples:

- The set of all [rational numbers](https://en.wikipedia.org/wiki/Rational_number) forms a field, denoted `Q`
- The set of all [constructible numbers](https://en.wikipedia.org/wiki/Constructible_numbers) forms a field.
- The set of all [real numbers](https://en.wikipedia.org/wiki/Real_number) forms a field, denoted `R`.
- The set of all [complex numbers](https://en.wikipedia.org/wiki/Complex_number) forms a field, denoted `C`.
- The ring `Z/3Z` is a (finite) field, also denoted `F3` (for *field with 3 elements*) or `GF(3)` (for *Galois field with 3 elements*).
- An infinite family of finite fields is given by fields of the form `Z/pZ`, where `p` is a prime number. At each prime `p`, the ring `Z/pZ` is a finite field, also denoted `Fp` (for *field with `p` elements*) or `GF(p)` (for *Galois field with `p` elements*).
- An infinite family of fields are given by [**global fields**](https://en.wikipedia.org/wiki/Global_field), which are finite-dimensional extensions of the field `Q` of rational numbers or finite-dimensional extensions of the field `Fq(t)` of rational functions over the finite field `Fq` (where `q` is a **prime power**).

### Absolute Galois group

The [**absolute Galois group**](https://en.wikipedia.org/wiki/Absolute_Galois_group) of a field `F`, denoted `Gal(F)`, is a **group** that encodes data for all the **separable extensions** of `F`.

The absolute Galois group of the finite field `GF(p)`, denoted `Gal(GF(p))`, is the [Prufer group](https://en.wikipedia.org/wiki/Pr%C3%BCfer_group).
