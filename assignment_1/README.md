# Assignment 1: Java Programming

## Overview
This project involves creating three Java programs to perform different tasks: calculating compound interest for a bank account, simulating a savings plan, and generating a test system for elementary arithmetic practice. Each program is designed to handle user input, perform calculations, and display the results.

## Objective
- **Implement Three Java Programs**: Develop separate Java applications to solve the given tasks.
- **Task 1: SKKU Bank**:
  - Calculate the final amount of money in a bank account based on user input for initial balance, interest rate, and number of years.
- **Task 2: Frank's Savings**:
  - Simulate a daily savings plan where the amount saved increases weekly.
- **Task 3: Test System**:
  - Generate random addition questions for elementary students and provide feedback based on their answers.

## Task Description

### Task 1: SKKU Bank
- **User Input**: Collect user’s name, initial balance, annual interest rate, and number of years.
- **Calculation**: Compute the balance at the end of each year.
- **Output**: Display the balance progression for each year.

### Task 2: Frank's Savings
- **Savings Simulation**: 
  - Start with a deposit of $1 on the first Monday and increase the deposit amount by $1 every subsequent Monday.
- **Daily Deposits**: Continue depositing $1 each day throughout the week.
- **Output**: Calculate and display the total savings after a given number of days.

### Task 3: Test System
- **Random Question Generation**: Use a `Random` object to generate five addition questions with positive integers.
- **User Interaction**: Prompt the user to answer each question.
- **Feedback**: Check answers and provide feedback based on the number of correct answers:
  - **5 correct answers**: Excellent!
  - **4 correct answers**: Very good!
  - **3 correct answers**: Good.
  - **2 correct answers**: Not Bad.
  - **1 correct answer**: Very bad.
  - **0 correct answers**: Try again.
