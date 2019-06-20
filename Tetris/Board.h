#pragma once

#include <iostream>
#include <deque>
#include <mutex>

constexpr int FREE = 0;
constexpr int LINE_PIECE = 1;
constexpr int T_PIECE = 2;
constexpr int Z_PIECE = 3;
constexpr int Z_MIRROR_PIECE = 4;
constexpr int L_PIECE = 5;
constexpr int L_MIRROR_PIECE = 6;
constexpr int SQUARE_PIECE = 7;
constexpr int GHOST = 8;
constexpr int ACTIVE = 9;
constexpr int DEFAULT_ROTATE_VALUE = 0;

using namespace std;

class Board
{
private:
	int bottomMostHelper();
	bool pieceInPlay;
	bool pieceSaved;
	int** gameBoard = nullptr;
	deque <int> pieceQueue;
	int savedPiece;
	//0: no rotation, 1: 90 degrees, 2: 180 degreees, 3: 270 degrees
	int currentPieceRotationValue = DEFAULT_ROTATE_VALUE;

	//board width and height
	int boardWidth;
	int boardHeight;

	mutex modify_board;
public: 
	//Creates a gameboard of size 10x20 and fills the queue with random pieces
	Board();

	//Creates a gameboard of size 10x20 and fills the queue with the given piece
	//(0: line, 1: T, 2: z, 3: z mirror, 4: L, 5: L mirror, 6: square)
	Board(int pieceType);

	//Copy constructor
	Board(const Board& board);

	//get function for variable savedPiece
	int getSavedPiece();

	//get function for variable currentPiece
	int getCurrentPiece();

	//returns a deep copy of the game board
	void get2DArray(int** copyArray);

	//get function to get board width
	int getMatrixWidth();

	//get function for board height
	int getMatrixHeight();

	//returns a deep copy of the pieceQueue
	deque<int> getPieceQueue();

	//gets flag to check if an active piece is on the board
	bool getPieceInPlay();

	//creates the piece at the front of pieceQueue onto the board
	//should be called once the last piece turns inactive
	bool spawnPiece();

	//if savedPiece is NULL_PIECE then sets savedPiece to front of pieceQueue then pops front of pieceQueue
	//otherwise swaps savedPiece and front of pieceQueue
	void savePiece();

	//attempts to drop the active piece by one tile
	//if the piece cannot fall, the board is not changed and the function returns false
	//if the piece can fall, active piece drops by one tile and function returns true
	//should be called every second or so
	bool boardGravity();

	//moves the active piece to the left by one column if possible
	void moveLeft();

	//moves the active piece to the right by one column if possible
	void moveRight();

	//converts all active pieces into inactive pieces, pops pieceQueue and pushes a new item to the back of pieceQueue
	//should be called every time a piece has reached as far bottom as it can go
	//AND has not moved or rotated for a second
	void turnInactive();

	//deletes old ghosts on the board
	//then creates outline of where piece will drop if it goes straight down
	//should be called every time a piece is spawned, moved, or rotated
	void createGhost();

	//sends the active piece as far bottom as possible
	void sendToBottom();

	//grabs a rotated piece and places it onto the game board
	void rotatePiece();

	//checks each row of the gameBoard if it is fully occupied
	//if so, counts the number of full rows, clears all full rows, drops placed tiles depending on lines cleared and returns the number.
	//should be called whenever a piece is placed to check if lines are full and clear them
	int clearFullLine();

	//overloaded << operator to print the gameBoard to outputstream
	friend ostream& operator<<(ostream& os, const Board& b);

	~Board();
};

