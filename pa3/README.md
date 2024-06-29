# System Programming Lab PA3: Reservation Server

## Grade
82.64/100

## Objective
&nbsp; &nbsp; The goal of this project is to design and implement a reservation server that supports many clients.
- Server manages the reservation procedure for each client.
- Client manages a user’s seat by sending a query, containing the user’s desired action, to the server. (note that the terms "client" and "user" are different)

&nbsp; &nbsp; This project implements a client-server system for booking seats with multiple actions such as login, booking, confirming booking, canceling booking, and logout. The server uses a multi-threaded approach to handle multiple clients concurrently.

## File Structure
The project consists of two main components: 
1. the client `pa3_client.c`
2. the server `pa3_server.c`
Each component includes several functions to handle specific tasks.

## Pre-requisites
Install the `libargon2` library to hash passwords using Argon2id.
```bash
sudo apt install libargon2-dev
```  

## Definitions
- A user associated with a client is called an active user.
- A client associated with a user is an active client.
- User can only use one client at a time.
- Client can only be used by one user at a time.
- Total of 256 seats are available for booking. (seat numbers 1-256)


## How to compile and run
In the `pa3` directory, run the following commands:
```bash
make
```

Run the server with the following command:
```bash
./bin/pa3_server <port>
```

Run the client with the following command:
```bash
./bin/pa3_client <host> <port> <input_file>
```
note: input file is optional

To clean the compiled files, run the following command in the `pa3` directory:
```bash
make clean
```

## Actions
Total of 6 actions are supported by the server:
- Actions are entered in format of `<user_id> <action> <data_field>`

| Action ID | Action Name | Description | Data Field |
| --- | --- | --- | --- |
| 0 | TERMINATION | Terminates the connection between the client and the server | 0 |
| 1 | LOGIN | Logs in the user with the given user ID and password | Password |
| 2 | BOOK | Books a seat for the user | Seat Number |
| 3 | CONFIRM | Confirms the booking of the user | 0 or N/A |
| 4 | CANCEL | Cancels the booking of the user | seat number |
| 5 | LOGOUT | Logs out the user | N/A |