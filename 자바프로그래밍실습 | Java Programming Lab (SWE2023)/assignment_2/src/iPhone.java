import java.util.Scanner;

public class iPhone extends Product {
	String colour;
	int memory;

	// constructor of iPhone class; basePrice is set to 800 dollars.
	public iPhone() {
		basePrice = 800;
	}

	// sets the color of the iPhone
	public void setColour(String colour) {
		this.colour = colour;
	}

	// sets the value of iPhone Memory
	public void setMemory(int memory) {
		this.memory = memory;
	}

	@Override
	// gets the user selections for iphones
	public void getUserSelection() {
		@SuppressWarnings("resource")
		Scanner scanner = new Scanner(System.in);

		System.out.println("Base cost of the iPhone: $800");
		System.out.print("Which model do you want (iPhone12 or iPhone13): ");
		setModel(scanner.nextLine());
		System.out.print("Color (base, Gold, Silver): ");
		setColour(scanner.nextLine());
		System.out.print("Memory (64, 128, 256, 512): ");
		setMemory(scanner.nextInt());
		System.out.print("How many do you want to buy: ");
		setQuantity(scanner.nextInt());
	}

	@Override
	// calculates and returns the cost per iPhone and prints the check as well
	public double calculateCost() {
		totalCost = basePrice;
		System.out.println("========check===========");

		// adds the additional cost that is charged based on the model selected
		if (model.equals("iPhone13")) {
			totalCost += 200;
			System.out.println("Model (iPhone13) ----- +$200");
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
			totalCost += 200;
			System.out.println("Memory (256) ----- +$200");
		} else if (memory == 128) {
			totalCost += 100;
			System.out.println("Memory (128) ----- +$100");
		}

		// prints total cost
		System.out.printf("\nTotal cost for one iPhone: $%.2f\n", totalCost);
		System.out.printf("Total cost (quantity x " + quantity + "): $%.2f\n", totalCost * quantity);
		return totalCost;
	}

}
