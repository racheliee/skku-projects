# Project Assignment 2: Table Reservation Application

## Overview
This project involves creating a simple table reservation mobile application. The application allows users to log in, view restaurants, make reservations, and manage their reservations. The user interface must be intuitive and follow the specified guidelines. This project was done as a mini-hackathon, expected to be completed in four hours.

## Objective
- **Implement a Table Reservation App**: Create a mobile application where users can log in, view restaurant details, make reservations, and manage their reservations.
- **UI Requirements**:
  - Use EditText for text input.
  - Display restaurant lists, reservation details, and user reservations using ListView or RecyclerView.

## Key Features

### Functionalities
- **User Authentication**: Validate user ID and password from a provided JSON file.
- **Restaurant Information**: Display a list of restaurants with details such as name, location, rating, and opening hours.
- **Make Reservations**: Allow users to make reservations by selecting a restaurant, entering the number of people, and choosing a date and time.
- **Manage Reservations**: View, edit, and cancel reservations.

### User Interface
- **Login Screen**:
  - Components: Application title, student ID, name, login field, password field, login button.
  - Transition: Start main activity upon successful login.
- **Main Activity**:
  - Components: Reservation button, user info, reservation list.
  - Transition: Start reservation activity when reservation button is pressed.
- **Restaurant List**:
  - Display restaurant details and navigate to detailed view on selection.
- **Restaurant Details**:
  - Show detailed restaurant information and menu.
  - Provide a button to start the reservation process.
- **Reservation Form**:
  - Fields: Number of people, date, and time.
  - Validate inputs and transition to reservation confirmation.
- **Reservation Confirmation**:
  - Show reservation details and provide options to confirm or cancel.
  - Add confirmed reservations to the user's reservation list.

### Specifications
- Evaluated with Pixel 5 API 29.

### Data Handling
- **JSON Files**: Use `user_info.json` and `restaurant_info.json` for user and restaurant data.
- **Image Handling**: Download and include restaurant images in the `res/drawable` folder.

### Grading Criteria
- **Functionality**: Correct implementation of listed features [100 points].
- **Additional Functionality**: Bonus points for extra features [+10 points].
- **Final Score**: Minimum of 100, including additional functionality.
- **Evaluation Environment**: Pixel 5 API 29.
