#include"Board.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Board::Board(unsigned int _size, std::vector<Player*>* _players, Coordinate chest) : size(_size),chest(chest), players(_players)
{
}



Board::~Board()
{
}

bool Board::isCoordinateInBoard(const Coordinate & c)
{
	if (0 <= c.x && c.x < size && 0 <= c.y && c.y < size) return true;
	return false;
}

bool Board::isPlayerOnCoordinate(const Coordinate & c)
{
	int sizeOfPlayers = players->size();
	for (int i = 0; i < sizeOfPlayers; i++) {
		if (players->at(i)->getCoord() == c) return true;
	}
	return false;
}

Player * Board::operator[](const Coordinate & c)
{
	int sizeOfPlayers = players->size();
	for (int i = 0; i < sizeOfPlayers; i++) {
		if (players->at(i)->getCoord() == c) return players->at(i);
	}
	return NULL;
}

Coordinate Board::getChestCoordinates()
{
	return chest;
}

void Board::printBoardwithID()
{
	int numOfPlayers = players->size();

	for (int i = 0; i < size; i++) { // y-axis
		for (int j = 0; j < size; j++) { // x-axis
			Coordinate square(j, i);
			if (this->operator[](square) && square == chest) {
				std::cout << this->operator[](square)->getBoardID() << " ";
			}
			if (this->operator[](square) && square != chest) {
				std::cout << this->operator[](square)->getBoardID() << " ";
			}
			if (!this->operator[](square) && square == chest) {
				std::cout << "Ch " ;
			}
			if (!this->operator[](square) && square != chest) {
				std::cout << "__ ";
			}
		}
		std::cout << std::endl;
	}
}

void Board::printBoardwithClass()
{
	for (int i = 0; i < size; i++) { // y-axis
		for (int j = 0; j < size; j++) { // x-axis
			Coordinate square(j, i);
			if (this->operator[](square) && square == chest) {
				std::cout << this->operator[](square)->getClassAbbreviation() << " ";
			}
			if (this->operator[](square) && square != chest) {
				std::cout << this->operator[](square)->getClassAbbreviation() << " " ;
			}
			if (!this->operator[](square) && square == chest) {
				std::cout << "Ch ";
			}
			if (!this->operator[](square) && square != chest) {
				std::cout << "__ ";
			}
		}
		std::cout << std::endl;
	}
}

int Board::getBoardSize()
{
	return size;
}

