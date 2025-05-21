import random
from abc import ABC, abstractmethod
from users import User
from market import Market
from constants import DEFAULT_STOCK, ActionType, StrategyType, STRATEGY_SHORT, STRATEGY_LONG, STRATEGY_MOMENTUM
from statistics import mean


class Strategy(ABC):
    @abstractmethod
    def execute(self, user: User, market: Market) -> None: ...


class RandomStrategy(Strategy):
    def execute(self, user, market) -> None:
        if random.random() < 0.5:
            ticker = random.choice(DEFAULT_STOCK)
            qty = random.randint(1, 5)

            # print(f"Auto-trading (random): Buying {qty} shares of {ticker}")
            user.execute_trade(ticker, qty, market, ActionType.BUY, auto=True)


class MovingAverageStrategy(Strategy):
    def execute(self, user, market) -> None:
        for ticker, stock in market.stocks.items():
            stock_prices = [p['price'] for p in stock.history]

            if len(stock_prices) < STRATEGY_LONG:
                # print("Auto-trading is on pause until enough market data is available.")
                continue

            short_avg = mean(stock_prices[-STRATEGY_SHORT:])
            long_avg = mean(stock_prices[-STRATEGY_LONG:])
            
            # print(f"Auto-trading (moving average): {ticker} short avg: {short_avg:.2f}, long avg: {long_avg:.2f}")

            if short_avg > long_avg:
                user.execute_trade(ticker, 1, market, ActionType.BUY, auto=True)
            else:
                user.execute_trade(ticker, 1, market, ActionType.SELL, auto=True)


class MomentumStrategy(Strategy):
    def execute(self, user, market) -> None:
        for ticker, stock in market.stocks.items():
            stock_prices = [p['price'] for p in stock.history]

            if len(stock_prices) < STRATEGY_MOMENTUM + 1:
                # print("Auto-trading is on pause until enough market data is available.")
                continue
            
            old_price = stock_prices[-STRATEGY_MOMENTUM - 1]
            curr_price = stock_prices[-1]
            
            # print(f"Auto-trading (momentum): {ticker} old price: {old_price:.2f}, current price: {curr_price:.2f}")
            
            if curr_price < old_price:
                user.execute_trade(ticker, 1, market, ActionType.SELL, auto=True)
            else:
                user.execute_trade(ticker, 1, market, ActionType.BUY, auto=True)


STRATEGY_OPTIONS = {
    StrategyType.RANDOM:         RandomStrategy(),
    StrategyType.MOVING_AVERAGE: MovingAverageStrategy(),
    StrategyType.MOMENTUM:       MomentumStrategy(),
}
        