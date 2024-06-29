import java.util.Scanner;

public class AirPods extends Product {
	// constructor of AirPods class. Sets the basePrice to 120 dollars.
	public AirPods() {
		basePrice = 120;
	}

	@Override
	// gets the user selections for airpods
	public void getUserSelection() {
		@SuppressWarnings("resource")
		Scanner scanner = new Scanner(System.in);

		System.out.println("Base cost of the AirPods: $120");
		System.out.print("Which model do you want (AirPods3 or AirPodsPro or AirPodsMax): ");
		setModel(scanner.nextLine());
		System.out.print("How many do you want to buy: ");
		setQuantity(scanner.nextInt());
	}

	@Override
	// calculates and returns the cost per Airpods and prints the check as well
	public double calculateCost() {
		totalCost = basePrice;

		System.out.println("========check===========");

		// returns the additional cost that is charged based on the model selected
		if (model.equals("AirPodsPro")) {
			totalCost += 100;
			System.out.println("Model (AirPodsPro) ------ +$100");
		} else if (model.equals("AirPodsMax")) {
			totalCost += 200;
			System.out.println("Model (AirPodsMax) ------ +$200");
		}
		
		// prints total cost
		System.out.printf("\nTotal cost for one AirPods: $%.2f\n", totalCost);
		System.out.printf("Total cost (quantity x " + quantity + "): $%.2f\n", totalCost * quantity);
		return totalCost;
	}
}
