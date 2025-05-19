import json
import os
from constants import MIN_PASSWORD_LENGTH, STRATEGY_OPTIONS, INITIAL_BALANCE, USER_FILE_NAME, StrategyType, PROGRAM_OPTIONS
from market import Market

users = {}


class User:
    def __init__(self, username: str, password: str, strategy: StrategyType, auto: bool = False, balance: float = INITIAL_BALANCE):
        self.username = username
        self.password = password
        self.strategy = strategy
        self.auto = auto
        self.balance = balance
        self.portfolio = {}

    def to_dict(self):
        return {
            'password': self.password,
            'strategy': self.strategy.name,
            'auto': self.auto,
            'balance': self.balance,
            'portfolio': self.portfolio
        }
    
    def toggle_auto(self):
        self.auto = not self.auto
        
    def buy_stock(self, ticker: str, qty: int):
        '''
        1. Display user's current balance and portfolio.
        2. Prompt: "Enter stock ticker and quantity to buy”
        3. Validate input:
            • Ticker must be valid (AAPL, TSLA, GOOG).
            • Quantity must be a positive integer.
        4. Check sufficient balance.
        5. If valid, execute trade:
            • Deduct cost from balance.
            • Add shares to portfolio.
        6. Log transaction in transactions.json
        '''
        pass
    
    def sell_stock(self, ticker: str, qty: int):
        '''
        1. Display user's current balance and portfolio.
        2. Prompt: "Enter stock ticker and quantity to sell”
        3. Validate input:
            • Ticker must be valid (AAPL, TSLA, GOOG).
            • Quantity must be a positive integer.
        4. Check sufficient shares.
        5. If valid, execute trade:
            • Calculates the proceeds = current price × quantity.
            • Adds the proceeds to the user's balance.
            • Deducts the shares from the user's portfolio.
            • If the number of shares reaches zero, the stock is removed from the portfolio.
            • Log transaction in transactions.json
        '''
        pass
    



def load_users():
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
        except json.JSONDecodeError:
            # print("Error loading user data.")
            users.clear()
            save_users()


def save_users():
    json_users = {username: user.to_dict() for username, user in users.items()}

    with open(USER_FILE_NAME, 'w') as file:
        json.dump(json_users, file, indent=2)


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


def login() -> User:
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
                print("Buy")
            elif options == "3":
                print("Sell")
            elif options == "4":
                print("Portfolio")
            elif options == "5":
                print("History")
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