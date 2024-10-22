# Assignment 3: SKKU Application Form

## Overview
This project involves creating a robust Java application form program for new student admissions at SKKU. The program must handle various input fields, validate user inputs, and provide feedback on incorrect entries. The user interface should be aesthetically pleasing and user-friendly.

## Objective
- **Implement an Application Form Program**: Develop a Java application to collect and validate student application data.
- **UI Requirements**:
  - Create a beautiful and user-friendly GUI.
  - Include all necessary input fields and error handling mechanisms.

## Key Features

### Functionalities
- **User Input Fields**: 
  - First Name
  - Last Name
  - Birthdate (MM/DD/YYYY)
  - Email
  - Phone Number
  - Previous University (for graduates)
  - GPA (for graduates, between 0 and 4.5)
  - Personal Statement (minimum 100 words)

- **Validation and Error Handling**:
  - **Name Validation**: Ensure both first and last names are provided.
  - **Empty Fields**: Prompt the user to fill any empty required fields.
  - **Birthdate Format**: Validate the birthdate format as 'MM/DD/YYYY'.
  - **Email Format**: Ensure the email contains '@' and a domain.
  - **Phone Number Format**: Validate the phone number format to avoid characters or symbols.
  - **Graduate Fields**: Ensure previous university and GPA are provided for graduate applicants.
  - **GPA Range**: Validate that GPA is between 0 and 4.5.
  - **Personal Statement**: Ensure the personal statement is at least 100 words.

- **Feedback Messages**: Display popup messages for each validation error, listing all issues found.

- **Success Confirmation**: Clear the form and show a success popup once all inputs are valid and the form is submitted.

### Specifications
- **Program Requirements**:
  - Implement a beautiful GUI using Java Swing or similar.
  - Add at least two extra fields (e.g., phone number, experience).
  - Handle all possible exceptions with strong exception handling.
  - Follow good coding practices with meaningful variable names and modular functions.
  - Include meaningful comments for code clarity.

### Grading Criteria
- **Correctness**: Code should run without errors and meet all requirements.
- **Comments**: Include meaningful comments for important lines of code.
- **Coding Style**: Use meaningful names for variables and methods, and ensure code is modular.
- **Exception Handling**: Robust handling of all potential exceptions.
- **GUI Design**: Create an aesthetically pleasing and user-friendly interface.
