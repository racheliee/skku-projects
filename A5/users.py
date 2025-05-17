import json
import os
from constants import MIN_PASSWORD_LENGTH, STRATEGY_OPTIONS, INITIAL_BALANCE, USER_FILE_NAME, StrategyType

users = {}


class User:
    def __init__(self, username: str, password: str, strategy: StrategyType, balance: float = INITIAL_BALANCE):
        self.username = username
        self.password = password
        self.strategy = strategy
        self.balance = balance
        self.portfolio = {}

    def to_dict(self):
        return {
            'username': self.username,
            'password': self.password,
            'strategy': self.strategy.name,
            'balance': self.balance,
            'portfolio': self.portfolio
        }


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


def login() -> bool:
    username = input("Username: ")
    password = input("Password: ")
    
    if username in users and users[username].password == password:
        print(f"Welcome, {username}!")
        return True

    else:
        print("Invalid username or password.")
        return False
