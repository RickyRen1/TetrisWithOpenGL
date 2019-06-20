#ifndef _PIECES_
#define _PIECES_

#include <iostream>

constexpr int NULL_PIECE = -1;

constexpr int NUMBER_OF_PIECES = 7;
constexpr int NUMBER_OF_ROTATIONS = 4;
constexpr int NUMBER_OF_ROWS = 5;
constexpr int NUMBER_OF_COLS = 5;
constexpr int AXIS_DIMENSIONS = 2;

using namespace std;

class Pieces {
private:
	void pieceHelper();
	int** piece = nullptr;
	int pieceType;
	int rotation;
	int matrixWidth;
	int matrixHeight;


	//gives the displacement to tetronimo[pieceType][rotation][0][0] from the top left most ACTIVE piece
	//left most part of piece given precedence over top most part of piece
	int tetronimoCornerOffset[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS][AXIS_DIMENSIONS] =
	{
		//line
		{
			{ -1, -2 },
			{ -2, -1 },
			{ 0, -2 },
			{ -2, 0 },
		},
		//T
		{
			{ -1, -2 },
			{ -2, -1 },
			{ -1, -2 },
			{ -1, -2 },
		},
		//z
		{
			{ -1, -1 },
			{ -2, -2 },
			{ -1, -2 },
			{ -1, -2 },
		},
		//z mirror
		{
			{ -1, -2 },
			{ -2, -1 },
			{ -1, -3 },
			{ -1, -1 },
		},
		//L
		{
			{ -1, -2 },
			{ -2, -1 },
			{ -1, -2 },
			{ -1, -1 },
		},
		//L mirror
		{
			{ -1, -1 },
			{ -2, -1 },
			{ -1, -2 },
			{ -1, -3 },
		},
		//square
		{
			{ -2, -2 },
			{ -2, -2 },
			{ -2, -2 },
			{ -2, -2 },
		},
	};

	//axis offset to spawn the block in the top middle of the game board
	//0: x axis, axis_offset 1: y axis
	int spawnOffset[NUMBER_OF_PIECES][AXIS_DIMENSIONS] = 
	{
		//line
		{ 2, 3 },
		//T
		{ 2, 2 },
		//z
		{ 2, 2 },
		//z mirror
		{ 2, 2 },
		//L
		{ 2, 2 },
		//L mirror
		{ 2, 2 },
		//square
		{ 2, 1 },
	};

	int tetronimoWidth[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS] =
	{
		//line
		{ 4, 1, 4, 1 },
		//T
		{ 3, 2, 3, 2 },
		//z
		{ 3, 2, 3, 2 },
		//z mirror
		{ 3, 2, 3, 2 },
		//L
		{ 3, 2, 3, 2 },
		//L mirror
		{ 3, 2, 3, 2 },
		//square
		{ 2, 2, 2, 2 }
	};


public:
	//default constructor which creates a random piece of default rotation
	Pieces();

	//constructor which specifies what piece (0: line, 1: T, 2: z, 3: z mirror, 4: L, 5: L mirror, 6: square) you want to generate
	Pieces(int type);

	//constructor which specifies what piece in what orientation (0: base, 1: 90 clockwise, 2: 180, 3: 270) you want to generate
	Pieces(int type, int rotate);

	//rotates the current piece on hand
	void rotatePiece();

	//gets the current piece type
	int getCurrentPiece();

	//returns a pointer to a 2D array containing a copy of the piece you have. must be freed
	void get2DArray(int** copy);

	//returns the rotational value of the piece
	//0 == default, 1 == 90 degrees, 2 == 180 degrees, 3 == 270 degrees
	int getRotation();

	//returns the x or y axis offset to place the piece properly on the game board
	int getSpawnOffset(int axis);

	//returns the x or y axis offset to place a new piece properly on the game board
	int getCornerOffset(int axis);

	//returns the width of the piece to check bounds
	int getPieceWidth();

	//returns the number of columns in the piece array
	int getMatrixWidth();

	//returns the number of rows in the piece array
	int getMatrixHeight();

	//overloaded operator for printing
	friend ostream& operator<<(ostream& os, const Pieces& block);

	//deconstructor which frees the memory taken up by the variable piece
	~Pieces();

};



#endif