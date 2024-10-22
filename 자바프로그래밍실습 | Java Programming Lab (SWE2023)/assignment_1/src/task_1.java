import java.util.Scanner;  // import the Scanner library

public class task_1 {

	public static void main(String[] args) {

		String name; //name of the bank user
		double initBalance; //stores the initial balance of the specified user
		double percentage; //stores the annual interest rate in percentage
		int numOfYears; //stores the number of years the user has stored their money
		double finalBalance; //total amount of money the user will have after interest
		
		//create scanner to take user input
	    Scanner scanner = new Scanner(System.in);
	    
	    System.out.println("Please, enter following information: ");
	    //asking and storing user's name
	    System.out.print("Your name: ");
	    name = scanner.nextLine();
	    
	    //asking and storing initial balance
	    System.out.print("Initial Balance: ");
	    initBalance = scanner.nextDouble();
	    
	    //asking and storing bank's annual interest
	    System.out.print("Percentage: ");
	    percentage = scanner.nextDouble();
	    
	    //asking and storing number of years the user has stored their money
	    System.out.print("Number of years: ");
	    numOfYears = scanner.nextInt();
	    scanner.close();
	    
	    finalBalance = calculateFinalBalance(initBalance, percentage, numOfYears);
	    
	    //printing final output (finalBalance is printed out to two decimal points)
	    System.out.println("================================");
	    System.out.println("User name: " + name);
	    System.out.println("Percentage: " + percentage);
	    System.out.println("Your balance (after " + numOfYears + " years): " + String.format("%.2f", finalBalance));
	}
	
	/**
	 * This function calculates the final deposit the user will have after
	 * a certain amount of years receiving a some percentage of annual interest 
	 * @param initBalance	initial balance the user has
	 * @param percentage	the amount of annual interest received
	 * @param numOfYears	the number of years that are to receive annual interest
	 * @return	final balance the user will have
	 */
	public static double calculateFinalBalance(double initBalance, double percentage, int numOfYears) {
		double finalBalance = initBalance; //stores the final amount of money returned
		percentage /= 100; //dividing percentage by 100 to convert it into a decimal
		
		//this loop iterates to get the balance increase per each year
		for(int i = 0; i < numOfYears; i++) {
			finalBalance += finalBalance * percentage;
		}
		
		return finalBalance;
	}

}
