from dataclasses import dataclass
from typing import Dict, Self


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
