# Midterm Project: Trip Planner Program

## Overview
This midterm project involves creating a Java program called Trip Planner to help students plan their trips. The application will manage various transportation methods, calculate the trip's overall cost, and handle trip cancellations. The user interface should be intuitive and incorporate advanced Java programming concepts.

## Objective
- **Implement a Trip Planner Program**: Develop a Java application that allows users to plan their trips using different transportation methods and calculates the total fare.
- **Key Requirements**:
  - Use Java Swing for the GUI.
  - Implement inheritance, polymorphism, and composition.
  - Create and use a `Payable` interface and a `PublicTransport` abstract class with subclasses for different transportation methods.

## Key Features

### Functionalities
- **Transportation Methods**:
  - **Taxi**: Calculate fare based on base fare and distance.
  - **Bus**: Calculate fare based on the number of stations.
  - **Train**: Calculate fare similar to the bus.
- **Trip Planning**:
  - Allow users to input details for multiple transportation segments.
  - Enable trip cancellation with a message and option to create a new plan.
  - Calculate and display the total fare and trip details if the trip is completed without cancellation.

### Classes
- **Payable Interface**: Define methods related to fare calculation.
- **PublicTransport Abstract Class**: Base class for different transportation methods.
  - Subclasses: `Taxi`, `Bus`, `Train`.
- **Station Class**: Manage information about transportation stations.

### Program Flow
- **Start Trip**: Display options to choose transportation methods and input relevant details.
- **Calculate Fare**: Compute the fare for each segment based on the chosen transportation method.
- **Cancel Trip**: Provide an option to cancel the trip at any point, resetting the plan.
- **Complete Trip**: Print the total fare and detailed trip description upon completion.

### Specifications
- **Program Requirements**:
  - Implement a user-friendly GUI using Java Swing.
  - Apply inheritance, polymorphism, and composition principles.
  - Use a list to manage transportation objects.
  - Ensure the program runs without errors and follows good coding practices.
  - Include meaningful comments for important lines of code.

### Grading Criteria
- **Correctness**: Code should run without errors and meet all requirements.
- **Comments**: Include meaningful comments for important lines of code.
- **Coding Style**: Use meaningful names for variables and methods, and ensure code is modular.
- **GUI Design**: Create an aesthetically pleasing and user-friendly interface.
- **Inheritance and Polymorphism**: Properly implement and use these concepts.
- **File I/O**: Save and load trip data as necessary.
