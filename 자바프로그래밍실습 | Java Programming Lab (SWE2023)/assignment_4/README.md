# Assignment 4: Lucas Series Calculation

## Overview
This project involves creating a Java application to calculate the Lucas series, a sequence similar to the Fibonacci series but with different starting values. The program will display each row of the series, calculate the overall sum using multithreading, and handle file input/output operations. The user interface should be intuitive and robust against improper inputs.

## Objective
- **Implement a Lucas Series Calculator**: Develop a Java application to compute and display the Lucas series.
- **UI Requirements**:
  - Create a user-friendly GUI.
  - Provide options to calculate and cancel operations.

## Key Features

### Functionalities
- **Lucas Series Calculation**: 
  - Display each row of the Lucas series separately.
  - Calculate the sum of the Lucas series using another thread (SwingWorker).
  - Save the Lucas series to a text file.

- **User Interaction**:
  - **Calculate Button**: Starts the calculation of the Lucas series and the sum.
  - **Cancel Button**: Stops the calculation and displays the sum calculated so far. This button is disabled before starting the calculation and after the calculation ends.

- **Exception Handling**:
  - Handle wrong inputs such as strings, too big integers, and floats by showing an error dialog box.

- **File I/O**:
  - Write the Lucas series to a text file.

### Specifications
- **Program Requirements**:
  - **GUI**: Create an intuitive and aesthetically pleasing interface.
  - **Exception Handling**: Implement robust exception handling.
  - **File I/O**: Save the Lucas series to a file.
  - **Multithreading**: Use SwingWorker for background calculations.
  - **Good Coding Style**: Use meaningful variable names, modular functions, and include meaningful comments.
  - **Recorded Video**: Demonstrate the program’s functionality, covering all cases.

### Grading Criteria
- **Correctness**: Code should run without errors and meet all requirements.
- **Comments**: Include meaningful comments for important lines of code.
- **Coding Style**: Use meaningful names for variables and methods, and ensure code is modular.
- **GUI Design**: Create an aesthetically pleasing and user-friendly interface.
- **Exception Handling**: Handle all possible exceptions appropriately.
- **File I/O**: Correctly implement file input/output operations.
- **Multithreading**: Use SwingWorker for efficient background processing.