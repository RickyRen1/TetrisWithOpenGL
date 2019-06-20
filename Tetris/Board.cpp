#include "Board.h"
#include "Pieces.h"
#include <random>
#include <iostream>
#include <assert.h>
#include <time.h>

//playable area is width 10
constexpr int BOARD_WIDTH = 10;
//playable area is height 20
constexpr int BOARD_HEIGHT = 25;
//constexpr int REDUNDANT_BOARD_SPACE = 5;

constexpr int QUEUE_SIZE = 6;

//constexpr int NUMBER_OF_ROTATIONS = 4;
//constexpr int NUMBER_OF_PIECES = 7;
//constexpr int NUMBER_OF_ROWS = 5;
//constexpr int NUMBER_OF_COLS = 5;

constexpr int X_AXIS = 0;
constexpr int Y_AXIS = 1;

#define RANDOM_PIECE rand()%NUMBER_OF_PIECES
#define TILE_FILLED(Y,X) (gameBoard[Y][X]>=LINE_PIECE&&gameBoard[Y][X]<=SQUARE_PIECE)
#define PIECE_FILLED(Y,X) (piece[Y][X]>=LINE_PIECE&&piece[Y][X]<=SQUARE_PIECE)


using namespace std;

Board::Board()
{
	srand(time(NULL));
	gameBoard = new int*[BOARD_HEIGHT];
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		gameBoard[i] = new int[BOARD_WIDTH];
		memset(gameBoard[i], FREE, sizeof(gameBoard[i]) * BOARD_WIDTH);
	}
	savedPiece = NULL_PIECE;
	for (int i = 0; i < QUEUE_SIZE; i++) {
		pieceQueue.push_back(RANDOM_PIECE);
	}
	boardWidth = BOARD_WIDTH;
	boardHeight = BOARD_HEIGHT;
	pieceInPlay = false;
	pieceSaved = false;
}

Board::Board(int pieceType)
{
	gameBoard = new int*[BOARD_HEIGHT];
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		gameBoard[i] = new int[BOARD_WIDTH];
		memset(gameBoard[i], FREE, sizeof(gameBoard[i]) * BOARD_WIDTH);
	}
	savedPiece = NULL_PIECE;
	for (int i = 0; i < QUEUE_SIZE; i++) {
		pieceQueue.push_back(pieceType);
	}
	boardWidth = BOARD_WIDTH;
	boardHeight = BOARD_HEIGHT;
	pieceInPlay = false;
	pieceSaved = false;
}

Board::Board(const Board& board) {
	savedPiece = board.savedPiece;
	//continueFalling = board.continueFalling;
	currentPieceRotationValue = board.currentPieceRotationValue;
	boardWidth = board.boardWidth;
	boardHeight = board.boardHeight;
	gameBoard = new int* [boardHeight];
	pieceQueue = board.pieceQueue;
	for (int i = 0; i < boardHeight; i++) {
		gameBoard[i] = new int[boardWidth];
		for (int j = 0; j < boardWidth; j++) {
			gameBoard[i][j] = board.gameBoard[i][j];
		}
	}
	pieceInPlay = false;
	pieceSaved = false;
}

int Board::getSavedPiece() {
	return savedPiece;
}

int Board::getCurrentPiece()
{
	return pieceQueue.front();
}

void Board::get2DArray(int** copyArray)
{
	if (gameBoard != nullptr) {
		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				copyArray[i][j] = gameBoard[i][j];
			}
		}
	}
	else {
		cerr << "gameBoard does not exist. Cannot copy" << endl;
	}
	return;
}

int Board::getMatrixWidth() {
	return boardWidth;
}

int Board::getMatrixHeight() {
	return boardHeight;
}

deque<int> Board::getPieceQueue() {
	deque<int> copiedPieceQueue(pieceQueue);
	return copiedPieceQueue;
}

bool Board::getPieceInPlay() {
	return pieceInPlay;
}

bool Board::spawnPiece() {
	assert(!pieceQueue.empty());
	int currentPiece = pieceQueue.front();
	//cout << "currentPiece in spawnPiece is: " << currentPiece << endl;
	Pieces createdPiece(currentPiece);
	//cout << "Created Piece from spawnPiece:" << endl;
	//cout << createdPiece << endl;
	int xOffset = createdPiece.getSpawnOffset(X_AXIS);
	int yOffset = createdPiece.getSpawnOffset(Y_AXIS);
	int** copy = new int*[createdPiece.getMatrixHeight()];
	for (int i = 0; i < createdPiece.getMatrixHeight(); i++) {
		copy[i] = new int[createdPiece.getMatrixWidth()];
	}
	createdPiece.get2DArray(copy);
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLS; j++) {
			//if the area the piece will be placed is occupied return false
			if (copy[i][j] == pieceQueue.front() + 1 && gameBoard[i + yOffset][j + xOffset] != FREE && gameBoard[i + yOffset][j + xOffset] != GHOST) {
				return false;
			}
		}
	}
	modify_board.lock();
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLS; j++) {
			if (copy[i][j] == pieceQueue.front() + 1) {
				gameBoard[i + yOffset][j + xOffset] = ACTIVE;
			}
		}
	}
	pieceInPlay = true;
	currentPieceRotationValue = DEFAULT_ROTATE_VALUE;
	modify_board.unlock();
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		delete[](copy[i]);
	}
	delete[] copy;
	return true;
}

void Board::savePiece() {
	if (pieceSaved) {
		return;
	}
	else {
		pieceSaved = true;
		if (savedPiece != NULL_PIECE) {
			int temp = savedPiece;
			savedPiece = pieceQueue.front();
			pieceQueue.pop_front();
			pieceQueue.push_front(temp);
		}
		else {
			assert(!pieceQueue.empty());
			savedPiece = pieceQueue.front();
			pieceQueue.pop_front();
			pieceQueue.push_back(RANDOM_PIECE);
		}
		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				if (gameBoard[i][j] == ACTIVE) {
					gameBoard[i][j] = FREE;
				}
			}
		}
		spawnPiece();
	}
}

bool Board::boardGravity() {
	//minus 2 so that index doesn't go out of bounds
	//start loop from the end so that you don't move parts that have already been moved
	modify_board.lock();
	bool continueFalling = true;
	for (int i = BOARD_HEIGHT - 1; i > 0; i--) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			//if part of the actively falling piece is found
			if (gameBoard[i][j] == ACTIVE) {
				//if the actively falling piece hits the bottom or an obstacle, stop falling
				if ((i + 1) > (BOARD_HEIGHT - 1)) {
					continueFalling = false;
					modify_board.unlock();
					return continueFalling;
				}
				if (gameBoard[i + 1][j] >= LINE_PIECE && gameBoard[i+1][j] <= SQUARE_PIECE) {
					continueFalling = false;
					modify_board.unlock();
					return continueFalling;
				}
			}
		}
		//if (!continueFalling) {
		//	break;
		//}
	}
	if (continueFalling) {
		for (int i = BOARD_HEIGHT - 2; i > 0; i--) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				//if the part of the piece found will not fall into itself, drop piece by one row
				if (gameBoard[i][j] == ACTIVE && gameBoard[i+1][j] != ACTIVE) {
					gameBoard[i + 1][j] = ACTIVE;
					gameBoard[i][j] = FREE;
				}
			}
		}
	}
	//for (int i = 0; i < BOARD_HEIGHT; i++) {
	//	for (int j = 0; j < BOARD_WIDTH; j++) {
	//		cout << gameBoard[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	modify_board.unlock();
	return continueFalling;
}

void Board::moveLeft() {
	bool canMove = true;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			//if part of the actively falling piece is found
			if (gameBoard[i][j] == ACTIVE) {
				//if the piece is as far left as possible do nothing
				if (j == 0 || (gameBoard[i][j-1] >= LINE_PIECE && gameBoard[i][j-1] <= SQUARE_PIECE)) {
					canMove = false;
					return;
				}
			}
		}
	}
	if (canMove) {
		modify_board.lock();
		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				//if part of the actively falling piece is found
				if (gameBoard[i][j] == ACTIVE) {
					gameBoard[i][j - 1] = ACTIVE;
					gameBoard[i][j] = FREE;
				}
			}
		}
		modify_board.unlock();
	}
}

void Board::moveRight() {
	bool canMove = true;
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = BOARD_WIDTH - 1; j >= 0; j--) {
			//if part of the actively falling piece is found
			if (gameBoard[i][j] == ACTIVE) {
				//if the piece is as far right as possible do nothing
				if (j == BOARD_WIDTH-1 || (gameBoard[i][j + 1] >= LINE_PIECE && gameBoard[i][j+1] <= SQUARE_PIECE)) {
					canMove = false;
					return;
				}
			}
		}
	}
	if (canMove) {
		modify_board.lock();
		for (int i = 0; i < BOARD_HEIGHT; i++) {
			for (int j = BOARD_WIDTH-1; j >= 0; j--) {
				//if part of the actively falling piece is found
				if (gameBoard[i][j] == ACTIVE) {
					gameBoard[i][j + 1] = ACTIVE;
					gameBoard[i][j] = FREE;
				}
			}
		}
		modify_board.unlock();
	}
}

void Board::turnInactive() {
	modify_board.lock();
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (gameBoard[i][j] == ACTIVE) {
				//need to increase by 1 to convert from index to board data
				gameBoard[i][j] = pieceQueue.front() + 1;
			}
		}
	}
	modify_board.unlock();
	pieceQueue.pop_front();
	pieceQueue.push_back(RANDOM_PIECE);
	pieceInPlay = false;
	pieceSaved = false;
}

//helper function to find the displacement required to send the active piece as far bottom as possible
int Board::bottomMostHelper() {
	int pieceWidth = 0;
	bool bottomFound = false;
	int potentialYDisplacement = BOARD_HEIGHT - 1;
	int yCoord = 0;
	for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			//if the bottom portion of the active piece is found on lowest possible location we can't go any further
			if (gameBoard[i][j] == ACTIVE && i == BOARD_HEIGHT - 1) {
				return 0;
			}
			//if bottom portion of active piece is found
			else if (gameBoard[i][j] == ACTIVE && gameBoard[i + 1][j] != ACTIVE) {
				bottomFound = true;
				//cout << "bottom part of piece is found at gameBoard[" << i << "][" << j << "]" << endl;
				//set potential displacement to maximum possible with no obstacles
				if (BOARD_HEIGHT - 1 - i < potentialYDisplacement) {
					potentialYDisplacement = BOARD_HEIGHT - 1 - i;
				}
				//cout << "maximum possible displacement is " << potentialYDisplacement << endl;
				for (yCoord = i; yCoord < BOARD_HEIGHT; yCoord++) {
					if (TILE_FILLED(yCoord, j) && (yCoord-i-1) < potentialYDisplacement) {
						//cout << "potentialYDisplacement is changing from " << potentialYDisplacement << " to " << yCoord-i-1 << " as gameBoard[" << yCoord << "][" << j << "] is occupied by " << gameBoard[yCoord][j];
						potentialYDisplacement = yCoord - i - 1;
					}
				}
			}
		}
	}
	if (!bottomFound) {
		return 0;
	}
	return potentialYDisplacement;
}

void Board::createGhost() {
	modify_board.lock();
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (gameBoard[i][j] == GHOST) {
				gameBoard[i][j] = FREE;
			}
		}
	}
	int bottomYDisplacement = bottomMostHelper();
	cout << "bottomYDisplacement: " << bottomYDisplacement << endl;
	//created ghost would not be seen so don't need to create
	if (bottomYDisplacement == 0) {
		modify_board.unlock();
		return;
	}
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			//if index is on active piece and potential ghost location is not occupied
			//create a ghost piece as far to the bottom as possible
			if (gameBoard[i][j] == ACTIVE) {
				if (i + bottomYDisplacement < BOARD_HEIGHT) {
					if(gameBoard[i+bottomYDisplacement][j] == FREE)
						gameBoard[i + bottomYDisplacement][j] = GHOST;
				}
				else {
					cout << "error in createGhost" << endl;
					cerr << "Ghost will spawn out of bounds: " << BOARD_HEIGHT << endl;
					cerr << "row index of piece: " << i << endl;
					cerr << "displacement calculated: " << bottomYDisplacement << endl;
					modify_board.unlock();
					return;
				}
			}
		}
	}
	modify_board.unlock();
}

void Board::sendToBottom() {
	modify_board.lock();
	int bottomYDisplacement = bottomMostHelper();
	if (bottomYDisplacement == 0) {
		modify_board.unlock();
		return;
	}
	for (int i = BOARD_HEIGHT - 1; i >=0 ; i--) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (gameBoard[i][j] == ACTIVE) {
				if ((i + bottomYDisplacement) >= BOARD_HEIGHT) {
					cerr << "Piece will drop out of bounds: " << BOARD_HEIGHT << endl;
					cerr << "row index of piece: " << i << endl;
					cerr << "displacement calculated: " << bottomYDisplacement << endl;
					modify_board.unlock();
					return;
				}
				gameBoard[i + bottomYDisplacement][j] = ACTIVE;
				gameBoard[i][j] = FREE;
			}
		}
	}
	modify_board.unlock();
}

bool checkRotatable(int xCoord, int yCoord, int** gameBoard, int** piece) {
	int i = yCoord;
	int j = xCoord;
	int rotateY = 0;
	int rotateX = 0;
	for (i = yCoord, rotateY = 0; i < (yCoord + NUMBER_OF_ROWS) && rotateY < NUMBER_OF_ROWS; i++, rotateY++) {
		for (j = xCoord, rotateX = 0; j < (xCoord + NUMBER_OF_COLS) && rotateX < NUMBER_OF_COLS; j++, rotateX++) {
			if (PIECE_FILLED(rotateY, rotateX) && TILE_FILLED(i, j))
				return false;
		}
	}
	return true;
}

void Board::rotatePiece() {
	//finding location of current piece on the board
	Pieces currentPiece(pieceQueue.front(), currentPieceRotationValue);
	currentPieceRotationValue = (currentPieceRotationValue + 1) % NUMBER_OF_ROTATIONS;
	Pieces rotated(pieceQueue.front(), currentPieceRotationValue);
	int yCoord;
	int xCoord;
	bool foundPiece = false;
	for (xCoord = 0; xCoord < BOARD_WIDTH; xCoord++) {
		for (yCoord = 0; yCoord < BOARD_HEIGHT; yCoord++) {
			if (gameBoard[yCoord][xCoord] == ACTIVE) {
				foundPiece = true;
				break;
			}
		}
		if (foundPiece) {
			break;
		}
	}
	xCoord = xCoord + currentPiece.getCornerOffset(X_AXIS);
	yCoord = yCoord + currentPiece.getCornerOffset(Y_AXIS);
	cout << "found corner offset" << endl;
	int** piece = new int* [rotated.getMatrixHeight()];
	for (int i = 0; i < rotated.getMatrixHeight(); i++) {
		piece[i] = new int[rotated.getMatrixWidth()];
	}
	rotated.get2DArray(piece);
	cout << "created data for rotated piece" << endl;
	//bool roomToRotate = true;
	int rotateY = 0;
	int rotateX = 0;
	//if rotated piece will go out of bounds, nudge it back in bounds
	int nudgeX = 0;
	int nudgeY = 0;
	//setting board index to maximum possible if it were all inbounds
	int i = yCoord + NUMBER_OF_ROWS;
	int j = xCoord + NUMBER_OF_COLS;
	cout << "xCoord: " << xCoord << endl;
	cout << "yCoord: " << yCoord << endl;
	if (xCoord < 0) {
		for (rotateX = 0; rotateX < 0-xCoord; rotateX++) {
			for (rotateY = 0; rotateY < NUMBER_OF_ROWS; rotateY++) {
				cout << "checking piece[" << rotateY << "][" << rotateX << "]: " << piece[rotateY][rotateX] << endl;
				if (PIECE_FILLED(rotateY, rotateX)) {
					nudgeX++;
					break;
				}
			}
		}
	}
	else {
		for (rotateX = NUMBER_OF_COLS - 1; rotateX >= (NUMBER_OF_COLS - (j - BOARD_WIDTH)) && rotateX >= 0; rotateX--) {
			for (rotateY = NUMBER_OF_ROWS - 1; rotateY >= 0; rotateY--) {
				cout << "checking piece[" << rotateY << "][" << rotateX << "]: " << piece[rotateY][rotateX] << endl;
				if (PIECE_FILLED(rotateY, rotateX)) {
					nudgeX--;
					break;
				}
			}
		}
	}
	for (rotateY = NUMBER_OF_ROWS - 1; rotateY >= (NUMBER_OF_ROWS - (i - BOARD_HEIGHT)) && rotateY >= 0; rotateY--) {
		for (rotateX = NUMBER_OF_COLS - 1; rotateX >= 0; rotateX--) {
			if (PIECE_FILLED(rotateY, rotateX)) {
				cout << "piece will be out of bounds" << endl;
				cout << "rotateY: " << rotateY << endl;
				cout << "rotateX: " << rotateX << endl;
				nudgeY--;
				break;
			}
		}
	}
	cout << "need to nudge X by: " << nudgeX << endl;
	xCoord += nudgeX;
	yCoord += nudgeY;
	bool roomToRotate = checkRotatable(xCoord, yCoord, gameBoard, piece);
	if (roomToRotate) {
		//clear the current piece from the board so we don't have duplicates
		modify_board.lock();
		for (int i = yCoord; i < BOARD_HEIGHT; i++) {
			for (int j = xCoord; j < BOARD_WIDTH; j++) {
				if (gameBoard[i][j] == ACTIVE) {
					gameBoard[i][j] = FREE;
				}
			}
		}
		cout << "finished clearing old, now printing new" << endl;
		cout << "starting at i: " << i << endl;
		cout << "and j at: " << j << endl;
		for (int i = yCoord, rotateY = 0; i < (yCoord + NUMBER_OF_ROWS) && rotateY < NUMBER_OF_ROWS; i++, rotateY++) {
			for (int j = xCoord, rotateX = 0; j < (xCoord + NUMBER_OF_COLS) && rotateX < NUMBER_OF_COLS; j++, rotateX++) {
				if (piece[rotateY][rotateX] == pieceQueue.front() + 1) {
					if (i >= BOARD_HEIGHT) {
						cout << "error will draw at row " << i << endl;
					}
					if (j >= BOARD_WIDTH) {
						cout << "error will draw at col " << j << endl;
					}
					gameBoard[i][j] = ACTIVE;
				}
			}
		}
		modify_board.unlock();
	}
	//if we can't rotate then decrement rotation to its original value
	else {
		currentPieceRotationValue = (currentPieceRotationValue--) % NUMBER_OF_ROTATIONS;
	}
	for (int i = 0; i < rotated.getMatrixHeight(); i++) {
		delete[] piece[i];
	}
	delete[] piece;
}

int Board::clearFullLine() {
	int rowOccupancyCounter = 0;
	int linesToClear = 0;
	int i = 0;
	for (i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			if (gameBoard[i][j] >= LINE_PIECE && gameBoard[i][j] <= SQUARE_PIECE) {
				rowOccupancyCounter++;
			}
		}
		//if a line needs to be cleared, drop every placed tile above it by one and reset rowOccupancyCounter for the next line
		if (rowOccupancyCounter == BOARD_WIDTH) {
			linesToClear++;
			modify_board.lock();
			for (int j = 0; j < BOARD_WIDTH; j++) {
				gameBoard[i][j] = FREE;
				for (int yCoord = i; yCoord > 0; yCoord--) {
					if (gameBoard[yCoord-1][j] >= LINE_PIECE && gameBoard[yCoord-1][j] <= SQUARE_PIECE) {
						gameBoard[yCoord][j] = gameBoard[yCoord - 1][j];
						gameBoard[yCoord - 1][j] = FREE;
					}
				}
			}
			modify_board.unlock();
		}
		rowOccupancyCounter = 0;
	}
	return linesToClear;
}

ostream& operator<<(ostream& os, const Board& b)
{
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			os << b.gameBoard[i][j] << " ";
		}
		os << endl;
	}
	return os;
}

Board::~Board()
{
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		delete[] gameBoard[i];
	}
	delete[] gameBoard;
}

