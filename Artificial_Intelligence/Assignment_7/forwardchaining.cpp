#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Rule structure: IF antecedents => THEN consequent
struct Rule {
    vector<string> antecedents;
    string consequent;
    bool used = false; // to prevent reusing a rule once it has fired
};

// Function to check if all antecedents of a rule are in the facts list
bool canFire(const Rule& rule, const vector<string>& facts) {
    for (const auto& ant : rule.antecedents) {
        bool found = false;
        for (const auto& f : facts) {
            if (f == ant) {
                found = true;
                break;
            }
        }
        if (!found) return false; // one antecedent is missing
    }
    return true; // all antecedents are present
}

int main() {
    // Define the rules for the knowledge base
    vector<Rule> rules = {
        {{"P", "Q"}, "R"},     // Rule 1: P and Q => R
        {{"R", "S"}, "T"},     // Rule 2: R and S => T
        {{"T"}, "U"}           // Rule 3: T => U
    };

    // Initial facts (the starting data)
    vector<string> facts = {"P", "Q", "S"};

    // The query we want to prove
    string query = "U";
    bool derived = false;

    cout << "Initial facts: ";
    for (const auto& f : facts) {
        cout << f << " ";
    }
    cout << "\n";

    // Forward chaining loop
    bool addedNew;
    do {
        addedNew = false;
        for (auto& rule : rules) {
            // Check if the rule hasn't been used and can be fired
            if (!rule.used && canFire(rule, facts)) {
                // Fire the rule
                cout << "Rule fired: ";
                for (const auto& a : rule.antecedents) {
                    cout << a << " ";
                }
                cout << "=> " << rule.consequent << "\n";
                
                // Add the new fact to the facts list
                facts.push_back(rule.consequent);
                rule.used = true;
                addedNew = true;

                // Check if the newly added fact is the query
                if (rule.consequent == query) {
                    derived = true;
                    break;
                }
            }
        }
    } while (addedNew && !derived);

    cout << "\nFinal facts: ";
    for (const auto& f : facts) {
        cout << f << " ";
    }
    cout << "\n";

    // Output the result
    if (derived) {
        cout << "SUCCESS: Query " << query << " is derived!\n";
    } else {
        cout << "FAIL: Query " << query << " cannot be derived.\n";
    }

    return 0;
}