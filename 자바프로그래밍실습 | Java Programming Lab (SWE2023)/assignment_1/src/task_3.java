import java.util.Random;   //import the Random library
import java.util.Scanner;  // import the Scanner class

public class task_3 {
	public static void main(String[] args) {
		
		int answer; //stores the answer the student gave
		int randInt1; //stores the first random integer given in the problem
		int randInt2; //stores the second random integer given in the problem
		int numCorrect = 0; //stores the number of problems answered correctly
		
		//creates random and scanner  
		Random randGenerator = new Random();
		Scanner scanner = new Scanner(System.in);
		
		//iterate to print out five addition problems
		for(int i = 1; i <= 5; i++) {
			//generates two random numbers between 0 and 49
			randInt1 = randGenerator.nextInt(50);
			randInt2 = randGenerator.nextInt(50);
			System.out.print("Question " + i + " - Calculate the addition (" + randInt1 + 
					" + " + randInt2 + "): ");
			answer = scanner.nextInt();

			//check if the student answered correctly, 
			//and count the number of correctly answered questions
			if((randInt1+randInt2) == answer)
				numCorrect++;
		}
		scanner.close();		
		
		//print final output
		System.out.println("Number of correct answers: " + numCorrect);
		printFeedback(numCorrect);
	}
	
	/**
	 * this function prints the feedback based on the correctly answered numbers
	 * @param numCorrect is the number of questions the student answered correctly
	 */
	public static void printFeedback(int numCorrect) {
		if(numCorrect == 0) 
			System.out.print("Try again.");
		else if(numCorrect == 1)
			System.out.print("Very bad.");
		else if(numCorrect == 2)
			System.out.print("Not bad.");
		else if(numCorrect == 3)
			System.out.print("Good.");
		else if(numCorrect == 4)
			System.out.print("Very good.");
		else
			System.out.print("Excellent!");
		
	}
}
