import atexit
from constants import PROGRAM_PROMPT
from users import register, login, save_users, load_users


def _setup():
    load_users()


def _cleanup():
    save_users()
    return


atexit.register(_cleanup)  # run cleanup function on exit


def main():  # main function where the program starts
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
                    pass
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
