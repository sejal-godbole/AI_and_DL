#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

// ----------------------
// Knowledge Base
// ----------------------
map<string, vector<string>> father;
map<string, vector<string>> mother;

// ----------------------
// Add Facts
// ----------------------
void addFather(string f, string c) {
    father[f].push_back(c);
}

void addMother(string m, string c) {
    mother[m].push_back(c);
}

// ----------------------
// Rules
// ----------------------

// Parent Rule
vector<string> getChildren(string p) {
    vector<string> result;

    for (string child : father[p])
        result.push_back(child);

    for (string child : mother[p])
        result.push_back(child);

    return result;
}

// Sibling Rule
vector<string> getSiblings(string name) {
    vector<string> result;

    // Check in father map
    for (auto &entry : father) {
        vector<string> children = entry.second;
        bool found = false;

        for (string c : children)
            if (c == name) found = true;

        if (found) {
            for (string c : children)
                if (c != name) result.push_back(c);
        }
    }

    // Check in mother map
    for (auto &entry : mother) {
        vector<string> children = entry.second;
        bool found = false;

        for (string c : children)
            if (c == name) found = true;

        if (found) {
            for (string c : children)
                if (c != name) result.push_back(c);
        }
    }

    return result;
}

// Grandparent Rule
vector<string> getGrandchildren(string gp) {
    vector<string> result;

    for (string child : father[gp]) {
        vector<string> gkids = getChildren(child);
        result.insert(result.end(), gkids.begin(), gkids.end());
    }

    for (string child : mother[gp]) {
        vector<string> gkids = getChildren(child);
        result.insert(result.end(), gkids.begin(), gkids.end());
    }

    return result;
}

// ----------------------
// Main Program
// ----------------------
int main() {
    // FACTS
    addFather("Mohan", "Ram");
    addFather("Mohan", "Shyam");
    addFather("Mohan", "Gita");
    addMother("Sita", "Ram");
    addMother("Sita", "Shyam");
    addMother("Sita", "Gita");
    addFather("Shyam", "Anu");
    addFather("Shyam", "Raju");

    cout << "Family Tree Knowledge Base Parser\n";
    cout << "----------------------------------\n";
    cout << "Queries Supported:\n";
    cout << "1. children <name>\n";
    cout << "2. siblings <name>\n";
    cout << "3. grandchildren <name>\n";
    cout << "Type 'exit' to quit.\n\n";

    string query, name;

    while (true) {
        cout << "Enter query: ";
        cin >> query;

        if (query == "exit") break;
        cin >> name;

        if (query == "children") {
            vector<string> res = getChildren(name);
            if (res.empty())
                cout << name << " has no children.\n";
            else {
                cout << "Children of " << name << ": ";
                for (string c : res) cout << c << " ";
                cout << endl;
            }
        }
        else if (query == "siblings") {
            vector<string> res = getSiblings(name);
            if (res.empty())
                cout << name << " has no siblings.\n";
            else {
                cout << "Siblings of " << name << ": ";
                for (string s : res) cout << s << " ";
                cout << endl;
            }
        }
        else if (query == "grandchildren") {
            vector<string> res = getGrandchildren(name);
            if (res.empty())
                cout << name << " has no grandchildren.\n";
            else {
                cout << "Grandchildren of " << name << ": ";
                for (string g : res) cout << g << " ";
                cout << endl;
            }
        }
        else {
            cout << "Unknown query type!\n";
        }
    }

    return 0;
}
