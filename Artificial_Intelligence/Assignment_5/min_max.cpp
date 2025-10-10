#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

// Constants for players
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = '.';
const int BOARD_SIZE = 3;

// Function prototypes
void print_board(const vector<vector<char>>& board);
int evaluate_board(const vector<vector<char>>& board);
bool is_game_over(const vector<vector<char>>& board);
int minimax(vector<vector<char>>& board, int depth, bool is_maximizing);
pair<int, int> find_best_move(vector<vector<char>>& board);
void play_game();

/**
 * @brief Prints the current state of the Tic-Tac-Toe board.
 */
void print_board(const vector<vector<char>>& board) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            cout << board[i][j] << (j == BOARD_SIZE - 1 ? "" : " ");
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * @brief Evaluates the board and returns a score.
 * @return 10 if 'X' (Max player) wins, -10 if 'O' (Min player) wins, 0 otherwise.
 */
int evaluate_board(const vector<vector<char>>& board) {
    // Check rows and columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        // Check row i
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) {
            return (board[i][0] == PLAYER_X) ? 10 : -10;
        }
        // Check column i
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY) {
            return (board[0][i] == PLAYER_X) ? 10 : -10;
        }
    }

    // Check main diagonal (top-left to bottom-right)
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) {
        return (board[0][0] == PLAYER_X) ? 10 : -10;
    }

    // Check anti-diagonal (top-right to bottom-left)
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY) {
        return (board[0][2] == PLAYER_X) ? 10 : -10;
    }

    return 0; // Draw or game still ongoing
}

/**
 * @brief Checks if the game is over (win, loss, or draw).
 */
bool is_game_over(const vector<vector<char>>& board) {
    int score = evaluate_board(board);
    if (score != 0) {
        return true; // Someone won
    }

    // Check for draw (board is full)
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                return false; // Board not full, game is ongoing
            }
        }
    }
    return true; // Board is full, it's a draw
}

/**
 * @brief The recursive Minimax algorithm.
 */
int minimax(vector<vector<char>>& board, int depth, bool is_maximizing) {
    int score = evaluate_board(board);

    // Terminal condition: game over
    if (score == 10 || score == -10) {
        return score;
    }

    // Terminal condition: draw
    if (is_game_over(board)) {
        return 0;
    }

    if (is_maximizing) { // Player 'X' (computer/optimal player in original Python code)
        int best_score = numeric_limits<int>::min(); // Equivalent to -math.inf
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    best_score = max(best_score, minimax(board, depth + 1, false));
                    board[i][j] = EMPTY; // Undo the move (backtracking)
                }
            }
        }
        return best_score;
    } else { // Player 'O' (opponent/minimizing player)
        int best_score = numeric_limits<int>::max(); // Equivalent to math.inf
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    best_score = min(best_score, minimax(board, depth + 1, true));
                    board[i][j] = EMPTY; // Undo the move (backtracking)
                }
            }
        }
        return best_score;
    }
}

/**
 * @brief Finds the best move for the maximizing player ('X' in the Python original, 
 * but 'O' is the computer in the main loop). We'll keep the logic from the Python 
 * `find_best_move` function which looks for the best move for the **current player** * before the call, which is **'X' (Maximizer)** in the Python `find_best_move`, 
 * but we adjust the player to **'O' (Minimizer)** since 'O' is the computer in `play_game`.
 * * *Note: The original Python code's `find_best_move` function calculates the best move for 'X' 
 * and then the main loop uses that move for 'O'. We will correct this to find the best move for 'O'.*
 * * **Correction:** Since the `play_game` function uses `find_best_move` for `PLAYER_O` 
 * (the computer), we must modify this function to calculate the best move for the **minimizing** player ('O').
 */
pair<int, int> find_best_move(vector<vector<char>>& board) {
    int best_value = numeric_limits<int>::max(); // 'O' is the Minimizer
    pair<int, int> best_move = {-1, -1};

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_O; // Try computer's move ('O')
                // Call minimax for the *next* player (Maximizer 'X')
                int move_value = minimax(board, 0, true); 
                board[i][j] = EMPTY; // Undo the move

                if (move_value < best_value) { // Find minimum score for 'O'
                    best_move = {i, j};
                    best_value = move_value;
                }
            }
        }
    }
    return best_move;
}

/**
 * @brief Main game loop for Tic-Tac-Toe.
 */
void play_game() {
    // Initialize an empty board
    vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY));
    
    cout << "Tic-Tac-Toe: Player X (you) vs. Player O (computer)" << endl;
    print_board(board);

    while (true) {
        // --- Player X (Human) ---
        int row, col;
        while (true) {
            cout << "Enter your move row (0, 1, or 2): ";
            if (!(cin >> row)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }
            cout << "Enter your move column (0, 1, or 2): ";
            if (!(cin >> col)) {
                 cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number." << endl;
                continue;
            }

            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
                cout << "Invalid input! Please enter 0, 1, or 2 for both row and column." << endl;
            } else if (board[row][col] != EMPTY) {
                cout << "Invalid move! That spot is already taken. Try again." << endl;
            } else {
                break; // Valid human move
            }
        }
        board[row][col] = PLAYER_X;
        
        // Check game status after Human's move
        if (is_game_over(board)) {
            print_board(board);
            int score = evaluate_board(board);
            if (score == 10) {
                cout << "You win!" << endl;
            } else {
                cout << "It's a draw!" << endl;
            }
            break;
        }

        // --- Player O (Computer) ---
        cout << "Computer's turn..." << endl;
        pair<int, int> best_move = find_best_move(board);
        board[best_move.first][best_move.second] = PLAYER_O;

        print_board(board);
        
        // Check game status after Computer's move
        if (is_game_over(board)) {
            int score = evaluate_board(board);
            if (score == -10) {
                cout << "Computer wins!" << endl;
            } else {
                cout << "It's a draw!" << endl;
            }
            break;
        }
    }
}

int main() {
    // This allows you to enter a number and press enter, then enter another number
    // and press enter, making it more interactive on standard consoles.
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);

    play_game();
    return 0;
}