
struct Node {
    string val;
    vector<Node*> children;

    Node(string v) : val(v) {}
};

Node* buildTreeExample() {
    Node* S = new Node("S");
    Node* A = new Node("A");
    Node* B = new Node("B");

    Node* a = new Node("a");
    Node* b = new Node("b");

    S->children = {A, B};
    A->children = {a};
    B->children = {b};

    return S;
}

void printTree(Node* root, int level = 0) {
    if (!root) return;

    for (int i = 0; i < level; i++)
        cout << "  ";

    cout << root->val << endl;

    for (auto c : root->children)
        printTree(c, level + 1);
}
int main() {
    int choice;
    string input;

    while (true) {
        cout << "\n===== COMPILER TOOL =====\n";
        cout << "1. Add Production\n";
        cout << "2. Print Grammar\n";
        cout << "3. CYK Check String\n";
        cout << "4. Derivation Example\n";
        cout << "5. Show Parse Tree\n";
        cout << "6. Exit\n";
        cout << "Enter: ";
        cin >> choice;

        if (choice == 1) {
            string lhs, rhs;
            cout << "LHS: "; cin >> lhs;
            cout << "RHS: "; cin >> rhs;
            addProduction(lhs, rhs);
        }

        else if (choice == 2) {
            printGrammar();
        }

        else if (choice == 3) {
            cout << "Input string: ";
            cin >> input;

            if (CYK("S", input))
                cout << "ACCEPTED\n";
            else
                cout << "REJECTED\n";
        }

        else if (choice == 4) {
            derivationExample();
        }

        else if (choice == 5) {
            Node* root = buildTreeExample();
            printTree(root);
        }

        else if (choice == 6) {
            break;
        }
    }

    return 0;
}