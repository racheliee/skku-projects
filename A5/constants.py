from enum import Enum
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(BASE_DIR, 'data')
os.makedirs(DATA_DIR, exist_ok=True)

MARKET_FILE_NAME = os.path.join(DATA_DIR, 'market.json')

USER_FILE_NAME = os.path.join(DATA_DIR, 'users.json')

TRANSACTION_FILE_NAME = os.path.join(DATA_DIR, 'transactions.json')

MIN_PASSWORD_LENGTH = 8

INITIAL_BALANCE = 10000

MARKET_UPDATE_INTERVAL = 10

MAX_MARKET_HISTORY = 30

MIN_FLUCTUATION = -100

MAX_FLUCTUATION = 100

class StrategyType(Enum):
    RANDOM = 1
    MOVING_AVERAGE = 2
    MOMENTUM = 3

DEFAULT_STOCK = ["AAPL", "GOOGL", "TSLA"]

PROGRAM_PROMPT = '''
=== Stock Trading Simulation ===
1) Register   2) Login   3) Exit
Select: '''

STRATEGY_OPTIONS = '''Select Strategy:
  1) Random Strategy
  2) Moving Average Strategy
  3) Momentum Strategy
Choose (1/2/3): '''

PROGRAM_OPTIONS = '''
=== Select Option ===
1. View : View current price and volume for all tickers
2. Buy : Purchase a quantity of a specific stock
3. Sell : Sell a quantity of a specific stock
4. Portfolio : Show the user's cash and stock holdings
5. History : View transaction history
6. Auto on/off : Enable or disable auto-trading using the selected strategy
7. Logout : Return to the initial login/register screen
'''
