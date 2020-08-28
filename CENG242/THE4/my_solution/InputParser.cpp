#include"InputParser.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Game * InputParser::parseGame()
{
	int boardSize;
	int x, y, n, id;
	int maxTurn;
	
	std::cin >> boardSize;
	std::cin >> x >> y;
	std::cin >> maxTurn;
	std::cin >> n;
	Coordinate _chest(x,y);
	Game* game = new Game(maxTurn, boardSize, _chest);

	for (int i = 0; i < n; i++) {
		int xC, yC ;
		std::string subClass;
		std::string _team;
		std::cin >> id;
		std::cin >> subClass;
		std::cin >> _team;
		std::cin >> xC >> yC;

		if (_team == "BARBARIAN") game->addPlayer(id, xC, yC, BARBARIANS, subClass);
		else game->addPlayer(id, xC, yC, KNIGHTS, subClass);
		
		
	}
	return game;

}
