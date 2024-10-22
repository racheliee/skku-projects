import java.util.Scanner;

public class MacBook extends Product {
	String colour;
	int memory;
	int size;

	// constructor of MacBook class. Sets the basePrice to 1000 dollars.
	public MacBook() {
		basePrice = 1000;
	}
	
	// sets the colour of the MacBook
	public void setColour(String colour) {
		this.colour = colour;
	}

	// sets the memory value of MacBook
	public void setMemory(int memory) {
		this.memory = memory;
	}

	// sets the size of MacBook
	public void setSize(int size) {
		this.size = size;
	}

	@Override
	// gets the user selections for MacBook
	public void getUserSelection() {
		@SuppressWarnings("resource")
		Scanner scanner = new Scanner(System.in);

		System.out.println("Base cost of the MacBook: $1000");
		System.out.print("Which model do you want (Pro or Air): ");
		setModel(scanner.next());
		System.out.print("Size (13, 14, 16): ");
		setSize(scanner.nextInt());
		System.out.print("Color (base, Gold, Silver): ");
		setColour(scanner.next());
		System.out.print("Memory (128, 256, 512): ");
		setMemory(scanner.nextInt());
		System.out.print("How many do you want to buy: ");
		setQuantity(scanner.nextInt());
	}

	@Override
	// calculates and returns the cost per MacBook and prints the check as well
	public double calculateCost() {
		totalCost = basePrice;
		System.out.println("========check===========");

		// adds the additional cost that is charged based on the model selected
		if (model.equals("Pro")) {
			totalCost += 200;
			System.out.println("Model (Pro) ------ +$200");
		}

		// adds the additional cost that is charged based on size selected
		if (size == 16) {
			totalCost += 200;
			System.out.println("Size (16) ------ +$200");
		} else if (size == 14) {
			totalCost += 100;
			System.out.println("Size (14) ------ +$100");
		}

		// adds the additional cost that is charged based on the colour selected
		if (colour.equals("Gold")) {
			totalCost += 20;
			System.out.println("Color (Gold) ------ +$20");
		} else if (colour.equals("Silver")) {
			totalCost += 10;
			System.out.println("Color (Silver) ------ +$10");
		}

		// adds the additional cost that is charged based on memory selected
		if (memory == 512) {
			totalCost += 300;
			System.out.println("Memory (512) ----- +$300");
		} else if (memory == 256) {
			totalCost += 100;
			System.out.println("Memory (256) ----- +$100");
		}

		// prints total cost
		System.out.printf("\nTotal cost for one MacBook: $%.2f\n", totalCost);
		System.out.printf("Total cost (quantity x " + quantity + "): $%.2f\n", totalCost * quantity);

		return totalCost;
	}

}
