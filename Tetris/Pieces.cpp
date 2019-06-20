
#include "Pieces.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <assert.h>


//constexpr int NUMBER_OF_PIECES = 7;
//constexpr int NUMBER_OF_ROTATIONS = 4;
//constexpr int NUMBER_OF_ROWS = 5;
//constexpr int NUMBER_OF_COLS = 5;

constexpr int ROTATE_NONE = 0;
constexpr int ROTATE_NINETY = 1;
constexpr int ROTATE_ONE_EIGHTY = 2;
constexpr int ROTATE_TWO_SEVENTY = 3;

int tetronimo[NUMBER_OF_PIECES][NUMBER_OF_ROTATIONS][NUMBER_OF_ROWS][NUMBER_OF_COLS] =
{
	//line
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 1, 1, 1, 1 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		}
	},
	//T 
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 2, 2, 2, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 0, 2, 2, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 2, 2, 2, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 2, 2, 0, 0 },
			{ 0, 0, 2, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		}
	},
	//z 
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 3, 3, 0, 0 },
			{ 0, 0, 3, 3, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 3, 0 },
			{ 0, 0, 3, 3, 0 },
			{ 0, 0, 3, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 3, 3, 0, 0 },
			{ 0, 0, 3, 3, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 3, 0, 0 },
			{ 0, 3, 3, 0, 0 },
			{ 0, 3, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		}
	},
	//z mirror
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 4, 4, 0 },
			{ 0, 4, 4, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 4, 0, 0 },
			{ 0, 0, 4, 4, 0 },
			{ 0, 0, 0, 4, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 4, 4, 0 },
			{ 0, 4, 4, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 4, 0, 0, 0 },
			{ 0, 4, 4, 0, 0 },
			{ 0, 0, 4, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		}

	},
	//L
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 5, 0 },
			{ 0, 5, 5, 5, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 5, 0, 0 },
			{ 0, 0, 5, 0, 0 },
			{ 0, 0, 5, 5, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 5, 5, 5, 0 },
			{ 0, 5, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 5, 5, 0, 0 },
			{ 0, 0, 5, 0, 0 },
			{ 0, 0, 5, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},

	},
	//L mirror
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 6, 0, 0, 0 },
			{ 0, 6, 6, 6, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 6, 6, 0 },
			{ 0, 0, 6, 0, 0 },
			{ 0, 0, 6, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 6, 6, 6, 0 },
			{ 0, 0, 0, 6, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 6, 0, 0 },
			{ 0, 0, 6, 0, 0 },
			{ 0, 6, 6, 0, 0 },
			{ 0, 0, 0, 0, 0 }
		},

	},
	//square
	{
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 7, 7, 0 },
			{ 0, 0, 0, 0, 0 }
		}

	}
};


//helper function which allocates memory for a 2D array
//then fills the array with values corresponding to a tetronimo
//of given type and rotation
void Pieces::pieceHelper() {
	piece = new int*[NUMBER_OF_ROWS];
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		piece[i] = new int[NUMBER_OF_COLS];
		for (int j = 0; j < NUMBER_OF_COLS; j++) {
			piece[i][j] = tetronimo[pieceType][rotation][i][j];
		}
	}
}

Pieces::Pieces() {
	srand(time(NULL));
	pieceType = rand() % NUMBER_OF_PIECES;
	rotation = ROTATE_NONE;
	matrixWidth = NUMBER_OF_COLS;
	matrixHeight = NUMBER_OF_ROWS;
	pieceHelper();
}

Pieces::Pieces(int type) {
	if (type >= 0 && type <= 6) {
		pieceType = type;
		pieceHelper();
	}
	else {
		pieceType = NULL_PIECE;
		piece = new int* [NUMBER_OF_ROWS];
		for (int i = 0; i < NUMBER_OF_ROWS; i++) {
			piece[i] = new int[NUMBER_OF_COLS];
			for (int j = 0; j < NUMBER_OF_COLS; j++) {
				piece[i][j] = 0;
			}
		}
	}
	rotation = ROTATE_NONE;
	matrixWidth = NUMBER_OF_COLS;
	matrixHeight = NUMBER_OF_ROWS;
}

Pieces::Pieces(int type, int rotate) {
	if (type >= 0 && type <= 6) {
		pieceType = type;
	}
	else {
		//std::cout << "pieceType must be between 0 and 6 inclusive. Defaulting to line piece (0)" << endl;
		pieceType = 0;
	}
	if (rotate >= 0 && rotate <= 3) {
		rotation = rotate;
	}
	else {
		//std::cout << "rotation must be between 0 and 3 inclusive. Defaulting to no rotation (0)" << endl;
		rotation = 0;
	}
	matrixWidth = NUMBER_OF_COLS;
	matrixHeight = NUMBER_OF_ROWS;
	pieceHelper();
}

void Pieces::rotatePiece() {
	rotation = (rotation + 1) % NUMBER_OF_ROTATIONS;
	assert(piece!=nullptr);
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLS; j++) {
			piece[i][j] = tetronimo[pieceType][rotation][i][j];
		}
	}
}

int Pieces::getCurrentPiece() {
	return pieceType;
}

void Pieces::get2DArray(int** copy) {
	for (int i = 0; i < getMatrixHeight(); i++) {
		for (int j = 0; j < getMatrixWidth(); j++) {
			copy[i][j] = tetronimo[pieceType][rotation][i][j];
		}
	}
	return;
}

int Pieces::getRotation() {
	return rotation;
}

int Pieces::getSpawnOffset(int axis) {
	return spawnOffset[pieceType][axis];
}

int Pieces::getCornerOffset(int axis) {
	return tetronimoCornerOffset[pieceType][rotation][axis];
}

int Pieces::getPieceWidth() {
	return tetronimoWidth[pieceType][rotation];
}

int Pieces::getMatrixWidth() {
	return matrixWidth;
}

int Pieces::getMatrixHeight() {
	return matrixHeight;
}

ostream & operator<<(ostream & os, const Pieces & block) {
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLS; j++) {
			os << block.piece[i][j] << " ";
		}
		os << endl;
	}
	return os;
}

Pieces::~Pieces() {
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		delete[](piece[i]);
	}
	delete[] piece;
}
