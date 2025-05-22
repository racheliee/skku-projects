# Assignment 5: Stock Trading Simulation

**Grade:** N/A

**Language:** Python 3.11



## 🧠 Objective

To implement a multi-user stock trading system in Python using modular design and persistent JSON storage. The system supports manual and automatic trading with real-time market simulation.

## 📁 Directory Structure

```
A5/
├── trading.py               # Main entry point (named 2022310853_Trading.py in submission)
├── market.py                # Market simulation and auto-trading engine
├── users.py                 # User management and portfolio logic
├── strategies.py            # Auto-trading strategy implementations
├── transactions.py          # Transaction logging and history
├── constants.py             # Enums and constants
└── data/
    ├── market.json          # Live market prices and history
    ├── users.json           # Registered users and portfolios
    └── transactions.json    # Transaction logs
```


## 🚀 Features

### 👥 User Management

* **Register/Login/Logout** via CLI
* Username & password (case-sensitive)
* Each user selects a trading strategy on registration

### 💹 Trading Interface

* View real-time stock prices
* Buy/Sell stocks with:

  * Symbol validation
  * Balance/share checks
  * Weighted average price calculations
* Portfolio updates stored persistently
* Transactions logged in `transactions.json`

### 📊 Portfolio & History

* Per-user portfolio with:

  * Ticker
  * Quantity owned
  * Average purchase price
* Transaction history includes:

  * Action type (buy/sell)
  * Price, quantity, timestamp

### 🤖 Auto-Trading (Background Thread)

* Continues trading while user is logged out
* Strategies:

  * `RandomStrategy`: Trades randomly
  * `MomentumStrategy`: Buys rising stocks, sells falling ones
  * `MovingAverageStrategy`: Trades based on moving average
* Runs in a daemon thread with thread-safe market updates

### 💾 JSON Persistence

* All state stored in:

  * `market.json`: Stock history
  * `users.json`: Accounts, portfolios, strategies
  * `transactions.json`: Per-user trade logs
* Graceful shutdown with `atexit` cleanup


## 🛠 Tech Notes

* Python 3.11 required (uses `| None` syntax)
* Fully modular and extensible system
* Threading used for market simulation
* CLI-based interface with friendly prompts


Read this [report](./a5.pdf) for a detailed analysis of the design and implementation as well as example usage.