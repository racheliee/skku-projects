import os
import json
import threading
import time
import random
from datetime import datetime
from typing import Dict
from constants import MARKET_FILE_NAME, DEFAULT_STOCK, MARKET_UPDATE_INTERVAL, MAX_MARKET_HISTORY, MIN_FLUCTUATION, MAX_FLUCTUATION


class Stock:
    def __init__(self, ticker: str, history: list[dict]):
        self.ticker = ticker
        self.history = history

    def to_dict(self):
        return {
            'history': self.history
        }

    def get_current_price(self):
        return self.history[-1]['price'] if self.history else None

    def get_latest_volume(self):
        return self.history[-1]['volume'] if self.history else None

    def update(self, price: float, volume: int, timestamp: str):
        self.history.append({
            "time": timestamp,
            "price": price,
            "volume": volume
        })

        self.history = self.history[-MAX_MARKET_HISTORY:]


class Market:
    def __init__(self):
        self.stocks: Dict[str, Stock] = {}  # key: ticker, value: Stock
        self._running = False
        self.lock = threading.Lock()

    def _load_market(self):
        default_data = {t: {"history": []} for t in DEFAULT_STOCK}
        
        if not os.path.exists(MARKET_FILE_NAME):
            default_data = {ticker: {"history": []}
                            for ticker in DEFAULT_STOCK}
            with open(MARKET_FILE_NAME, 'w') as f:
                json.dump(default_data, f, indent=2)

        with open(MARKET_FILE_NAME, 'r') as f:
            try:
                raw_data = json.load(f)
            except json.JSONDecodeError:
                raw_data = {
                    ticker: {"history": []} for ticker in DEFAULT_STOCK
                }

        for ticker, data in raw_data.items():
            self.stocks[ticker] = Stock(ticker, data['history'])

    def _save_market(self):
        data = {ticker: stock.to_dict()
                for ticker, stock in self.stocks.items()}
        with open(MARKET_FILE_NAME, 'w') as f:
            json.dump(data, f, indent=2)

    def _update_market(self):
        for stock in self.stocks.values():
            latest_price = stock.get_current_price() or random.uniform(100, 500)
            new_price = round(
                max(1, latest_price +
                    random.uniform(MIN_FLUCTUATION, MAX_FLUCTUATION)), 2
            )

            latest_volume = stock.get_latest_volume() or random.randint(1, 1000)
            new_volume = max(1, latest_volume +
                             random.randint(MIN_FLUCTUATION, MAX_FLUCTUATION))

            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            stock.update(new_price, new_volume, timestamp)
        self._save_market()

    def _run_market(self):
        # print("market thread started.")
        while self._running:
            try:
                with self.lock:
                    self._update_market()
            except Exception as e:
                print(f"Error updating market: {e}")
            time.sleep(MARKET_UPDATE_INTERVAL)

    def open(self):
        if self._running:
            return
        self._running = True
        self._load_market()
        # print(f"loaded stocks: {list(self.stocks.keys())}")
        threading.Thread(target=self._run_market, daemon=True).start()

    def close(self):
        if not self._running:
            return
        self._running = False
        self._save_market()

    def view(self):
        '''
        Command: view AAPL (or any ticker)
        • Shows:
            • Current price
            • Lastest 5 price
            • Lastest 5 volumes
        '''
        print() # formatting purposes
        for ticker, stock in self.stocks.items():
            print(
                f"[{ticker}]: ${stock.get_current_price() or 0.0:.2f} Vol:{stock.get_latest_volume() or 0}")

            recents = stock.history[-5:]

            price_list = [f"${e['price']:.2f}" for e in recents]
            vol_list = [str(e['volume']) for e in recents]

            print("Last 5 prices: [" + ", ".join(price_list) + "]")
            print("Last 5 volumes: [" + ", ".join(vol_list) + "]\n")
