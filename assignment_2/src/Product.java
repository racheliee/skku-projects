
public abstract class Product {
	double basePrice;
	double totalCost;
	String model;
	int quantity;

	// sets model of the product
	public void setModel(String model) {
		this.model = model;
	}

	// sets quantity of the product
	public void setQuantity(int quantity) {
		this.quantity = quantity;
	}

	// methods are defined abstract since they are to be defined differently for each product
	public abstract void getUserSelection(); // accepts the selections the user made

	public abstract double calculateCost(); // calculates the total cost and prints the check of the product bought
}
