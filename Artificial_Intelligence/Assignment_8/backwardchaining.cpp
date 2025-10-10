#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Rule structure: IF antecedents => THEN consequent
struct Rule {
    vector<string> antecedents;
    string consequent;
};

// Function to build an index for quick lookup of rules by their consequent
unordered_map<string, vector<Rule>> index_rules(const vector<Rule>& rules) {
    unordered_map<string, vector<Rule>> idx;
    for (const auto& r : rules) {
        idx[r.consequent].push_back(r);
    }
    return idx;
}

// The core backward_prove function
bool backward_prove(const string& goal,
                    unordered_set<string>& facts,
                    const unordered_map<string, vector<Rule>>& rules_by_cons,
                    unordered_set<string>& proven,
                    unordered_set<string>& visiting,
                    vector<string>& trace) {
    // 1) If the goal is already a known fact, it's proved.
    if (facts.count(goal)) {
        trace.push_back("KNOWN FACT: " + goal);
        proven.insert(goal);
        return true;
    }

    // 2) If the goal has been proved before, reuse the result.
    if (proven.count(goal)) {
        trace.push_back("ALREADY PROVED: " + goal);
        return true;
    }

    // 3) Cycle detection: if the goal is already being visited, there's a loop.
    if (visiting.count(goal)) {
        trace.push_back("CYCLE DETECTED: " + goal);
        return false;
    }

    // 4) Find rules that can produce the goal.
    auto it = rules_by_cons.find(goal);
    if (it == rules_by_cons.end()) {
        trace.push_back("NO RULES PRODUCE: " + goal);
        return false;
    }

    visiting.insert(goal);
    trace.push_back("TRY PROVING: " + goal);

    // Try each rule that can produce the goal
    for (const auto& rule : it->second) {
        // Log the rule being tried
        string s = "Try rule: ";
        for (size_t i = 0; i < rule.antecedents.size(); ++i) {
            if (i > 0) s += " ∧ ";
            s += rule.antecedents[i];
        }
        s += " ⇒ " + rule.consequent;
        trace.push_back(s);

        bool rule_ok = true;
        // Recursively prove each antecedent of the rule
        for (const auto& ant : rule.antecedents) {
            bool ant_proved = backward_prove(ant, facts, rules_by_cons, proven, visiting, trace);
            if (!ant_proved) {
                trace.push_back(" -> antecedent FAILED: " + ant);
                rule_ok = false;
                break;
            } else {
                trace.push_back(" -> antecedent PROVED: " + ant);
            }
        }

        if (rule_ok) {
            // All antecedents were proved, so the goal is proved by this rule.
            facts.insert(goal);
            proven.insert(goal);
            trace.push_back("RULE SUCCEEDED -> proved: " + goal);
            visiting.erase(goal);
            return true;
        } else {
            trace.push_back("Rule failed for: " + goal);
        }
    }

    // All candidate rules failed to prove the goal.
    visiting.erase(goal);
    trace.push_back("ALL RULES FAILED for: " + goal);
    return false;
}

int main() {
    // Define the knowledge base rules
    vector<Rule> rules = {
        {{"Fever", "Cough"}, "ViralInfection"},
        {{"ViralInfection", "BodyAche"}, "FluSymptoms"},
        {{"FluSymptoms"}, "Flu"}
    };

    // Initial facts (the patient's symptoms)
    unordered_set<string> facts = {"Fever", "Cough", "BodyAche"};

    // Index the rules for efficient backward search
    auto rules_by_cons = index_rules(rules);

    // The query (goal)
    string query = "Flu";
    vector<string> trace;
    unordered_set<string> proven;
    unordered_set<string> visiting;

    // Start the backward chaining process
    bool result = backward_prove(query, facts, rules_by_cons, proven, visiting, trace);

    // Print the reasoning trace
    cout << "Backward Chaining Trace:\n";
    for (const auto& line : trace) {
        cout << line << "\n";
    }

    // Print the final result
    cout << "\nResult: " << (result ? "PROVED" : "NOT PROVED") << " for goal: " << query << "\n";
    cout << "Final facts: ";
    for (const auto& f : facts) {
        cout << f << " ";
    }
    cout << "\n";

    return 0;
}