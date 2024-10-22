import java.util.Scanner;

public class Main {

	public static void main(String[] args) {
		Scanner scanner = new Scanner(System.in);
		int repeatProcess = 1;

		// do-while loop runs while user would like to purchase a product
		do {
			System.out.println("==== Welcom Apple Korea ====");
			System.out.println("What do you want to buy?");
			System.out.println("Choose 1 for MacBook");
			System.out.println("Choose 2 for iPhone");
			System.out.println("Choose 3 for AirPods");
			System.out.println("Choose 0 for Exit");
			System.out.print("Please enter your choice: ");

			// item stores the product the user would like to buy
			int item = scanner.nextInt();
			if (item == 0)
				break;

			System.out.println("=================================");

			switch (item) {
			case 1:
				MacBook mac = new MacBook();
				mac.getUserSelection();
				mac.calculateCost();
				break;

			case 2:
				iPhone iphone = new iPhone();
				iphone.getUserSelection();
				iphone.calculateCost();
				break;

			case 3:
				AirPods airpods = new AirPods();
				airpods.getUserSelection();
				airpods.calculateCost();
				break;
			}

			// checks if the user wants to buy another item
			System.out.print("Do you want to purchase something else (Yes: 1, No: 0): ");

			repeatProcess = scanner.nextInt();
			if (repeatProcess == 0)
				break;

		} while (repeatProcess != 0);

		System.out.println("Thank you for your purchase!");
		scanner.close();
	}

}
