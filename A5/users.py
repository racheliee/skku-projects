import json
import os
from constants import MIN_PASSWORD_LENGTH, STRATEGY_OPTIONS, INITIAL_BALANCE, USER_FILE_NAME, StrategyType, PROGRAM_OPTIONS, DEFAULT_STOCK, ActionType
from market import Market
from typing import Dict
from dataclasses import dataclass, field
from transactions import log_transaction, display_history, Holding


@dataclass
class User:
    username: str
    password: str
    strategy: StrategyType
    auto: bool = False
    balance: float = INITIAL_BALANCE
    portfolio: Dict[str, Holding] = field(default_factory=dict)

    def to_dict(self) -> Dict:
        return {
            'password': self.password,
            'strategy': self.strategy.name,
            'auto': self.auto,
            'balance': self.balance,
            'portfolio': self.portfolio
        }

    def toggle_auto(self) -> None:
        self.auto = not self.auto

        if self.auto:
            print("Auto-trade enabled")
        else:
            print("Auto-trade disabled")

    def execute_trade(self, ticker: str, qty: int, market: Market, action: ActionType, auto=False) -> None:
        # check balance
        curr_price = market.stocks[ticker].get_current_price()
        if curr_price is None:
            if not auto:
                print("Market data not yet available. Please try again in 10 seconds.")
            return

        if action == ActionType.BUY:
            cost = curr_price * qty
            if cost > self.balance:
                if not auto:
                    print(f"Insufficient funds to buy {qty} shares of {ticker}.")
                return

            # execute trade
            self.balance -= cost
            current_holdings = self.portfolio.get(ticker, Holding(0, 0.0))
            new_qty = current_holdings.qty + qty
            new_avg_price = (
                (current_holdings.avg_price * current_holdings.qty) + (curr_price * qty)) / new_qty  # weighted avg
            self.portfolio[ticker] = Holding(new_qty, new_avg_price)

        elif action == ActionType.SELL:
            # check if user owns the stock
            if ticker not in self.portfolio:
                if not auto:
                    print(f"You don't own any shares of {ticker}.")
                return
            
            # check quantity
            holding = self.portfolio[ticker]
            if qty > holding.qty:
                print(f"You only have {holding.qty} shares of {ticker}.")
                return
            # execute trade
            proceeds = curr_price * qty
            self.balance += proceeds

            remaining = holding.qty - qty
            if remaining > 0:
                self.portfolio[ticker] = Holding(remaining, holding.avg_price)
            else:
                del self.portfolio[ticker]

        # log transaction
        log_transaction(self.username, ticker, action, qty, curr_price)

        if not auto:
            action_str = "Bought" if action == ActionType.BUY else "Sold"
            print(f"{action_str} {qty} {ticker} @ ${curr_price:.2f}")

    def get_trade_options(self, action: ActionType) -> tuple[str, int] | None:
        menu = "Buy" if action == ActionType.BUY else "Sell"
        print(f"====== {menu} Menu ======"
              + f"\nAvailable Cash: ${self.balance:.2f}"
              + "\nYour Holdings:")

        if len(self.portfolio) == 0:
            print("    (No stocks owned)")
        else:
            for stock, h in self.portfolio.items():
                print(
                    f"    {stock}: {h.qty} shares @ avg {h.avg_price:.2f}")

        # get ticker
        ticker_prompt = DEFAULT_STOCK if action == ActionType.BUY else self.portfolio.keys()
        while True:
            ticker = input(
                f"Enter ticker ({ticker_prompt}) or 'back' to return: ").upper()
            if ticker == 'BACK':
                return None
            if ticker not in DEFAULT_STOCK:
                print("Invalid ticker. Please try again.")
                continue
            break

        # get quantity
        while True:
            action_str = "buy" if action == ActionType.BUY else "sell"
            qty_input = input(f"Enter quantity to {action_str}: ")
            try:
                qty = int(qty_input)
                if qty <= 0:
                    raise ValueError
                break
            except ValueError:
                print("Invalid quantity. Please enter a positive integer.")

        return ticker, int(qty)

    def buy_stock(self, market: Market) -> None:
        result = self.get_trade_options(ActionType.BUY)
        if result is None:
            return
        ticker, qty = result
        self.execute_trade(ticker, qty, market, ActionType.BUY)

    def sell_stock(self, market: Market) -> None:
        result = self.get_trade_options(ActionType.SELL)
        if result is None:
            return
        ticker, qty = result
        self.execute_trade(ticker, qty, market, ActionType.SELL)

    def portfolio_summary(self, market: Market) -> None:
        print(f"\nCash: ${self.balance:.2f}")

        total_eval = self.balance
        for ticker, h in self.portfolio.items():
            curr_price = market.stocks[ticker].get_current_price()

            pos_val = h.qty * curr_price if curr_price else 0
            total_eval += pos_val

            if h.avg_price > 0:
                pl_percentage = (
                    (curr_price - h.avg_price) / h.avg_price) * 100
            else:
                pl_percentage = 0.0
            sign = "+" if pl_percentage >= 0 else ""

            print(
                f"{ticker}: {h.qty} @avg${h.avg_price:.2f} -> ${curr_price:.2f}  ({sign}{pl_percentage:.2f}%)")

        print(f"Total: ${total_eval:.2f}")
        return


users: Dict[str, User] = {}


def load_users() -> None:
    # create file if it doesn't exist
    if not os.path.exists(USER_FILE_NAME):
        with open(USER_FILE_NAME, 'w') as file:
            json.dump({}, file)

    # load users from file
    global users
    users = {}

    with open(USER_FILE_NAME, 'r') as file:
        try:
            data = json.load(file)
            for username, user_data in data.items():
                users[username] = User(
                    username=username,
                    password=user_data["password"],
                    strategy=StrategyType[user_data["strategy"]],
                    auto=user_data["auto"],
                    balance=user_data["balance"]
                )
                for ticker, h in user_data["portfolio"].items():
                    users[username].portfolio[ticker] = Holding.from_dict(h)
        except json.JSONDecodeError:
            # print("Error loading user data.")
            users.clear()
            save_users()


def save_users() -> None:
    serializable = {}
    for username, user in users.items():
        data = {
            "password": user.password,
            "strategy": user.strategy.name,
            "auto":     user.auto,
            "balance":  user.balance,
            "portfolio": {
                ticker: hold.to_dict()
                for ticker, hold in user.portfolio.items()
            }
        }
        serializable[username] = data

    with open(USER_FILE_NAME, "w") as f:
        json.dump(serializable, f, indent=2)


def _is_valid_reg_username(username: str) -> bool:
    if username in users:
        print("Username already exists. Please choose another.")
        return False
    return True


def _is_valid_reg_password(password: str) -> bool:
    errors = ""
    special_chars = "!@#$%^&*()"
    if len(password) < MIN_PASSWORD_LENGTH:
        errors += f" - Password must be at least {MIN_PASSWORD_LENGTH} characters.\n"
    if not any(char.isupper() for char in password):
        errors += " - Password must include at least one uppercase letter.\n"
    if not any(char in special_chars for char in password):
        errors += f" - Password must include at least one special character ({special_chars})."

    if errors == "":
        return True
    else:
        print(errors)
        print("Please enter a valid password.")
        return False


def register() -> None:
    # get username
    username = input("Username: ")
    while not _is_valid_reg_username(username):
        username = input("Username: ")

    # get password
    password = input("Password: ")
    while not _is_valid_reg_password(password):
        password = input("Password: ")

    # get strategy
    strategy = None
    while strategy is None:
        try:
            strategy_input = int(input(STRATEGY_OPTIONS))
            strategy = StrategyType(strategy_input)
        except (ValueError, KeyError):
            print("Invalid selection. Try again.")

    user = User(username, password, strategy)
    users[username] = user

    print(f"User '{username}' registered successfully.")
    return


def login() -> User | None:
    username = input("Username: ")
    password = input("Password: ")

    if username in users and users[username].password == password:
        print(f"Welcome, {username}!")
        return users[username]

    else:
        print("Invalid username or password.")
        return None


def login_options(user: User, market: Market) -> None:
    while True:
        try:
            print(PROGRAM_OPTIONS)
            options = input(f"{user.username} > ")
            if options == "1":
                market.view()
            elif options == "2":
                user.buy_stock(market)
            elif options == "3":
                user.sell_stock(market)
            elif options == "4":
                user.portfolio_summary(market)
            elif options == "5":
                display_history(user.username)
            elif options == "6":
                user.toggle_auto()
            elif options == "7":
                print("Logout")
                break
            else:
                print("Invalid selection. Please try again.")
        except ValueError:
            print("Invalid input. Please enter a number.")
        except (KeyboardInterrupt, EOFError):
            print("\nUser interrupted. Logging out...")
            break
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
    return
