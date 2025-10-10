#include <iostream>
#include <vector>

using namespace std;

// Function to print the chessboard
void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int cell : row) {
            cout << (cell == 1 ? "Q " : ". ");
        }
        cout << endl;
    }
}

// Function to check if a position is safe for a queen
bool isSafe(const vector<vector<int>>& board, int row, int col, int N) {
    // Check this row on the left side
    for (int i = 0; i < col; ++i) {
        if (board[row][i] == 1) {
            return false;
        }
    }

    // Check upper diagonal on the left side
    for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {
        if (board[i][j] == 1) {
            return false;
        }
    }

    // Check lower diagonal on the left side
    for (int i = row, j = col; i < N && j >= 0; ++i, --j) {
        if (board[i][j] == 1) {
            return false;
        }
    }

    return true;
}

// Recursive function to solve the 8-Queens problem
bool solveNQueensUtil(vector<vector<int>>& board, int col, int N) {
    // Base case: if all queens are placed, return true
    if (col >= N) {
        return true;
    }

    // Consider this column and try placing this queen in all rows
    for (int i = 0; i < N; ++i) {
        if (isSafe(board, i, col, N)) {
            // Place the queen
            board[i][col] = 1;

            // Recur to place the rest of the queens
            if (solveNQueensUtil(board, col + 1, N)) {
                return true;
            }

            // If placing queen here doesn't lead to a solution, then backtrack
            board[i][col] = 0; // Backtrack
        }
    }

    // If the queen cannot be placed in any row in this column, return false
    return false;
}

int main() {
    int N = 8;
    vector<vector<int>> board(N, vector<int>(N, 0));

    if (solveNQueensUtil(board, 0, N)) {
        cout << "Solution found:\n";
        printBoard(board);
    } else {
        cout << "Solution does not exist.\n";
    }

    return 0;
}