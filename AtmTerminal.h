#include <string>
#include <vector>

using std::string;
using std::vector;

class Client;

class AtmTerminal
{
public:
    AtmTerminal(const string& pathToDatabase);
    ~AtmTerminal();
    void start();
private:
    void loadDB(const string& pathToDatabase);
    void authorization();
    int chooseOption();
    void withdrawCash();
    void depositCash();
    void showBalance();
    void modifyPINcode();
    void transferBetweenAccounts();
    void quit();
    void printHeader();
    int verifiedNumCard(int numCard);
    bool verifiedPin(int index, string pin);
    string promptClientAnswer();
    float userInputPosFloat(string msg);
    int userInputPosInt(string msg);
private:
    const int COMMISSION_OVER_20000_KZT = 500;
    const int COMMISSION_TO_ANOTHER_BANK = 200;
    vector<Client*> clients;
    string pathToDatabase;
    int indexClient;
};


