# ATM Terminal Simulation

## Project Overview
This project simulates an ATM terminal, allowing users to perform various banking operations such as withdrawing and depositing cash, transferring funds, modifying PIN codes, and checking account balances. The application is implemented in C++ and reads client data from a database file.

## Features
- **Authorization:** Secure login using a card number and PIN code.
- **Withdraw Cash:** Allows users to withdraw an amount from their account, ensuring balance sufficiency.
- **Deposit Cash:** Enables users to deposit money into their account.
- **Modify PIN Code:** Provides functionality to activate cards and modify PIN codes securely.
- **Show Balance:** Displays account balance and the most recent transaction.
- **Transfer Between Accounts:** Supports fund transfers between accounts, with appropriate commission calculations.
- **End Session:** Saves changes to the database and terminates the session.

## File Structure
- `AtmTerminal.h`: Header file for the `AtmTerminal` class.
- `client.h`: Header file for the `Client` class.
- `main.cpp`: Main program file to initialize and run the ATM simulation.
- `database.csv`: A sample database containing client information (Bank ID, Name, Card Number, PIN, Balance, Last Transaction).

## How to Use
1. **Compile the Project:**
   Compile the program using a C++ compiler, such as g++.
   ```bash
   g++ -o AtmTerminal main.cpp AtmTerminal.cpp client.cpp
   ```

2. **Run the Program:**
   Execute the compiled program.
   ```bash
   ./AtmTerminal
   ```

3. **Database File:**
   Ensure the `database.csv` file is present in the directory. This file contains client data that the application reads at runtime.

4. **Interactive Menu:**
   The program displays an interactive menu for performing various operations after successful authorization.

## Database Format
The database file should follow this structure:
```csv
Bank ID,Name,Card Number,PIN,Balance,Last Transaction
1,John Doe,123456,1234,5000.00,0.00
2,Jane Smith,234567,5678,7000.00,0.00
```
- **Bank ID:** Unique identifier for the bank.
- **Name:** Client's name.
- **Card Number:** Six-digit card number.
- **PIN:** Four-digit PIN code.
- **Balance:** Current account balance.
- **Last Transaction:** Amount of the last transaction.

## Functions and Classes
### `AtmTerminal` Class
- **Constructor:** Initializes the ATM with the database file.
- **Destructor:** Cleans up dynamically allocated memory.
- **start():** Main loop to handle user interactions.
- **authorization():** Handles user login with card number and PIN.
- **chooseOption():** Displays the main menu and handles option selection.
- **withdrawCash(), depositCash(), showBalance(), modifyPINcode(), transferBetweenAccounts():** Core functionalities for ATM operations.
- **quit():** Saves changes to the database and exits.

### `Client` Class
- **Attributes:**
  - `Bank ID`
  - `Name`
  - `Card Number`
  - `PIN`
  - `Balance`
  - `Last Transaction`
- **Methods:** Getters and setters for client attributes.

## Example Usage
1. Launch the program.
2. Enter the card number and PIN to log in.
3. Choose an option from the menu (e.g., withdraw cash, deposit cash, etc.).
4. Perform desired transactions.
5. End the session to save changes.

## Error Handling
- Validates card number and PIN during login.
- Ensures sufficient balance for withdrawals and transfers.
- Handles invalid inputs (e.g., non-numeric values).

## Future Enhancements
- Add support for multi-language interfaces.
- Integrate with a real-time banking server.
- Add biometric authentication for enhanced security.
- Improve database security by encrypting sensitive data.

