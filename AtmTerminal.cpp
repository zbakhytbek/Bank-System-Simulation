#include "AtmTerminal.h"
#include "client.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

AtmTerminal::AtmTerminal(const std::string& pathToDatabase)
{
    this->pathToDatabase = pathToDatabase;
    loadDB(pathToDatabase);
}

AtmTerminal::~AtmTerminal()
{
    for (auto item : clients) {
        delete item;
    }
}

void AtmTerminal::start()
{
    while (true) {
        system("cls");
        authorization();
        int command = -1;

        while (command != 6) {
            system("cls");
            command = chooseOption();

            system("cls");
            switch (command) {
            case 1:
                withdrawCash();
                break;
            case 2:
                depositCash();
                break;
            case 3:
                modifyPINcode();
                break;
            case 4:
                showBalance();
                break;
            case 5:
                transferBetweenAccounts();
                break;
            case 6:
                quit();
                break;
            default:
                break;
            }
        }
    }
}

void AtmTerminal::loadDB(const std::string& pathToDatabase)
{
    ifstream in(pathToDatabase);
    if (!in.is_open()) {
        return;
    }

    string line;
    getline(in, line);

    while (getline(in, line)) {
        istringstream iss(line);
        int indCell = 1;
        Client* client = new Client();
        string cell;
        while (getline(iss, cell, ',')) {
            switch (indCell) {
            case 1:
                client->setBankId(stoi(cell));
                break;
            case 2:
                client->setName(cell);
                break;
            case 3:
                client->setCardNumber(stoi(cell));
                break;
            case 4:
                client->setPin(cell);
                break;
            case 5:
                client->setBalance(stof(cell));
                break;
            case 6:
                client->setLastTransaction(stof(cell));
                break;
            default:
                break;
            }
            ++indCell;
        }
        clients.push_back(client);
    }
}

void AtmTerminal::authorization()
{
    printHeader();

    while (true) {
        cout << "Card number(6 digits): ";
        int numCard;
        cin >> numCard; cin.get();

        int idx = verifiedNumCard(numCard);

        if (idx == -1) {
            cout << "The card number or PIN code is incorrect. Try again!" << endl;
            continue;
        }

        cout << "PIN code(4 digits): ";

        string pin;
        getline(cin, pin);

        bool isDone = verifiedPin(idx, pin);

        if (!isDone) {
            cout << "The card number or PIN code is incorrect. Try again!" << endl;
            continue;
        }
        indexClient = idx;

        break;
    }
}

int AtmTerminal::chooseOption()
{
    printHeader();

    cout << "Select one of the following options:\n";

    cout << "1. Withdraw Cash\t\t2. Deposit Cash" << endl;
    cout << "3. Modify PIN Code\t\t4. Show Balance" << endl;
    cout << "5. Transfer Between Accounts\t6. End Session" << endl << endl;

    cout << "Enter option number: ";

    int option;
    cin >> option; cin.get();

    return option;
}

void AtmTerminal::withdrawCash()
{
    printHeader();

    Client* client = clients[indexClient];
    
    if (!client->isActivated()) {
        cout << "Firstly you need activated card" << endl;
        cin.get();
        return;
    }

    string clientAnswer;
    string answer = "n";
    cout.precision(2);
    do {
        float balance = client->getBalance();
        cout << "Current balance: " << fixed << balance << " KZT" << endl << endl;

        float withdraw;
        cout << "Enter amount to withdraw: ";
        cin >> withdraw; cin.get();

        if (withdraw > balance) {
            cout << "Withdraw a lesser amount";
            return;
        }

        client->setBalance(balance - withdraw);
        client->setLastTransaction(-withdraw);

        cout << "Cash withdrawn successfully!" << endl;
        cout << "Remaining balance: " << client->getBalance() << " KZT" << endl << endl;

        clientAnswer = promptClientAnswer();
        
    } while (clientAnswer != "Y" && clientAnswer != "y");
}

void AtmTerminal::depositCash()
{
    printHeader();

    Client* client = clients[indexClient];
    string clientAnswer;
    cout.precision(2);
    do {
        float balance = client->getBalance();
        cout << "Current balance: " << fixed <<balance << " KZT" << endl << endl;

        float deposit;
        cout << "Enter amount to deposit: ";
        cin >> deposit; cin.get();

        client->setBalance(balance + deposit);
        client->setLastTransaction(deposit);

        cout << "Cash deposit successfully!" << endl;
        cout << "Remaining balance: " << fixed << client->getBalance() << " KZT" << endl << endl;

        clientAnswer = promptClientAnswer();

    } while (clientAnswer != "Y" && clientAnswer != "y");
}

void AtmTerminal::showBalance()
{
    printHeader();

    string clientAnswer;
    cout.precision(2);
    do {
        cout << "Card owner: " << clients[indexClient]->getName() << endl;
        cout << "Card number: " << clients[indexClient]->getCardNumber() << endl;
        cout << "Current balance in KZT: " << fixed << clients[indexClient]->getBalance() << endl;
        cout << "Current balance in USD: " << fixed << clients[indexClient]->getBalance() / 450 << endl;
        cout << "Last transaction: " << fixed << clients[indexClient]->getLastTransaction() << " KZT" << endl << endl;

        clientAnswer = promptClientAnswer();

    } while (clientAnswer != "Y" && clientAnswer != "y");
}

void AtmTerminal::modifyPINcode()
{
    printHeader();

    Client* client = clients[indexClient];
    string clientAnswer;

    do {
        string pin;
        if (!client->isActivated()) {
            cout << "Enter new PIN code for actived(4 digits): ";
            getline(cin, pin);

            bool isDoneNewPin = true;
            for (auto val : pin) {
                if (val < '0' || val > '9') {
                    cout << "Wrong PIN code. Try again!" << endl;
                    isDoneNewPin = false;
                    break;
                }
            }

            if (isDoneNewPin) {
                client->setActived(true);
                client->setPin(pin);
            }
        }
        else {
            cout << "PIN code(4 digits): ";
            getline(cin, pin);

            if (verifiedPin(indexClient, pin)) {
                cout << "Wrong PIN code. Try again!" << endl;
                continue;
            }

            cout << "Enter new PIN code(4 digits): ";
            getline(cin, pin);

            bool isDoneNewPin = true;

            for (auto val : pin) {
                if (val < '0' || val > '9') {
                    cout << "Wrong PIN code. Try again!" << endl;
                    isDoneNewPin = false;
                    break;
                }
            }

            if (isDoneNewPin) {
                client->setPin(pin);
                cout << "Successfully modify PIN code" << endl;
            }
        }
        
        clientAnswer = promptClientAnswer();

    } while (clientAnswer != "Y" && clientAnswer != "y");
}

void AtmTerminal::transferBetweenAccounts()
{
    if (!clients[indexClient]->isActivated()) {
        cout << "Firstly you need activated card" << endl;
        cin.get();
        return;
    }

    string clientAnswer;
    printHeader();

    cout << "Commission for fund transfers over 20000 KZT is " << fixed << COMMISSION_OVER_20000_KZT << " KZT" << endl << endl;

    do {
        int toCardNumber = userInputPosInt("Beneficiary's card number: ");
        int idx = verifiedNumCard(toCardNumber);

        if (idx == -1) {
            cout << "The card number is incorrect. Try again!" << endl;
            continue;
        }

        float transferAmount = userInputPosFloat("Transfer amount(in KZT): ");
        float total = transferAmount;

        if (transferAmount > 20000) {
            total += COMMISSION_OVER_20000_KZT;
        }

        if (clients[indexClient]->getBankId() != clients[idx]->getBankId()) {
            total += COMMISSION_TO_ANOTHER_BANK;
        }

        if (total > clients[indexClient]->getBalance()) {
            cout << "Transfer a lesser amount. Try again!";
            continue;
        }

        cout << "Beneficiary's name: " << clients[idx]->getName() << endl;
        cout << "Conform transfer?(Y/N):";

        getline(cin, clientAnswer);

        if (clientAnswer == "Y" || clientAnswer == "y") {
            clients[indexClient]->setBalance(clients[indexClient]->getBalance() - total);
            clients[indexClient]->setLastTransaction(-total);

            clients[idx]->setBalance(clients[idx]->getBalance() + transferAmount);
            clients[idx]->setLastTransaction(transferAmount);
            
            cout << endl << "Transfer successful!" << endl << endl;
        }

        clientAnswer = promptClientAnswer();

    } while (clientAnswer != "Y" && clientAnswer != "y");
}

void AtmTerminal::quit()
{
    printHeader();

    cout << endl << "Process save changes into file..." << endl;
    ofstream out(pathToDatabase);

    out << "Bank ID,Name,Card Number,PIN,Balance,Last Transaction" << endl;

    for (auto client : clients) {
        out << fixed << setprecision(2) << client->getBankId() << "," << client->getName() << "," << client->getCardNumber() << "," <<
            client->getPin() << "," << client->getBalance() << "," << client->getLastTransaction() << endl;
    }

    cout << "Good bye!!!" << endl; cin.get();
}

void AtmTerminal::printHeader()
{
    cout << "   Welcome to the Bank of <Your Team Name>\n" << endl;
}

int AtmTerminal::verifiedNumCard(int numCard)
{
    int i = 0;
    for (auto item : clients) {
        if (item->getCardNumber() == numCard) {
            return i;
        }
        ++i;
    }

    return -1;
}

bool AtmTerminal::verifiedPin(int index, string pin)
{
    return clients[index]->getPin() == pin;
}

string AtmTerminal::promptClientAnswer()
{
    string clientAnswer;

    while (true) {
        cout << "Would you like to return to the main menu?(Y/N): ";
        getline(cin, clientAnswer);
        if (clientAnswer == "Y" || clientAnswer == "y" || clientAnswer == "N" || clientAnswer == "n")
            return clientAnswer;
        else
            cout << "Wrong answer. Try again!" << endl << endl;
    }
}

float AtmTerminal::userInputPosFloat(string msg)
{
    string value;
    while (true) {
        cout << msg;
        getline(cin, value);
        if (value.find_first_not_of("1234567890.") != string::npos) {
            cout << "Uncorrect number. Try again" << endl;
            continue;
        }

        return stof(value);
    }
}

int AtmTerminal::userInputPosInt(string msg)
{
    string value;
    while (true) {
        cout << msg;
        getline(cin, value);
        if (value.find_first_not_of("1234567890") != string::npos) {
            cout << "Uncorrect number. Try again" << endl;
            continue;
        }

        return stoi(value);
    }
}
