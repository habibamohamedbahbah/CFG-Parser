#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct CYKEntry {
    string         symbol;
    vector<string> production;
    int            split;
};

struct DerivationNode {
    string                  symbol;
    int                     i, j;
    vector<DerivationNode*> children;

    DerivationNode(const string& s, int i, int j)
        : symbol(s), i(i), j(j) {}

    ~DerivationNode() {
        for (auto* c : children) delete c;
    }
};

using CYKTable = vector<vector<vector<CYKEntry>>>;

const CYKEntry* searchCYKCell(const CYKTable& table, const string& symbol, int i, int j)
{
    if (i >= (int)table.size() || j >= (int)table[i].size())
        return nullptr;
    for (const auto& entry : table[i][j])
        if (entry.symbol == symbol)
            return &entry;
    return nullptr;
}

DerivationNode* buildDerivationTree(const CYKTable& table, const string& str, const string& symbol, int i, int j)
{
    DerivationNode* node = new DerivationNode(symbol, i, j);
    const CYKEntry* entry = searchCYKCell(table, symbol, i, j);
    if (!entry) return node;

    if (entry->production.size() == 1) {
        node->children.push_back(new DerivationNode(entry->production[0], i, j));
        return node;
    }

    if (entry->production.size() == 2 && entry->split != -1) {
        node->children.push_back(buildDerivationTree(table, str, entry->production[0], i, entry->split - 1));
        node->children.push_back(buildDerivationTree(table, str, entry->production[1], entry->split, j));
    }

    return node;
}

int findLeftmostSymbol(const vector<string>& sententialForm, const string& symbol)
{
    for (int i = 0; i < (int)sententialForm.size(); i++)
        if (sententialForm[i] == symbol) return i;
    return -1;
}

string joinSymbols(const vector<string>& form)
{
    string result;
    for (int i = 0; i < (int)form.size(); i++) {
        if (i > 0) result += " ";
        result += form[i];
    }
    return result;
}

void traverseAndDerive(DerivationNode* node, vector<string>& sententialForm, vector<string>& steps)
{
    if (node->children.empty()) return;

    int idx = findLeftmostSymbol(sententialForm, node->symbol);
    if (idx == -1) return;

    vector<string> childSymbols;
    for (auto* child : node->children)
        childSymbols.push_back(child->symbol);

    string beforeStep = joinSymbols(sententialForm);

    vector<string> newForm;
    for (int i = 0; i < idx; i++)
        newForm.push_back(sententialForm[i]);
    for (const auto& s : childSymbols)
        newForm.push_back(s);
    for (int i = idx + 1; i < (int)sententialForm.size(); i++)
        newForm.push_back(sententialForm[i]);

    steps.push_back(beforeStep + "  =>  " + joinSymbols(newForm));
    sententialForm = newForm;

    for (auto* child : node->children)
        traverseAndDerive(child, sententialForm, steps);
}

vector<string> getDerivationSteps(DerivationNode* derivationTree, const string& startSymbol)
{
    vector<string> steps;
    vector<string> sententialForm = { startSymbol };
    traverseAndDerive(derivationTree, sententialForm, steps);
    return steps;
}

void printDerivationSteps(const vector<string>& steps, const string& inputString, const string& startSymbol, bool accepted)
{
    cout << "\n" << string(55, '=') << "\n";
    cout << "         Derivation Steps\n";
    cout << string(55, '=') << "\n";

    if (!accepted) {
        cout << "\n  [X]  String '" << inputString << "' rejected\n\n";
        cout << string(55, '=') << "\n\n";
        return;
    }

    if (steps.empty()) {
        cout << "\n  No steps found\n\n";
        cout << string(55, '=') << "\n\n";
        return;
    }

    cout << "\n  Start: " << startSymbol << "\n\n";
    for (int i = 0; i < (int)steps.size(); i++)
        cout << "  Step " << (i + 1) << ": " << steps[i] << "\n";

    cout << "\n" << string(55, '-') << "\n";
    cout << "\n  [OK]  " << startSymbol << "  =>*  " << inputString << "\n";
    cout << "\n  Result: String Accepted\n";
    cout << string(55, '=') << "\n\n";
}

struct DerivationOutput {
    vector<string>  steps;
    DerivationNode* derivationTree;
};

DerivationOutput computeDerivation(const CYKTable& cykTable, const string& inputString, const string& startSymbol, bool accepted)
{
    DerivationOutput output;
    output.derivationTree = nullptr;

    if (!accepted) {
        printDerivationSteps({}, inputString, startSymbol, false);
        return output;
    }

    int n = (int)inputString.size();
    DerivationNode* tree = buildDerivationTree(cykTable, inputString, startSymbol, 0, n - 1);
    vector<string> steps = getDerivationSteps(tree, startSymbol);
    printDerivationSteps(steps, inputString, startSymbol, true);

    output.steps          = steps;
    output.derivationTree = tree;
    return output;
}
