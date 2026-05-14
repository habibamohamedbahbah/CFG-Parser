#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

int main()
{
    int productions;

    cout << "Enter number of productions: ";
    cin >> productions;
    cin.ignore();

    map<string, vector<string>> grammar;

    cout << "\nEnter productions in CNF format\n";


    for (int i = 0; i < productions; i++)
    {
        string left, arrow, right;

        cin >> left >> arrow >> right;

        grammar[left].push_back(right);
    }

    string input;

    cout << "\nEnter input string: ";
    cin >> input;

    int n = input.length();

    // CYK Table
    vector<vector<set<string>>> table(n, vector<set<string>>(n));

    // Step 1: Fill first row using terminals
    for (int i = 0; i < n; i++)
    {
        string terminal(1, input[i]);

        for (auto rule : grammar)
        {
            string variable = rule.first;

            for (string production : rule.second)
            {
                if (production == terminal)
                {
                    table[i][i].insert(variable);
                }
            }
        }
    }

    // Step 2: Fill remaining table
    for (int length = 2; length <= n; length++)
    {
        for (int i = 0; i <= n - length; i++)
        {
            int j = i + length - 1;

            for (int k = i; k < j; k++)
            {
                set<string> leftSet = table[i][k];
                set<string> rightSet = table[k + 1][j];

                for (string B : leftSet)
                {
                    for (string C : rightSet)
                    {
                        string combination = B + C;

                        for (auto rule : grammar)
                        {
                            string variable = rule.first;

                            for (string production : rule.second)
                            {
                                if (production == combination)
                                {
                                    table[i][j].insert(variable);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Print CYK Table
    cout << "\nCYK Table:\n\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j < i)
            {
                cout << " \t";
                continue;
            }

            cout << "{";

            for (string value : table[i][j])
            {
                cout << value;
            }

            cout << "}\t";
        }

        cout << endl;
    }
// Final Result
string startSymbol;
cout << "\nEnter start symbol: ";
cin >> startSymbol;

if (table[0][n - 1].count(startSymbol))
{
    cout << "\nString is ACCEPTED by the grammar.\n";
}
else
{
    cout << "\nString is REJECTED by the grammar.\n";
}
}