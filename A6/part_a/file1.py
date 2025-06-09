from abc import abstractmethod, ABC
import datetime


class StoreChain(ABC):
    def __init__(self, name: str, address: str):
        self.__name = name
        self.__address = address

    @property
    def name(self) -> str:
        return self.__name

    @name.setter
    def name(self, new_name: str) -> None:
        self.__name = new_name

    @property
    def address(self) -> str:
        return self.__address

    @address.setter
    def address(self, new_addr: str) -> None:
        self.__address = new_addr

    @abstractmethod
    def __str__(self) -> str:
        pass


class Store(StoreChain):
    __sid: int
    __telephone: str

    def __init__(self, sid: int, name: str, address: str, telephone: str):
        super().__init__(name, address)
        self.__sid = sid
        self.__telephone = telephone

    @property
    def sid(self) -> int:
        return self.__sid

    @property
    def telephone(self) -> str:
        return self.__telephone

    @sid.setter
    def sid(self, new_sid: int) -> None:
        self.__sid = new_sid

    @telephone.setter
    def telephone(self, new_telephone: str) -> None:
        self.__telephone = new_telephone

    def __str__(self) -> str:
        return f"ID: {self.__sid}, Name: {self.name}, Address: {self.address}, Telephone: {self.__telephone}"


class Citizen(ABC):
    __ssn: str
    __name: str
    __address: str

    def __init__(self, ssn: str, name: str, address: str):
        self.__ssn = ssn
        self.__name = name
        self.__address = address

    @property
    def ssn(self) -> str:
        return self.__ssn

    @property
    def name(self) -> str:
        return self.__name

    @property
    def address(self) -> str:
        return self.__address

    @ssn.setter
    def ssn(self, new_ssn: str) -> None:
        self.__ssn = new_ssn

    @name.setter
    def name(self, new_name: str) -> None:
        self.__name = new_name

    @address.setter
    def address(self, new_address: str) -> None:
        self.__address = new_address

    @abstractmethod
    def __str__(self) -> str:
        pass


class Staff(Citizen):
    __sid: int
    __job_title: str
    __salary: float

    def __init__(self, sid: int, ssn: str, name: str, address: str, job_title: str, salary: float):
        super().__init__(ssn, name, address)
        self.__sid = sid
        self.__job_title = job_title
        self.__salary = salary

    @property
    def sid(self) -> int:
        return self.__sid

    @property
    def job_title(self) -> str:
        return self.__job_title

    @property
    def salary(self) -> float:
        return self.__salary

    @sid.setter
    def sid(self, new_sid: int) -> None:
        self.__sid = new_sid

    @job_title.setter
    def job_title(self, new_job_title: str) -> None:
        self.__job_title = new_job_title

    @salary.setter
    def salary(self, new_salary: float) -> None:
        self.__salary = new_salary

    def __str__(self) -> str:
        return f"ID: {self.__sid}, SSN: {self.ssn}, Name: {self.name}, Address: {self.address}, Job Title: {self.__job_title}, Salary: {self.__salary}"


class Customer(Citizen):
    __cid: int
    __tel: str
    # -	Purchasing_Points is an integer number that represents the points the customer receives when he buys products.
    __purchasing_points: int
    # -	Membership is a list of string that represents the memberships the customer has. These memberships could be good to get some discounts on the purchased products
    __membership: list[str]

    def __init__(self, ssn: str, name: str, address: str, cid: int, tel: str, purchasing_points: int, membership: list[str]):
        super().__init__(ssn, name, address)
        self.__cid = cid
        self.__tel = tel
        self.__purchasing_points = purchasing_points
        self.__membership = membership

    @property
    def cid(self) -> int:
        return self.__cid

    @property
    def purchasing_points(self) -> int:
        return self.__purchasing_points

    @property
    def membership(self) -> list[str]:
        return self.__membership

    @property
    def tel(self) -> str:
        return self.__tel

    @cid.setter
    def cid(self, new_cid: int) -> None:
        self.__cid = new_cid

    @tel.setter
    def tel(self, new_tel: str) -> None:
        self.__tel = new_tel

    @purchasing_points.setter
    def purchasing_points(self, new_points: int) -> None:
        self.__purchasing_points = new_points

    @membership.setter
    def membership(self, new_membership: list[str]) -> None:
        self.__membership = new_membership

    def __str__(self) -> str:
        return f"ID: {self.__cid}, SSN: {self.ssn}, Name: {self.name}, Address: {self.address}, Tel: {self.__tel}, Purchasing Points: {self.__purchasing_points}, Memberships: {', '.join(self.__membership)}"


class Product():
    __code: str
    __name: str
    __description: str
    __price: float
    # -	Point type is an integer that represents the number of points that a customer will receive if they buy that product. When a customer receives a specific number of points, they will receive a gift from the store chain.
    __points: int

    def __init__(self, code: str, name: str, description: str, price: float, points: int):
        self.__code = code
        self.__name = name
        self.__description = description
        self.__price = price
        self.__points = points

    @property
    def code(self) -> str:
        return self.__code

    @property
    def name(self) -> str:
        return self.__name

    @property
    def description(self) -> str:
        return self.__description

    @property
    def price(self) -> float:
        return self.__price

    @property
    def points(self) -> int:
        return self.__points

    @code.setter
    def code(self, new_code: str) -> None:
        self.__code = new_code

    @name.setter
    def name(self, new_name: str) -> None:
        self.__name = new_name

    @description.setter
    def description(self, new_description: str) -> None:
        self.__description = new_description

    @price.setter
    def price(self, new_price: float) -> None:
        self.__price = new_price

    @points.setter
    def points(self, new_points: int) -> None:
        self.__points = new_points

    def __str__(self) -> str:
        return f"Code: {self.__code}, Name: {self.__name}, Description: {self.__description}, Price: {self.__price}, Points: {self.__points}"


class Order():
    __store: Store
    __customer: Customer
    __staff: Staff
    # -	Product_Objects [] is a list of products the customer wants to buy from the store.
    __prod_list: list[Product]
    # -	Quantity is either the total quantity of the order or the list of quantities of all products in the order.
    __quantity: list[int]

    def __init__(self, store: Store, customer: Customer, staff: Staff, prod_list: list[Product], quantity: list[int]):
        self.__store = store
        self.__customer = customer
        self.__staff = staff
        self.__prod_list = prod_list
        self.__quantity = quantity

    @property
    def store(self) -> Store:
        return self.__store

    @property
    def customer(self) -> Customer:
        return self.__customer

    @property
    def staff(self) -> Staff:
        return self.__staff

    @property
    def prod_list(self) -> list[Product]:
        return self.__prod_list

    @property
    def quantity(self) -> list[int]:
        return self.__quantity

    @store.setter
    def store(self, new_store: Store) -> None:
        self.__store = new_store

    @customer.setter
    def customer(self, new_customer: Customer) -> None:
        self.__customer = new_customer

    @staff.setter
    def staff(self, new_staff: Staff) -> None:
        self.__staff = new_staff

    @prod_list.setter
    def prod_list(self, new_prod_list: list[Product]) -> None:
        self.__prod_list = new_prod_list

    @quantity.setter
    def quantity(self, new_quantity: list[int]) -> None:
        self.__quantity = new_quantity

    # -	Implement the addProduct method that will add a product object to the Product_Objects list.
    def addProduct(self, product: Product, qty: int) -> None:
        self.prod_list.append(product)
        self.quantity.append(qty)

    # -	Implement printReceipt method that will print a receipt including store information, staff member name, customer Store ID and name, list of all products he wants to buy with their price, quantity of each product, and the total money and points of the order.
    def printReceipt(self) -> None:
        receipt = f"Welcome to {self.store.name}\n"
        receipt += f"Staff: {self.staff.name}\n"
        receipt += f"RECEIPT\n{datetime.datetime.now().strftime('%Y-%m-%d\n%H:%M:%S')}\n\n"
        receipt += f"Customer ID: {self.customer.cid}\n"
        receipt += f"ST # {self.store.sid}\n\n"

        receipt += f"{'Name':<10} {'Code':<15} {'Price':>5}  {'Qty':>3}\n"
        total_price = 0
        total_points = 0
        for i, product in enumerate(self.prod_list):
            qty = self.quantity[i]
            line_total = product.price * qty
            line_points = product.points * qty
            receipt += f"{product.name.upper():<10} {product.code:<15} {product.price:>5.2f}  {qty}\n"
            total_price += line_total
            total_points += line_points

        receipt += f"\nTOTAL\t\t{total_price:.2f}\n"
        receipt += f"# ITEMS SOLD {sum(self.quantity)}\n\n"
        receipt += f"Total Points: {total_points}\n\n"
        receipt += "***CUSTOMER COPY***\n"

        print(receipt)


def main():
    store = Store(1, "Kim's Convenience", "Seobu-reo 2066", "010-0000-0000")
    print("=== Store Info ===")
    print(store, end="\n\n")

    # customers
    c1 = Customer("2022310853", "Timothee Chalamet",
                  "Seobu-reo 2066", 11111, "010-1111-1111", 3, ["GOLD"])
    c2 = Customer("2021310853", "Kylie Jenner", "Seobu-reo 2066",
                  22222, "010-2222-2222", 5, ["PLATINUM"])

    print("=== Customers ===")
    print(c1)
    print(c2, end="\n\n")

    # staff
    s1 = Staff(10101, "999-88-7777", "Kim Kardashian",
               "Seobu-reo 2066", "Cashier", 4000)
    s2 = Staff(20202, "888-77-6666", "Khloe Kardashian",
               "Seobu-reo 2066", "Manager", 5000)

    print("=== Staff Members ===")
    print(s1)
    print(s2, end="\n\n")

    while True:
        try:
            chosen_cid = int(
                input("Choose a customer ID (11111 or 22222): "))
            if chosen_cid not in [11111, 22222]:
                raise ValueError
            break
        except ValueError:
            print("Not a recognized customer ID. Please try again.")

    # order
    order = Order(store, c1 if chosen_cid == 202506101 else c2, s1, [], [])

    print("\n=== Creating Order ===\nEnter products to add to the order (type 'done' to finish):")
    while True:
        name = input("Product Name: ")
        if name.lower() == "done":
            break
        code = input("Product Code: ")
        desc = input("Description: ")
        try:
            price = float(input("Price: "))
            points = int(input("Points: "))
            qty = int(input("Quantity: "))
        except ValueError:
            print(
                "Invalid input. Please enter numeric values for price, points, and quantity.\n")
            continue

        product = Product(code, name, desc, price, points)
        order.addProduct(product, qty)
        print(f"Added {qty} x {name} to the order.\n")

    print("\n=== FINAL RECEIPT ===")
    order.printReceipt()


if __name__ == "__main__":
    main()
