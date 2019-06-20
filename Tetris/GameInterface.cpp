#include "Pieces.h"
#include "Board.h"
#include "GameGraphics.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>

#include <SFML\Graphics.hpp>

constexpr int POLL_RATE = 30;

using namespace std;

//void userInput(Board* gameBoard) {
//	bool buttonPress = false;
//	while (1) {
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//			if (!buttonPress) {
//				gameBoard->moveLeft();
//				gameBoard->createGhost();
//				buttonPress = true;
//			}
//		}
//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//			if (!buttonPress) {
//				gameBoard->moveRight();
//				gameBoard->createGhost();
//				buttonPress = true;
//			}
//		}
//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//			if (!buttonPress) {
//				gameBoard->rotatePiece();
//				gameBoard->createGhost();
//				buttonPress = true;
//			}
//		}
//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//			gameBoard->boardGravity();
//			gameBoard->createGhost();
//			buttonPress = true;
//			Sleep(POLL_RATE);
//		}
//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
//			if (!buttonPress) {
//				gameBoard->savePiece();
//				gameBoard->createGhost();
//				buttonPress = true;
//			}
//		}
//		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
//			if (!buttonPress) {
//				gameBoard->sendToBottom();
//				gameBoard->turnInactive();
//				gameBoard->createGhost();
//				buttonPress = true;
//			}
//		}
//		else {
//			buttonPress = false;
//		}
//	}
//}

void gameLogic(Board* gameBoard) {
	bool keepPlaying = false;
	do {
		if (!gameBoard->getPieceInPlay()) {
			keepPlaying = gameBoard->spawnPiece();
			gameBoard->createGhost();
		}
		Sleep(1000);
		if (!gameBoard->boardGravity()) {
			gameBoard->turnInactive();
			gameBoard->clearFullLine();
			cout << "piece can no longer be dropped" << endl;
			cout << "getPieceInPlay == " << gameBoard->getPieceInPlay() << endl;
		}
	} while (keepPlaying);
	if (!keepPlaying) {
		cout << "Game is over" << endl;
	}
}

int main() {
	Board gameBoard;
	thread th1(renderWindow, &gameBoard);
	thread th2(gameLogic, &gameBoard);
	th1.join();
	th2.join();
	return 0;
}

void pieceTests() {
	Pieces defaultPiece;
	Pieces selectType(3);
	Pieces invalidType(10);
	Pieces selectAll(0, 3);
	Pieces invalidAll(10, 7);
	cout << "Printing default piece" << endl;
	cout << defaultPiece << endl;
	cout << "Printing selected z mirror piece" << endl;
	cout << selectType << endl;
	cout << "Printing selected line piece rotated 270 degrees" << endl;
	cout << selectAll << endl;
	cout << "Rotating default piece 90 degrees" << endl;
	defaultPiece.rotatePiece();
	cout << defaultPiece << endl;
	cout << "Rotating selected z mirror piece 90 degrees" << endl;
	selectType.rotatePiece();
	cout << selectType << endl;
	cout << "Rotating selected line piece rotated 90 degrees" << endl;
	selectAll.rotatePiece();
	cout << selectAll << endl;
	cout << "Getting default piece" << endl;
	int** copiedPiece = new int* [defaultPiece.getMatrixHeight()];
	for (int i = 0; i < defaultPiece.getMatrixHeight(); i++) {
		copiedPiece[i] = new int[defaultPiece.getMatrixWidth()];
	}
	defaultPiece.get2DArray(copiedPiece);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << copiedPiece[i][j] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < defaultPiece.getMatrixHeight(); i++) {
		delete[] copiedPiece[i];
	}
	delete[] copiedPiece;
}

void boardTests() {
	cout << "Creating newGameBoard" << endl;
	Board newGameBoard;
	cout << "Printing game board" << endl;
	cout << newGameBoard;
	cout << "Spawning piece" << endl;
	newGameBoard.spawnPiece();
	cout << newGameBoard;
	cout << "Saving piece with nothing already saved" << endl;
	newGameBoard.savePiece();
	cout << "Saved piece is: " << newGameBoard.getSavedPiece() << endl;
	cout << "Now active piece is: " << newGameBoard.getCurrentPiece() << endl;
	cout << newGameBoard << endl;
	cout << "Saving piece with something already saved" << endl;
	newGameBoard.savePiece();
	cout << "Saved piece is: " << newGameBoard.getSavedPiece() << endl;
	cout << "Now active piece is: " << newGameBoard.getCurrentPiece() << endl;
	cout << newGameBoard << endl;
	cout << "moving to the left once" << endl;
	newGameBoard.moveLeft();
	cout << newGameBoard << endl;
	cout << "moving to the right once" << endl;
	newGameBoard.moveRight();
	cout << newGameBoard << endl;
	cout << "rotating piece once" << endl;
	newGameBoard.rotatePiece();
	cout << newGameBoard << endl;
	cout << "rotating piece once" << endl;
	newGameBoard.rotatePiece();
	cout << newGameBoard << endl;
	cout << "rotating piece once" << endl;
	newGameBoard.rotatePiece();
	cout << newGameBoard << endl;
	cout << "rotating piece once" << endl;
	newGameBoard.rotatePiece();
	cout << newGameBoard << endl;
	cout << "moving to the left 30 times" << endl;
	for (int i = 0; i < 30; i++)
		newGameBoard.moveLeft();
	cout << newGameBoard << endl;
	cout << "moving to the right 30 times" << endl;
	for (int i = 0; i < 30; i++)
		newGameBoard.moveRight();
	cout << newGameBoard << endl;
	cout << "Dropping piece 30 times" << endl;
	for (int i = 0; i < 30; i++)
		newGameBoard.boardGravity();
	cout << newGameBoard << endl;
	cout << "Turning active piece inactive" << endl;
	newGameBoard.turnInactive();
	cout << newGameBoard << endl;
	cout << "Spawning new piece" << endl;
	newGameBoard.spawnPiece();
	cout << newGameBoard << endl;
	cout << "Creating ghost for new piece" << endl;
	newGameBoard.createGhost();
	cout << newGameBoard << endl;
	cout << "Sending new piece to the bottom" << endl;
	newGameBoard.sendToBottom();
	cout << "Turning active piece inactive" << endl;
	newGameBoard.turnInactive();
	cout << newGameBoard << endl;
	cout << "Spawning new piece" << endl;
	newGameBoard.spawnPiece();
	cout << "Creating ghost for new piece" << endl;
	newGameBoard.createGhost();
	cout << newGameBoard << endl;
	cout << "Dropping new piece to bottom" << endl;
	newGameBoard.sendToBottom();
	cout << newGameBoard << endl;
	cout << "Turning active piece inactive" << endl;
	newGameBoard.turnInactive();
	cout << newGameBoard << endl;
}

void unitTests() {
	pieceTests();
	boardTests();
}