#pragma once
#include <SFML\Graphics.hpp>

void colourPixel(sf::Uint8* pixelArray, int index, int red, int green, int blue, int alpha);

template <class T>
void updatePixelArray(T* gameBoard, sf::Uint8* pixelArray);

void createCheckerboard(int boardWidth, int boardHeight, sf::Uint8* pixels);

void renderWindow(Board* gameBoard);

constexpr int UI_WIDTH = 500;
constexpr int UI_HEIGHT = 570;
constexpr int TILE_WIDTH = 20;

constexpr int RGBA = 4;