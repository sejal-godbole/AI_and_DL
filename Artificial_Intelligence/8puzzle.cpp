#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
using namespace std;

string goal = "123456780"; // Goal state where '0' is the blank space

// Function to check if the current state is the goal state
bool isGoal(const string& state) {
    return state == goal;
}

// Function to generate all valid neighbors by moving the blank
vector<string> getNeighbors(const string& state) {
    vector<string> neighbors;
    int pos = state.find('0'); // index of blank
    int row = pos / 3;
    int col = pos % 3;

    // Directions: up, down, left, right
    vector<pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (size_t i = 0; i < directions.size(); ++i) {
        int dr = directions[i].first;
        int dc = directions[i].second;
        int newRow = row + dr;
        int newCol = col + dc;

        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
            int newPos = newRow * 3 + newCol;
            string newState = state;
            swap(newState[pos], newState[newPos]); // move the blank
            neighbors.push_back(newState);
        }
    }

    return neighbors;
}

// Breadth-First Search to solve the puzzle
void solvePuzzle(string start) {
    queue<pair<string, vector<string>>> q; // current state + path
    unordered_set<string> visited;

    q.push(make_pair(start, vector<string>(1, start)));
    visited.insert(start);

    while (!q.empty()) {
        pair<string, vector<string>> front = q.front();
        q.pop();

        string currState = front.first;
        vector<string> path = front.second;

        if (isGoal(currState)) {
            cout << "Solution found in " << path.size() - 1 << " moves:\n";
            for (size_t i = 0; i < path.size(); ++i) {
                string s = path[i];
                for (int j = 0; j < 9; ++j) {
                    cout << s[j] << " ";
                    if (j % 3 == 2) cout << endl;
                }
                cout << "---\n";
            }
            return;
        }

        vector<string> neighbors = getNeighbors(currState);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            string neighbor = neighbors[i];
            if (visited.find(neighbor) == visited.end()) {
                vector<string> newPath = path;
                newPath.push_back(neighbor);
                q.push(make_pair(neighbor, newPath));
                visited.insert(neighbor);
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    string start;
    cout << "Enter initial state as a 9-digit string (0 for blank): ";
    cin >> start;

    if (start.size() != 9) {
        cout << "Invalid input!" << endl;
        return 1;
    }

    solvePuzzle(start);
    return 0;
}
