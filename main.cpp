// Libarries included
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <sstream>


using namespace std;

// Global definitions and variables
map<char, std::pair<string, char> > Maptype; // map to put the data
#define MAX_TOKEN_SIZE 512
#define KEY_COUNT 5 //count of key
#define SPEC_COUNT 9 //count of special characters
#define MAX_SYMBOLS 128 //This is maximum symbols that the program can handle. Increase if needed
 //Max token size

class Symbols
{
private:
    //list of key
    static const string key[];

    //list of special characters
    static const char specialCharacter[];

    // declaring enum to identify the string
    enum node
    {
        KEYWORD,
        IDENTIFIER,
        INTEGER,
        REAL,
        SPECIAL
    };

    // declaring a struct to have public members
    struct sym
    {
        node ptr; // head of the file
        string token; //token
        int count; //items count

    };
    //list of all symbols
    sym symbolTable[MAX_SYMBOLS];

    int symCount;

     //This holds first character of next token but not entire token
    char nxtToken;

    // to handle error
    string err;

public:
    Symbols()
    {
        symCount = 0;
        err = "";
        nxtToken = 0;
    }
    bool parser(char* exe)
    {
        // using if stream to open the file

        ifstream myfile;

        // string to hold line
        string str;
        myfile.open("test.txt");

        if (myfile.bad())
            {
            err = "Cannot open the file";
            myfile.close();
            return false;
        }
        nxtToken = myfile.get();

        // Read through the end of file

        while (!myfile.eof())
        {
            //Read the each char and according to its  type

            readFile(str, myfile);

            if (getKeyword(str))
            {
                getSymbol(str, KEYWORD);
            }

            else if (getInteger(str))
                {
                if (nxtToken == '.')
                {

                    // to hold the string

                    string line;
                    nxtToken = myfile.get();
                    readFile(line, myfile);
                    str = str + "." + line;
                    getSymbol(str, REAL);

                }
                else
                {
                    // getSymbol(string, Value);
                     getSymbol(str, INTEGER);
                }

            }
            else if (getSpecialChar(str))
            {

                getSymbol(str, SPECIAL);
            }

            else if (getIdentifier(str))
            {

                getSymbol(str, IDENTIFIER);
            }

            else
                {
                // if the file has other token than the assignment suggests

                err = "Token not present. Error \"" + str + "\"";
                return false;

            }

        }
        myfile.close();
        return true;
    }
    //  used to get the error message

    string getError()
    {

        return err;
    }

    // read from the file
    void readFile(string& str , ifstream& myfile)
    {
        // declaring a integer to increment the counter
        int i = 0;

        // defining scan string and all the delimeters
        char scanstr[] = "\n\t ()[]+-=,;.\0";
        char spacestr[] = "\n\t ";
        char specialstr[] = "()[]+-=,;";
        char token[MAX_TOKEN_SIZE];

        //Special Characters being handled

        if (strchr(specialstr, nxtToken))
            {
            token[i++] = nxtToken;
            nxtToken = myfile.get();
            while (strchr(spacestr, nxtToken) && myfile.good())
            {
                nxtToken = myfile.get();
            }
            token[i] = '\0';
            str = string(token);
            return;
        }
        // loop which handle all tokens except special characters

        while (myfile.good())
        {
            if (strchr(scanstr, nxtToken))
            {
                while (strchr(spacestr, nxtToken) && myfile.good())
                {
                    nxtToken = myfile.get();
                }
                token[i] = '\0';
                str = string(token);
                return;
            }
            else
                token[i++] = nxtToken;
            nxtToken = myfile.get();
        }
    }

    // compare the keywoard and return boolean

    bool getKeyword(string& str)
    {
        for (int i = 0; i < KEY_COUNT; i++)
            if (!str.compare(key[i]))
                return true;
        return false;
    }

    // compare wether it is and interger

    bool getInteger(string& str)
    {
        for (unsigned int i = 0; i < str.length(); i++)
            if (str.at(i) < '0' || str.at(i) > '9')
            {
                 return false;
            }

        return true;
    }

    // compare whether it is an string charache and return boolean value

    bool getIdentifier(string& str)
    {
        for (unsigned int i = 0; i < str.length(); i++)
            if (!((str.at(i) >= 'a' && str.at(i) <= 'z') ||
                (str.at(i) >= 'A' && str.at(i) <= 'Z')))
                {
                   return false;
                }

        return true;
    }

    // compares special charachter and returns boolan values

    bool getSpecialChar(string& str)
    {
        if (str.length() != 1)

            return false;
        char sc = str.at(0);
        for (int i = 0; i < SPEC_COUNT; i++)

            if (sc == specialCharacter[i])
                return true;
        return false;
    }

    // function to form the table

    void getSymbol(string& total, node ptr)
    {
        for (int i = 0; i < symCount; i++)
            if (!total.compare(symbolTable[i].token))
                {
                symbolTable[i].count++;
                return;
            }
        symbolTable[symCount].token = total;
        symbolTable[symCount].count = 1;
        symbolTable[symCount++].ptr = ptr;
        return;
    }

    // to print the token list

    void printTokenList()
    {
        cout << "----------------------------------------" << endl;
        cout << "SYMBOL \t Occurence \t TOKENTYPE\n";
        cout << "----------------------------------------" << endl;
        for (int i = 0; i < symCount; i++)
        {
            cout << symbolTable[i].token << "\t  " << symbolTable[i].count << "\t";
            if (symbolTable[i].ptr == INTEGER)
            {
                cout << "\tINTEGER  " << endl;
            }

            else if (symbolTable[i].ptr == REAL)
                {
                     cout << "\tREAL" << endl;
                }

            else if (symbolTable[i].ptr == IDENTIFIER)
                {
                    cout << "\tIDENTIFIER" << endl;
                }

            else if (symbolTable[i].ptr == KEYWORD)
                {
                    cout << "\tKEYWORD" << endl;
                }

            else
            {
                cout << "\tSPECIAL" << endl;
            }


        }
        cout << "----------------------------------------" << endl;
        cout << "----------------------------------------" << endl;
    }
};

int main(int argc, char* argv[])
{

    Maptype['+'] = std::make_pair("SPECIAL", '+');
//    m['+'] = std::make_pair("SPECIAL", '+');
//
//    std::pair<string>, char> val = m[123];

    Symbols parser;
    if (!parser.parser(argv[1]))
        {
        cout << "Parser returned err: " << parser.getError();
        system(0);
    }
    parser.printTokenList();
    return 0;
}

// Initializing global class variable

const string Symbols::key[] = { "if", "then", "else", "begin", "end" };
const char Symbols::specialCharacter[] = { '(', ')', '[', ']', '+', '-', '=', ',', ';' };
