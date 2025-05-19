from abc import ABC, abstractmethod
from users import User
from market import Market


class Strategy(ABC):
    @abstractmethod
    def execute(self, user: User, market: Market) -> None: ...


class RandomStrategy(Strategy):
    def execute(self, user, market):
        ...


class MovingAverageStrategy(Strategy):
    def execute(self, user, market):
        ...


class MomentumStrategy(Strategy):
    def execute(self, user, market):
        ...
