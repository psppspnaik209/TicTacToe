// game.h
#ifndef GAME_H
#define GAME_H

#include <vector>

const char EMPTY = ' ';

void displayBoard(const std::vector<char>& board, bool showPositions);
bool isWinner(const std::vector<char>& board, char symbol);
bool isBoardFull(const std::vector<char>& board);
int minimax(std::vector<char> board, char currentPlayer, char computerSymbol, char playerSymbol);
int getBestMove(std::vector<char> board, char computerSymbol, char playerSymbol);
void playerMove(std::vector<char>& board, char playerSymbol);
void computerMove(std::vector<char>& board, char computerSymbol, char playerSymbol);
void playGame();

#endif
