import atexit
from constants import PROGRAM_PROMPT
from market import Market
from users import register, login, save_users, load_users, login_options
from transactions import load_transactions, save_transactions

global_market = None 

def _setup():
    global global_market
    load_users()
    load_transactions()
    global_market = Market()
    global_market.open()


def _cleanup():
    save_users()
    save_transactions()
    global_market.close()
    return


atexit.register(_cleanup)  # run cleanup on exit


def main():  # program entry point
    user = None

    while True:
        try:
            menu = input(PROGRAM_PROMPT)
            if menu == "1":
                register()
            elif menu == "2":
                user = login()
                if user is None:
                    continue
                else:
                    login_options(user, global_market)
                    user = None
            elif menu == "3":
                print("Goodbye!")
                break
            else:
                print("Invalid selection. Please try again.")
        except ValueError:
            print("Invalid input. Please enter a number.")
        except (KeyboardInterrupt, EOFError):
            print("\nGoodbye!")
            break


if __name__ == "__main__":
    _setup()
    main()
