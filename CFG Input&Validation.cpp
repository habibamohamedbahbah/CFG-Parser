#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

class CFGParser {
private:

    map<char, vector<string>> grammar;
    set<char> definedNonTerminals;
    set<char> usedNonTerminals;

    bool isNonTerminal(char c) {
        return (c >= 'A' && c <= 'Z');
    }

    bool isTerminal(char c) {
        return (c >= 'a' && c <= 'z');
    }

public:

    void inputGrammar() {

        int n;

        cout << "Enter number of productions: ";
        cin >> n;
        cin.ignore();

        cout << "\nEnter productions:\n";
        cout << "Example: S -> aA | b\n\n";

        for (int i = 0; i < n; i++) {

            string line;
            getline(cin, line);

            validateAndStore(line);
        }

        checkUndefinedNonTerminals();
    }

    
    void validateAndStore(string line) {

        
        if (line.find("->") == string::npos) {
            cout << "ERROR: Missing -> in production\n";
            return;
        }

        
        string cleaned = "";

        for (char c : line) {
            if (c != ' ')
                cleaned += c;
        }

        
        int arrowPos = cleaned.find("->");

        
        string lhs = cleaned.substr(0, arrowPos);

        
        if (lhs.length() != 1 || !isNonTerminal(lhs[0])) {

            cout << "ERROR: Invalid Left-Hand Side: "
                 << lhs << endl;

            return;
        }

        char nonTerminal = lhs[0];

        definedNonTerminals.insert(nonTerminal);

        
        string rhs = cleaned.substr(arrowPos + 2);

        
        if (rhs.empty()) {

            cout << "ERROR: Empty production for "
                 << nonTerminal << endl;

            return;
        }

        
        stringstream ss(rhs);

        string production;

        while (getline(ss, production, '|')) {

            
            if (production.empty()) {

                cout << "ERROR: Empty alternative in "
                     << nonTerminal << endl;

                continue;
            }

            
            bool valid = true;

            for (char c : production) {

                if (isNonTerminal(c)) {
                    usedNonTerminals.insert(c);
                }

                else if (isTerminal(c)) {
                    continue;
                }

                else if (c == '#') {
                    
                    continue;
                }

                else {

                    cout << "ERROR: Invalid symbol '"
                         << c << "' in production "
                         << production << endl;

                    valid = false;
                    break;
                }
            }

            if (valid) {

                
                bool duplicate = false;

                for (string s : grammar[nonTerminal]) {

                    if (s == production) {
                        duplicate = true;
                        break;
                    }
                }

                if (!duplicate) {
                    grammar[nonTerminal].push_back(production);
                }
            }
        }
    }

    void checkUndefinedNonTerminals() {

        for (char c : usedNonTerminals) {

            if (definedNonTerminals.find(c)
                == definedNonTerminals.end()) {

                cout << "WARNING: Non-Terminal "
                     << c
                     << " used but not defined.\n";
            }
        }
    }

    
    void printGrammar() {

        cout << "\nVALID CFG \n\n";

        for (auto rule : grammar) {

            cout << rule.first << " -> ";

            for (int i = 0; i < rule.second.size(); i++) {

                cout << rule.second[i];

                if (i != rule.second.size() - 1)
                    cout << " | ";
            }

            cout << endl;
        }
    }
};

int main() {

    CFGParser parser;

    parser.inputGrammar();

    parser.printGrammar();

    return 0;
}