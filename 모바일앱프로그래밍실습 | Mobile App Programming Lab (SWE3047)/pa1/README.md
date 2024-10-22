# Project Assignment 1: Simple Wordle

## Overview
This project involves creating a mobile application version of the popular game Wordle. The application will allow users to guess a five-letter English word and provide feedback on the accuracy of their guesses. The user interface should be intuitive and follow the specified guidelines.

## Objective
- **Implement a Wordle Game**: Create a mobile application where users can guess a five-letter word and receive feedback.
- **UI Requirements**: 
  - Use EditText for text input.
  - Display the guess list and color-coded feedback.
  - Use ListView or RecyclerView for displaying guesses and feedback.

## Key Features

### Functionalities
- **Generate Random Answer**: On app execution, generate a random five-letter word from a given dictionary.
- **User Input Validation**: Ensure that the user can only guess valid words from the dictionary.
- **Feedback Display**: Show feedback for each guess:
  - **Green**: Correct letter in the correct position.
  - **Yellow**: Correct letter in the wrong position.
  - **Gray**: Incorrect letter.

### Specifications
- **Project Settings**:
  - Minimum SDK: 29 (Android 10.0)
  - Target & Compile SDK: 34 (Android 14.0)
  - Application ID: `edu.skku.cs.pa1`
- **Application Execution**:
  - Must show the Wordle screen without any action.
  - Start within 10 seconds and UI must not stop for more than 5 seconds.
  - No errors during build/execution.

