def Store():
    __sid: int
    __name: str
    __address: str
    __telephone: str
    
    def __init__(self, sid: int, name: str, address: str, telephone: str):
        self.__sid = sid
        self.__name = name
        self.__address = address
        self.__telephone = telephone
        
    # todo: properties, setter, object __str__
    
    
def Staff():
    __id: int
    __ssn: str
    __name: str
    __address: str
    __job_title: str
    __salary: float
    
    def __init__(self, id: int, ssn: str, name: str, address: str, job_title: str, salary: float):
        self.__id = id
        self.__ssn = ssn
        self.__name = name
        self.__address = address
        self.__job_title = job_title
        self.__salary = salary
        
    # todo: properties, setter, object __str__
    
    
def Customer():
    __ssn: str
    __name: str
    __address: str
    # -	Purchasing_Points is an integer number that represents the points the customer receives when he buys products.
    __purchasing_points: int
    # -	Membership is a list of string that represents the memberships the customer has. These memberships could be good to get some discounts on the purchased products 
    __membership: list[str]

    def __init__(self, ssn: str, name: str, address: str, purchasing_points: int, membership: list[str]):
        self.__ssn = ssn
        self.__name = name
        self.__address = address
        self.__purchasing_points = purchasing_points
        self.__membership = membership
        
def Product():
    __code: str
    __name: str
    __description: str
    __price: float
    __points: int
    
    def __init__(self, code: str, name: str, description: str, price: float, points: int):
        self.__code = code
        self.__name = name
        self.__description = description
        self.__price = price
        self.__points = points
        
    # -	Point type is an integer that represents the number of points that a customer will receive if they buy that product. When a customer receives a specific number of points, they will receive a gift from the store chain. 
    
    
def Order():
    store: Store
    customer: Customer
    staff: Staff
    prod_list: list[Product]
    quantity: list[int]
    
    def __init__(self, store: Store, customer: Customer, staff: Staff, prod_list: list[Product], quantity: list[int]):
        self.store = store
        self.customer = customer
        self.staff = staff
        self.prod_list = prod_list
        self.quantity = quantity
    
    # -	Product_Objects [] is a list of products the customer wants to buy from the store.
    # -	Quantity is either the total quantity of the order or the list of quantities of all products in the order. 
    # -	Implement the addProduct method that will add a product object to the Product_Objects list.
    # -	Implement printReceipt method that will print a receipt including store information, staff member name, customer Store ID and name, list of all products he wants to buy with their price, quantity of each product, and the total money and points of the order.   

    