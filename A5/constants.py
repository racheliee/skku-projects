from enum import Enum
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.join(BASE_DIR, 'data')

MARKET_FILE_NAME = os.path.join(DATA_DIR, 'market.json')

USER_FILE_NAME = os.path.join(DATA_DIR, 'users.json')

TRANSACTION_FILE_NAME = os.path.join(DATA_DIR, 'transactions.json')

MIN_PASSWORD_LENGTH = 8

INITIAL_BALANCE = 10000

class StrategyType(Enum):
    RANDOM = 1
    MOVING_AVERAGE = 2
    MOMENTUM = 3


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
1. view
2. 
'''
