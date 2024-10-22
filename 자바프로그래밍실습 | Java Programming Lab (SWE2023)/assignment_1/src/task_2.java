import java.util.Scanner;  // import the Scanner library

public class task_2 {
	public static void main(String[] args) {
		int numOfDays; //stores the number of days that Frank will be saving money
		int totalSavings; //stores the total amount frank has saved
		
		//asking the how many days Frank will be saving using a scanner
		Scanner scanner = new Scanner(System.in);
		System.out.print("Enter the number of days: ");
		numOfDays = scanner.nextInt();
		scanner.close();
		
		//calculate the total amount of savings that Frank has made and print final output
		totalSavings = calculateTotalSavings(numOfDays);
		System.out.println("Total Frank's saving (after "+ numOfDays + " days): " + totalSavings);
		
	}
	
	/**
	 * this function calculates the total amount of savings Frank will have
	 * after a certain number of days when he saves one more dollar than the previous week
	 * @param numOfDays is the number of days that Frank will be saving
	 * @return returns the total amount of savings Frank has made 
	 */
	public static int calculateTotalSavings(int numOfDays) {
		int totalSavings = 0; //stores the total amount frank has saved
		
		//iterates to reflect the change in savings as each day passes
		for(int i = 0; i < numOfDays; i++) {
			totalSavings += i/7 + 1;
		}
		
		return totalSavings;
		
	}
	
}
