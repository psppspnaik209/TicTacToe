// game.cpp
#include "game.h"
#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

void displayBoard(const vector<char>& board, bool showPositions, const vector<int>& winningCombo) {
    cout << "\n";
    for (int i = 0; i < 9; ++i) {
        char displayChar = board[i];
        if (displayChar == EMPTY) {
            displayChar = showPositions ? ('1' + i) : ' ';
        }
        string cellStr = " ";
        if (!winningCombo.empty() && find(winningCombo.begin(), winningCombo.end(), i) != winningCombo.end()) {
            // Cell is part of the winning combination
            cellStr = "/";
            cellStr += displayChar;
            cellStr += "/";
        } else {
            cellStr += displayChar;
            cellStr += " ";
        }
        cout << cellStr;
        if (i % 3 != 2) {
            cout << "|";
        } else if (i != 8) {
            cout << "\n---+---+---\n";
        }
    }
    cout << "\n\n";
}

bool isWinner(const vector<char>& board, char symbol) {
    const int WIN_COMBINATIONS[8][3] = {
        {0, 1, 2}, // Rows
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6}, // Columns
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8}, // Diagonals
        {2, 4, 6}
    };
    for (int i = 0; i < 8; ++i) {
        const int* combo = WIN_COMBINATIONS[i];
        if (board[combo[0]] == symbol && board[combo[1]] == symbol && board[combo[2]] == symbol) {
            return true;
        }
    }
    return false;
}

vector<int> getWinningCombination(const vector<char>& board, char symbol) {
    const int WIN_COMBINATIONS[8][3] = {
        {0, 1, 2}, // Rows
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6}, // Columns
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8}, // Diagonals
        {2, 4, 6}
    };
    for (int i = 0; i < 8; ++i) {
        const int* combo = WIN_COMBINATIONS[i];
        if (board[combo[0]] == symbol && board[combo[1]] == symbol && board[combo[2]] == symbol) {
            return vector<int>{combo[0], combo[1], combo[2]};
        }
    }
    return vector<int>(); // No winning combination
}

bool isBoardFull(const vector<char>& board) {
    for (char cell : board) {
        if (cell == EMPTY) return false;
    }
    return true;
}


int minimax(vector<char> board, char currentPlayer, char computerSymbol, char playerSymbol) {
    if (isWinner(board, computerSymbol)) return 10;
    if (isWinner(board, playerSymbol)) return -10;
    if (isBoardFull(board)) return 0;

    vector<int> scores;

    for (int i = 0; i < board.size(); ++i) {
        if (board[i] == EMPTY) {
            board[i] = currentPlayer;
            int score = minimax(board, (currentPlayer == computerSymbol) ? playerSymbol : computerSymbol, computerSymbol, playerSymbol);
            scores.push_back(score);
            board[i] = EMPTY;
        }
    }

    if (currentPlayer == computerSymbol) {
        int maxScore = -1000;
        for (int s : scores) {
            if (s > maxScore) maxScore = s;
        }
        return maxScore;
    } else {
        int minScore = 1000;
        for (int s : scores) {
            if (s < minScore) minScore = s;
        }
        return minScore;
    }
}

int getBestMove(vector<char> board, char computerSymbol, char playerSymbol) {
    int bestScore = -1000;
    int move = -1;

    for (int i = 0; i < board.size(); ++i) {
        if (board[i] == EMPTY) {
            board[i] = computerSymbol;
            int score = minimax(board, playerSymbol, computerSymbol, playerSymbol);
            board[i] = EMPTY;

            if (score > bestScore) {
                bestScore = score;
                move = i;
            }
        }
    }
    return move;
}

// some how dual checking the input for any nonsense charc fixes the infinite loop
void playerMove(vector<char>& board, char playerSymbol) {
    int move;
    while (true) {
        cout << "Enter your move (1-9): ";
        if (!(cin >> move)) {
            cout << "Invalid input. Please enter a number between 1 and 9.\n";
            cin.clear(); // Clear the error flags
            cin.ignore(1000, '\n'); // Discard invalid input
            continue;
        }
        cin.ignore(1000, '\n'); // Clear any extra input after the number
        --move; // Adjust for 0-based index
        if (move >= 0 && move < 9 && board[move] == EMPTY) {
            board[move] = playerSymbol;
            break;
        } else {
            cout << "Invalid move. Try again.\n";
        }
    }
}

void computerMove(vector<char>& board, char computerSymbol, char playerSymbol) {
    int move = getBestMove(board, computerSymbol, playerSymbol);
    board[move] = computerSymbol;
    cout << "Computer chooses position " << (move + 1) << "\n";
}

void playGame() {
    vector<char> board(9, EMPTY);
    char playerSymbol, computerSymbol;
    int playerScore = 0, computerScore = 0, draws = 0;
    char choice;

    cout << "Welcome to Tic-Tac-Toe!\n";
    cout << "Do you want to be 'X' or 'O'? ";
    cin >> playerSymbol;
    playerSymbol = toupper(playerSymbol);
    while (playerSymbol != 'X' && playerSymbol != 'O') {
        cout << "Invalid choice. Please choose 'X' or 'O': ";
        cin >> playerSymbol;
        playerSymbol = toupper(playerSymbol);
    }
    computerSymbol = (playerSymbol == 'X') ? 'O' : 'X';

    do {
        board.assign(9, EMPTY);
        char currentPlayer = 'X';
        cout << "\nGame start!\n";
        // Display initial board with positions
        displayBoard(board, true);
        cout << "This is the default placement of the board" << endl;
        cout << "Use the grid number positions to place your option" << endl << endl;

        while (true) {
            if (currentPlayer == playerSymbol) {
                playerMove(board, playerSymbol);
            } else {
                computerMove(board, computerSymbol, playerSymbol);
            }
            // Display the board without positions
            if (isWinner(board, currentPlayer)) {
                vector<int> winningCombo = getWinningCombination(board, currentPlayer);
                displayBoard(board, false, winningCombo);
                if (currentPlayer == playerSymbol) {
                    cout << "You win!\n";
                    ++playerScore;
                } else {
                    cout << "Computer wins!\n";
                    ++computerScore;
                }
                break;
            } else if (isBoardFull(board)) {
                displayBoard(board, false);
                cout << "It's a draw!\n";
                ++draws;
                break;
            } else {
                displayBoard(board, false);
            }
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

        cout << "\nScore:\n";
        cout << "Player:   " << playerScore << "\n";
        cout << "Computer: " << computerScore << "\n";
        cout << "Draws:    " << draws << "\n";

        cout << "\nDo you want to play again? (y/n): ";
        cin >> choice;
        choice = tolower(choice);
    } while (choice == 'y');

    cout << "Thank you for playing!\n";
}