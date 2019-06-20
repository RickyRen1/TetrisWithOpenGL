#include "Board.h"
#include "Pieces.h"
#include "GameGraphics.h"
#include <Windows.h>
#include <SFML\Graphics.hpp>

using namespace std;

#define COLOUR_LINE_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 100, 100, 255, 255)
#define COLOUR_T_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 255, 0, 255, 255)
#define COLOUR_Z_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 255, 0, 0, 255)
#define COLOUR_Z_MIRROR_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 0, 255, 0, 255)
#define COLOUR_L_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 255, 165, 0, 255)
#define COLOUR_L_MIRROR_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 0, 0, 255, 255)
#define COLOUR_SQUARE_PIECE(ARR, INDEX) colourPixel(ARR, INDEX, 255, 255, 0, 255)
#define COLOUR_GHOST(ARR, INDEX) colourPixel(ARR, INDEX, 200, 200, 200, 255)
#define COLOUR_BLACK(ARR, INDEX) colourPixel(ARR, INDEX, 0, 0, 0, 0)
#define COLOUR_WHITE(ARR, INDEX) colourPixel(ARR, INDEX, 255, 255, 255, 255)

//int tileToPixelTranslator(int row, int column, int width) {
//	int index = column + (row * width * RGBA);
//	return index;
//}

void colourPixel(sf::Uint8* pixelArray, int index, int red, int green, int blue, int alpha) {
	pixelArray[index + 0] = red;
	pixelArray[index + 1] = green;
	pixelArray[index + 2] = blue;
	pixelArray[index + 3] = alpha;
	return;
}

void outlineWhiteSquare(sf::Uint8* pixelArray, int yCoord, int xCoord, int width, int height) {
	int y = yCoord;
	int botPixelArrayIndex = width * height - width;
	for (y = yCoord; y < (height); y++) {
		int pixelArrayIndex = (xCoord + (y * width));
		COLOUR_WHITE(pixelArray, pixelArrayIndex);
		COLOUR_WHITE(pixelArray, pixelArrayIndex + width - RGBA);
	}
	for (int x = xCoord; x < (width); x++) {
		int pixelArrayIndex = (x + (yCoord * width));
		COLOUR_WHITE(pixelArray, botPixelArrayIndex);
		//cout << "botPixelArrayIndex: " << botPixelArrayIndex << endl;
		//cout << "Max index: " << width * height - 1 << endl;
		COLOUR_WHITE(pixelArray, pixelArrayIndex);
		if (botPixelArrayIndex < (width * height - RGBA)) {
			botPixelArrayIndex++;
		}
	}
}

template <class T>
void updatePixelArray(T* objectToDraw, sf::Uint8* pixelArray) {
	int boardCols = objectToDraw->getMatrixWidth();
	int boardRows = objectToDraw->getMatrixHeight();
	int pixelTileWidth = TILE_WIDTH * RGBA;
	int** gameData = new int* [boardRows];
	for (int i = 0; i < boardRows; i++) {
		gameData[i] = new int[boardCols];
	}
	objectToDraw->get2DArray(gameData);
	int pieceType = objectToDraw->getCurrentPiece();
	//zeroing out old pixel data
	//boardGenerator(boardCols*TILE_WIDTH, boardRows*TILE_WIDTH, pixelArray);
	for (int i = 0; i < boardRows; i++) {
		for (int j = 0; j < boardCols; j++) {
			if (gameData[i][j] >= LINE_PIECE && gameData[i][j] <= GHOST) {
				for (int y = (i * TILE_WIDTH); y < ((i * TILE_WIDTH) + TILE_WIDTH); y++) {
					for (int x = j * pixelTileWidth; x < ((j * pixelTileWidth) + (pixelTileWidth)); x += RGBA) {
						int pixelArrayIndex = (x + (y * pixelTileWidth * boardCols)); //columnIndex + (rowIndex * total # pixels in one row)
						//(0: empty 1: line, 2: T, 3: z, 4: z mirror, 5: L, 6: L mirror, 7: square, default: ghost)
						switch (gameData[i][j]) {
						case 0: break;
						case 1: COLOUR_LINE_PIECE(pixelArray, pixelArrayIndex); break;
						case 2: COLOUR_T_PIECE(pixelArray, pixelArrayIndex); break;
						case 3: COLOUR_Z_PIECE(pixelArray, pixelArrayIndex); break;
						case 4: COLOUR_Z_MIRROR_PIECE(pixelArray, pixelArrayIndex); break;
						case 5: COLOUR_L_PIECE(pixelArray, pixelArrayIndex); break;
						case 6: COLOUR_L_MIRROR_PIECE(pixelArray, pixelArrayIndex); break;
						case 7: COLOUR_SQUARE_PIECE(pixelArray, pixelArrayIndex); break;
						case 8: COLOUR_GHOST(pixelArray, pixelArrayIndex); break;
						default: COLOUR_BLACK(pixelArray, pixelArrayIndex); break;
						}
					}
				}
				for (int y = (i * TILE_WIDTH); y < ((i * TILE_WIDTH) + TILE_WIDTH); y++) {
					int xLeft = j * pixelTileWidth;
					//int xRight = xLeft + pixelTileWidth;
					int pixelArrayIndexLeft = (xLeft + (y * pixelTileWidth * boardCols));
					//int pixelArrayIndexRight = (xRight + (y * pixelTileWidth * boardCols));
					switch (gameData[i][j]) {
					case 0:
						break;
					case 8: 
						COLOUR_WHITE(pixelArray, pixelArrayIndexLeft); 
						//COLOUR_WHITE(pixelArray, pixelArrayIndexRight);
						break;
					default:
						COLOUR_BLACK(pixelArray, pixelArrayIndexLeft);
						//COLOUR_BLACK(pixelArray, pixelArrayIndexRight);
						break;
					}
				}
				for (int x = j * pixelTileWidth; x < ((j * pixelTileWidth) + (pixelTileWidth)); x += RGBA) {
					int yTop = i * TILE_WIDTH;
					int pixelArrayIndexTop = (x + (yTop * pixelTileWidth * boardCols));
					switch (gameData[i][j]) {
					case 0:
						break;
					case 8:
						COLOUR_WHITE(pixelArray, pixelArrayIndexTop);
						//COLOUR_WHITE(pixelArray, pixelArrayIndexRight);
						break;
					default:
						COLOUR_BLACK(pixelArray, pixelArrayIndexTop);
						//COLOUR_BLACK(pixelArray, pixelArrayIndexRight);
						break;
					}
				}
			}
			else if (gameData[i][j] == ACTIVE) {
				for (int y = (i * TILE_WIDTH); y < ((i * TILE_WIDTH) + TILE_WIDTH); y++) {
					//cout << "j * pixelTileWidth == " << j * pixelTileWidth << endl;
					//cout << "j * pixelTileWidth + pixelTileWidth == " << j * pixelTileWidth + pixelTileWidth << endl;
					for (int x = j * pixelTileWidth; x < ((j * pixelTileWidth) + (pixelTileWidth)); x += RGBA) {
						int pixelArrayIndex = (x + (y * pixelTileWidth * boardCols)); //columnIndex + (rowIndex * total # pixels in one row)
						//(0: line, 1: T, 2: z, 3: z mirror, 4: L, 5: L mirror, 6: square)
						switch (pieceType) {
						case 0: COLOUR_LINE_PIECE(pixelArray, pixelArrayIndex); break;
						case 1: COLOUR_T_PIECE(pixelArray, pixelArrayIndex); break;
						case 2: COLOUR_Z_PIECE(pixelArray, pixelArrayIndex); break;
						case 3: COLOUR_Z_MIRROR_PIECE(pixelArray, pixelArrayIndex); break;
						case 4: COLOUR_L_PIECE(pixelArray, pixelArrayIndex); break;
						case 5: COLOUR_L_MIRROR_PIECE(pixelArray, pixelArrayIndex); break;
						case 6: COLOUR_SQUARE_PIECE(pixelArray, pixelArrayIndex); break;
						default: COLOUR_GHOST(pixelArray, pixelArrayIndex); break;
						}
					}
				}
				for (int y = (i * TILE_WIDTH); y < ((i * TILE_WIDTH) + TILE_WIDTH); y++) {
					int xLeft = j * pixelTileWidth;
					//int xRight = xLeft + pixelTileWidth;
					int pixelArrayIndexLeft = (xLeft + (y * pixelTileWidth * boardCols));
					//int pixelArrayIndexRight = (xRight + (y * pixelTileWidth * boardCols));
					switch (gameData[i][j]) {
					case 0:
						break;
					case 8:
						COLOUR_WHITE(pixelArray, pixelArrayIndexLeft);
						//COLOUR_WHITE(pixelArray, pixelArrayIndexRight);
						break;
					default:
						COLOUR_BLACK(pixelArray, pixelArrayIndexLeft);
						//COLOUR_BLACK(pixelArray, pixelArrayIndexRight);
						break;
					}
				}
				for (int x = j * pixelTileWidth; x < ((j * pixelTileWidth) + (pixelTileWidth)); x += RGBA) {
					int yTop = i * TILE_WIDTH;
					int pixelArrayIndexTop = (x + (yTop * pixelTileWidth * boardCols));
					switch (gameData[i][j]) {
					case 0:
						break;
					case 8:
						COLOUR_WHITE(pixelArray, pixelArrayIndexTop);
						//COLOUR_WHITE(pixelArray, pixelArrayIndexRight);
						break;
					default:
						COLOUR_BLACK(pixelArray, pixelArrayIndexTop);
						//COLOUR_BLACK(pixelArray, pixelArrayIndexRight);
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < boardRows; i++) {
		delete[] gameData[i];
	}
	delete[] gameData;
	return;
}

void createCheckerboard(int boardWidth, int boardHeight, sf::Uint8* pixels) {
	int evenOdd = 0;
	bool startWithDarkTile = true;
	for (int y = 0; y < boardHeight; y++) {
		if (startWithDarkTile) {
			evenOdd = 0;
		}
		else {
			evenOdd = 1;
		}
		for (int x = 0; x < boardWidth * RGBA; x += RGBA) {
			int index = (x + (y * boardWidth * RGBA));
			//int index = tileToPixelTranslator(y, x, boardWidth);
			if ((evenOdd % 2) == 0) {
				pixels[index + 0] = 20;
				pixels[index + 1] = 20;
				pixels[index + 2] = 20;
				pixels[index + 3] = 255;
			}
			else {
				pixels[index + 0] = 25;
				pixels[index + 1] = 25;
				pixels[index + 2] = 25;
				pixels[index + 3] = 255;
			}
			if (x % (TILE_WIDTH * RGBA) == 0) {
				evenOdd++;
			}
		}
		if ((y % TILE_WIDTH) == 0) {
			startWithDarkTile = !startWithDarkTile;
		}
	}
	return;
}

void renderWindow(Board* gameBoard) {
	sf::RenderWindow window(sf::VideoMode(UI_WIDTH, UI_HEIGHT), "SFML works!");
	while (window.isOpen())
	{
		//if (!gameBoard.getPieceInPlay()) {
		//	gameBoard.spawnPiece();
		//	gameBoard.createGhost();
		//}
		int boardWidth = TILE_WIDTH * gameBoard->getMatrixWidth();
		int boardHeight = TILE_WIDTH * gameBoard->getMatrixHeight();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Left:
						gameBoard->moveLeft();
						break;
					case sf::Keyboard::Right:
						gameBoard->moveRight();
						break;
					case sf::Keyboard::Up:
						gameBoard->rotatePiece();
						break;
					case sf::Keyboard::Down:
						if (!gameBoard->boardGravity()) {
							gameBoard->turnInactive();
							gameBoard->clearFullLine();
							cout << "piece can no longer be dropped" << endl;
							cout << "getPieceInPlay == " << gameBoard->getPieceInPlay() << endl;
						}
						break;
					case sf::Keyboard::Space:
						gameBoard->sendToBottom();
						gameBoard->turnInactive();
						gameBoard->clearFullLine();
						break;
					case sf::Keyboard::C:
						gameBoard->savePiece();
						break;
					case sf::Keyboard::Escape:
						window.close();

				}
				gameBoard->createGhost();
			}
		}

		window.clear(sf::Color::Black);
		sf::Texture gameBoardTexture;
		if (!gameBoardTexture.create(boardWidth, boardHeight)) {
			cerr << "gameBoardTexture could not be created" << endl;
		}
		sf::Uint8* boardPixelArray = new sf::Uint8[boardWidth * boardHeight * RGBA];
		createCheckerboard(boardWidth, boardHeight, boardPixelArray);
		updatePixelArray<Board>(gameBoard, boardPixelArray);
		int startOfViewableArea = (boardWidth * RGBA * TILE_WIDTH * NUMBER_OF_ROWS) - (boardWidth * RGBA * 5);
		for (int i = 0; i < startOfViewableArea; i++) {
			boardPixelArray[i] = 0;
		}
		outlineWhiteSquare(boardPixelArray, (NUMBER_OF_ROWS * TILE_WIDTH) - 5, 0, boardWidth * RGBA, boardHeight);
		gameBoardTexture.update(boardPixelArray);
		sf::Sprite gameBoardSprite;
		gameBoardSprite.setPosition(UI_WIDTH/4, 0);
		gameBoardSprite.setTexture(gameBoardTexture);
		//sprite.setColor(sf::Color(0, 255, 0));
		window.draw(gameBoardSprite);
		delete[] boardPixelArray;

		Pieces savedPiece(gameBoard->getSavedPiece());
		int pieceWidth = TILE_WIDTH * savedPiece.getMatrixWidth();
		sf::Texture holdTexture;
		if (!holdTexture.create(pieceWidth, pieceWidth)) {
			cerr << "holdTexture could not be created" << endl;
		}
		sf::Uint8* savedPixelArray = new sf::Uint8[pieceWidth * pieceWidth * RGBA];
		for (int i = 0; i < pieceWidth * pieceWidth * RGBA; i++) {
			savedPixelArray[i] = 0;
		}
		outlineWhiteSquare(savedPixelArray, 0, 0, pieceWidth * RGBA, pieceWidth);
		updatePixelArray<Pieces>(&savedPiece, savedPixelArray);
		holdTexture.update(savedPixelArray);
		sf::Sprite holdSprite;
		holdSprite.setTexture(holdTexture);
		holdSprite.setPosition(5, 20);
		window.draw(holdSprite);
		delete[] savedPixelArray;

		deque<int> pieceQueue = gameBoard->getPieceQueue();
		Pieces pieceOne(pieceQueue.at(1));
		sf::Texture pieceOneTexture;
		if (!pieceOneTexture.create(pieceWidth, pieceWidth)) {
			cerr << "pieceOneTexture could not be created" << endl;
		}
		sf::Uint8* pieceOnePixelArray = new sf::Uint8[pieceWidth * pieceWidth * RGBA];
		for (int i = 0; i < pieceWidth * pieceWidth * RGBA; i++) {
			pieceOnePixelArray[i] = 0;
		}
		outlineWhiteSquare(pieceOnePixelArray, 0, 0, pieceWidth * RGBA, pieceWidth);
		updatePixelArray<Pieces>(&pieceOne, pieceOnePixelArray);
		pieceOneTexture.update(pieceOnePixelArray);
		sf::Sprite pieceOneSprite;
		pieceOneSprite.setTexture(pieceOneTexture);
		pieceOneSprite.setPosition(UI_WIDTH - (UI_WIDTH/4), 20);
		window.draw(pieceOneSprite);
		delete[] pieceOnePixelArray;

		Pieces pieceTwo(pieceQueue.at(2));
		Pieces pieceThree(pieceQueue.at(3));
		Pieces pieceFour(pieceQueue.at(4));
		Pieces pieceFive(pieceQueue.at(5));

		window.display();
		//if (!newGameBoard.boardGravity()) {
		//	newGameBoard.turnInactive();
		//	cout << "piece can no longer be dropped" << endl;
		//	cout << "getPieceInPlay == " << newGameBoard.getPieceInPlay() << endl;
		//}
	}

}