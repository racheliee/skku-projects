import json
import os
from datetime import datetime
from constants import TRANSACTION_FILE_NAME, ActionType
from dataclasses import dataclass, asdict
from typing import Dict, List, Self


@dataclass
class Holding:
    qty:       int
    avg_price: float

    def to_dict(self) -> Dict:
        return {
            'qty':       self.qty,
            'avg_price': self.avg_price
        }

    def from_dict(d: dict) -> Self:
        return Holding(
            qty=d['qty'],
            avg_price=d['avg_price']
        )


@dataclass
class Transaction:
    time:   str
    ticker: str
    action: ActionType
    qty:    int
    price:  float

    def to_dict(self) -> Dict:
        d = asdict(self)
        d["action"] = self.action.value
        return d


transactions: Dict[str, List[Transaction]] = {}


def load_transactions() -> None:
    global transactions

    if not os.path.exists(TRANSACTION_FILE_NAME):
        transactions = {}
        save_transactions()
        return

    with open(TRANSACTION_FILE_NAME, 'r') as file:
        try:
            raw_data = json.load(file)
        except json.JSONDecodeError:
            transactions = {}
            save_transactions()
            return

    transactions = {
        user: [
            Transaction(
                time=t["time"],
                ticker=t["ticker"],
                action=ActionType(t["action"]),
                qty=t["qty"],
                price=t["price"]
            )
            for t in history
        ]
        for user, history in raw_data.items()
    }


def save_transactions() -> None:
    data = {
        user: [t.to_dict() for t in history]
        for user, history in transactions.items()
    }
    with open(TRANSACTION_FILE_NAME, 'w') as file:
        json.dump(data, file, indent=2)


def log_transaction(username: str, ticker: str, action: str, qty: int, price: float):
    ts = Transaction(
        time=datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        ticker=ticker,
        action=action,
        qty=qty,
        price=price,
    )

    transactions.setdefault(username, []).append(ts)

    save_transactions()  # optional probs; transaction history should be saved at exit


def display_history(username: str) -> None:
    history = transactions.get(username, [])

    if not history:
        print("\nNo transaction history found.")
        return

    print("\n==== Transactions ====")
    for ts in reversed(history):
        print(f"{ts.time} - {ts.action.value} {ts.qty} {ts.ticker} @${ts.price:.2f}")
    print()
