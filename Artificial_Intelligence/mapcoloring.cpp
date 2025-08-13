#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Possible colors
vector<string> colors = {"Red", "Green", "Blue"};

// Graph: adjacency list for each region
map<string, vector<string> > neighbors;

bool isConsistent(const string& region, const string& color,
                  const map<string, string>& assignment) {
    vector<string> neighborList = neighbors[region];
    for (size_t i = 0; i < neighborList.size(); ++i) {
        string neighbor = neighborList[i];
        if (assignment.find(neighbor) != assignment.end() && assignment.at(neighbor) == color) {
            return false;
        }
    }
    return true;
}

bool forwardCheck(const string& region, const string& color,
                  map<string, set<string> >& domains,
                  const map<string, string>& assignment) {
    vector<string> neighborList = neighbors[region];
    for (size_t i = 0; i < neighborList.size(); ++i) {
        string neighbor = neighborList[i];
        if (assignment.find(neighbor) == assignment.end()) {
            if (domains[neighbor].count(color)) {
                domains[neighbor].erase(color);
                if (domains[neighbor].empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool backtrack(map<string, string>& assignment,
               map<string, set<string> >& domains,
               const vector<string>& varOrder, int index) {
    if (index == varOrder.size()) return true;

    string region = varOrder[index];
    set<string> domainColors = domains[region];
    for (set<string>::iterator it = domainColors.begin(); it != domainColors.end(); ++it) {
        string color = *it;
        if (isConsistent(region, color, assignment)) {
            // Save domains to restore after backtracking
            map<string, set<string> > savedDomains = domains;

            assignment[region] = color;

            if (forwardCheck(region, color, domains, assignment)) {
                if (backtrack(assignment, domains, varOrder, index + 1)) {
                    return true;
                }
            }

            // Backtrack
            assignment.erase(region);
            domains = savedDomains;
        }
    }
    return false;
}

int main() {
    // Define neighbors
    neighbors["WA"] = vector<string>{"NT", "SA"};
    neighbors["NT"] = vector<string>{"WA", "SA", "Q"};
    neighbors["SA"] = vector<string>{"WA", "NT", "Q", "NSW", "V"};
    neighbors["Q"] = vector<string>{"NT", "SA", "NSW"};
    neighbors["NSW"] = vector<string>{"SA", "Q", "V"};
    neighbors["V"] = vector<string>{"SA", "NSW"};
    neighbors["T"] = vector<string>{};

    // Variable assignment order
    vector<string> varOrder = {"WA", "NT", "SA", "Q", "NSW", "V", "T"};

    // Initialize domains
    map<string, set<string> > domains;
    for (size_t i = 0; i < varOrder.size(); ++i) {
        set<string> domainSet(colors.begin(), colors.end());
        domains[varOrder[i]] = domainSet;
    }

    // Assignment result
    map<string, string> assignment;

    if (backtrack(assignment, domains, varOrder, 0)) {
        cout << "Final Assignment:\n";
        map<string, string>::iterator it;
        for (it = assignment.begin(); it != assignment.end(); ++it) {
            cout << it->first << " = " << it->second << endl;
        }
    } else {
        cout << "No valid assignment found.\n";
    }

    return 0;
}
