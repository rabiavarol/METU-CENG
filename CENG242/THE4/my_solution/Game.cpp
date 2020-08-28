#include"Game.h"
#include<algorithm>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Game::Game(unsigned int maxTurnNumber, unsigned int boardSize, Coordinate chest) : 
			board(boardSize,&players,chest), maxTurnNumber(maxTurnNumber), turnNumber(0)
{
}

Game::~Game()
{
	for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		delete (*it);
	}
	players.clear();
}

void Game::addPlayer(int id, int x, int y, Team team, std::string cls)
{
	if (cls == "ARCHER") {
		players.push_back(new Archer(id, x, y, team));
	}
	if (cls == "FIGHTER") {
		players.push_back(new Fighter(id, x, y, team));
	}
	if (cls == "PRIEST") {
		players.push_back(new Priest(id, x, y, team));
	}
	if (cls == "SCOUT") {
		players.push_back(new Scout(id, x, y, team));
	}
	if (cls == "TANK") {
		players.push_back(new Tank(id, x, y, team));
	}
}

bool Game::isGameEnded()
{
	int numOfPlayer = players.size();
	int numOfBarbars = 0;
	int numOfKnights = 0;
	

	if (turnNumber > maxTurnNumber) {
		std::cout << "Game ended at turn " << turnNumber << ". Maximum turn number reached. Knight victory." << std::endl;
		return true;
	}

	for (int i = 0; i < numOfPlayer; i++) {

		if (players[i]->getTeam() == BARBARIANS) {
			numOfBarbars++;
			if (players[i]->getCoord() == board.getChestCoordinates()) {
				std::cout << "Game ended at turn " << turnNumber << ". Chest captured. Barbarian victory." << std::endl;
				return true;
			}
		}
		else numOfKnights++;
	}
	if (numOfBarbars == 0) {
		std::cout << "Game ended at turn " << turnNumber << ". All barbarians dead. Knight victory." << std::endl;
		return true;
	}
	if (numOfKnights == 0) {
		std::cout << "Game ended at turn " << turnNumber << ". All knights dead. Barbarian victory." << std::endl;
		return true;
	}
	return false;
}

bool idComp(Player* p1, Player* p2) {
	return (p1->getBoardID()) < (p2->getBoardID());
}



void Game::playTurn()
{
	turnNumber++;
	std::cout << "Turn " << turnNumber << " has started." << std::endl;
	std::sort(players.begin(), players.end(), idComp);
	for (int i = 0; i < players.size();) {
		if (players[i]->isDead() == true) {
			playTurnForPlayer(players[i]);
			continue;
		}
		else {
			playTurnForPlayer(players[i]);
			i++;
		}
	}

}

Goal Game::playTurnForPlayer(Player * player)
{
	if (player->isDead()) { 
		std::cout << "Player " << player->getBoardID() << " has died." << std::endl;
		std::vector<Player*>::iterator it;
		delete player;
		it = std::find(players.begin(), players.end(), player);
		players.erase(it);
		return NO_GOAL;
	}
	std::vector<Player*> enemies;
	std::vector<Player*> allies;
	
	for (int i = 0; i < players.size(); i++) {
		if (players[i] != player && players[i]->getTeam() == player->getTeam())
			allies.push_back(players[i]);
		if (players[i] != player && players[i]->getTeam() != player->getTeam())
			enemies.push_back(players[i]);
	}
	int numOfGoals = player->getGoalPriorityList().size();
	for (int i = 0; i < numOfGoals; i++) {
		if (player->getGoalPriorityList()[i] == ATTACK) {
			std::vector<Player*> enemiesToAttack = playersInTheCoordinates(enemies, player->getAttackableCoordinates());
			if (enemiesToAttack.size() == 0) continue;
			if (enemiesToAttack.size() > 0) {
				std::sort(enemiesToAttack.begin(), enemiesToAttack.end(), idComp);
				player->attack(enemiesToAttack[0]);
				return ATTACK;
			}
	
		}
		if (player->getGoalPriorityList()[i] == HEAL) { 
			std::vector<Player*> alliesToHeal = playersInTheCoordinates(allies, player->getHealableCoordinates());
			if (alliesToHeal.size() == 0) continue;
			else {
				for (unsigned i = 0; i < alliesToHeal.size(); i++)
					player->heal(alliesToHeal[i]);
				return HEAL;
			}
		}
		if (player->getGoalPriorityList()[i] == TO_ALLY) { 
		
			std::vector<Coordinate> moveables = player->getMoveableCoordinates();
			std::vector<Player*> nearestAlliesToMove;

			if (enemies.size() == 0) continue;
			int nearest = player->getCoord() - allies[0]->getCoord();
			for (unsigned i = 1; i < allies.size(); i++) {
				if (player->getCoord() - allies[i]->getCoord() < nearest)
					nearest = player->getCoord() - allies[i]->getCoord();
			}
			for (unsigned i = 0; i < allies.size(); i++) {
				if (player->getCoord() - allies[i]->getCoord() == nearest)
					nearestAlliesToMove.push_back(allies[i]);
			}
			if (nearestAlliesToMove.size() > 1) {
				std::sort(nearestAlliesToMove.begin(),nearestAlliesToMove.end(),idComp);
			}
			Coordinate goal(-1, -1);
			int dist;
			int minDist = player->getCoord() - nearestAlliesToMove[0]->getCoord();
			int prevDist = minDist;
			for (unsigned i = 0; i < moveables.size(); i++) {
				dist = moveables[i] - nearestAlliesToMove[0]->getCoord();
				if (dist < minDist && !board.isPlayerOnCoordinate(moveables[i])) {
					goal = moveables[i];
					minDist = dist;
				}
				else if (dist == minDist && !board.isPlayerOnCoordinate(moveables[i]) && minDist != prevDist) {
					if (isHorizontal(player->getCoord(), moveables[i])) {
						goal = moveables[i];
					}
				}
			}
			if (board.isCoordinateInBoard(goal)) {
				player->movePlayerToCoordinate(goal);
				return TO_ALLY;
			}
			continue;	
		}
		if (player->getGoalPriorityList()[i] == TO_ENEMY) {
			std::vector<Coordinate> moveables = player->getMoveableCoordinates();
			std::vector<Player*> nearestEnemiesToMove;
			if (enemies.size() == 0) continue;
			int nearest = player->getCoord() - enemies[0]->getCoord();
			for (unsigned i = 1; i < enemies.size(); i++) {
				if (player->getCoord() - enemies[i]->getCoord() < nearest)
					nearest = player->getCoord() - enemies[i]->getCoord();
			}
			for (unsigned i = 0; i < enemies.size(); i++) {
				if (player->getCoord() - enemies[i]->getCoord() == nearest)
					nearestEnemiesToMove.push_back(enemies[i]);
			}
			if (nearestEnemiesToMove.size() > 0) {
				std::sort(nearestEnemiesToMove.begin(),nearestEnemiesToMove.end(),idComp);
			}
			Coordinate goal(-1, -1);
			int dist;
			if (nearestEnemiesToMove.size() == 0) continue;
			int minDist = player->getCoord() - nearestEnemiesToMove[0]->getCoord();
			for (unsigned i = 0; i < moveables.size(); i++) {
				dist = moveables[i] - nearestEnemiesToMove[0]->getCoord();
				if (dist < minDist && !board.isPlayerOnCoordinate(moveables[i])) {
					goal = moveables[i];
					minDist = dist;
				}
				else if (dist == minDist && !board.isPlayerOnCoordinate(moveables[i])) {
					if (isHorizontal(player->getCoord(), moveables[i])) {
						goal = moveables[i];
					}
				}
			}
			if (board.isCoordinateInBoard(goal)) {
				player->movePlayerToCoordinate(goal);
				return TO_ENEMY;
			}
			continue;
		}
		if (player->getGoalPriorityList()[i] == CHEST) {
			std::vector<Coordinate> moveables = player->getMoveableCoordinates();
			Coordinate goal(-1, -1);
			int dist;
			int minDist = player->getCoord() - board.getChestCoordinates();
			for (unsigned i = 0; i < moveables.size(); i++) {
				dist = moveables[i] - board.getChestCoordinates();
				if (dist < minDist && !board.isPlayerOnCoordinate(moveables[i])) {
					goal = moveables[i];
					minDist = dist;
				}
				else if (dist == minDist && !board.isPlayerOnCoordinate(moveables[i])) {
					if (isHorizontal(player->getCoord(), moveables[i])) {
						goal = moveables[i];
					}
				}
			}
			if (board.isCoordinateInBoard(goal)) {
				player->movePlayerToCoordinate(goal);
				return CHEST;
			}
			continue;
		}
	}	
	return NO_GOAL;
}

std::vector<Player*> Game::playersInTheCoordinates(std::vector<Player*> _players, std::vector<Coordinate> attackCoord) {
	std::vector<Player*> playersInCoord;
	for (unsigned i = 0; i < _players.size(); i++) {
		for (unsigned j = 0; j < attackCoord.size(); j++) {
			if (_players[i]->getCoord() == attackCoord[j])
				playersInCoord.push_back(_players[i]);
		}
	}
	return playersInCoord;
}

bool Game::isHorizontal(Coordinate _player, Coordinate _other) {
	if (_player.y == _other.y) return true;
	return false;
}