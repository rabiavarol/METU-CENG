#include"Player.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

Player::Player(unsigned int id, int x, int y, Team team) : id(id), coordinate(x, y), team(team){}

unsigned int Player::getID() const
{
	return id;
}

const Coordinate & Player::getCoord() const
{
	return coordinate;
}

int Player::getHP() const
{
	return HP;
}

Team Player::getTeam() const
{
	return team;
}

std::string Player::getBoardID()
{
	std::string strID = std::to_string(id);
	if (strID.length() == 1) return "0" + strID;
	else return strID;
}


void Player::setHP(int hp)
{
	HP = hp;
}


bool Player::attack(Player * enemy)
{
	std::string aBID = this->getBoardID();
	std::string eBID = enemy->getBoardID();
	int currEnemyHP = enemy->getHP();
	int attackDamageOfAttacker = this->getAttackDamage();
	enemy->setHP(currEnemyHP - attackDamageOfAttacker);
	std::cout << "Player " << aBID << " attacked Player " << eBID << " (" << attackDamageOfAttacker << ")" << std::endl;
	if (enemy->isDead()) return true;
	else return false;
}

void Player::heal(Player * ally)
{
	int newHP = ally->getHP() + this->getHealPower();
	int allyMaxHP = ally->getMaxHP();
	ally->setHP(newHP);
	if (ally->HP > allyMaxHP) ally->setHP(allyMaxHP);
	std::cout << "Player " << this->getBoardID() << " healed Player " << ally->getBoardID() << std::endl;
}


void Player::movePlayerToCoordinate(Coordinate c)
{
	int oldX = coordinate.x;
	int oldY = coordinate.y;
	coordinate = c;
	std::cout << "Player " << this->getBoardID() << " moved from (" << oldX << "/" << oldY << ") to (" << coordinate.x << "/" << coordinate.y << ")" << std::endl;

}
bool Player::isDead() const
{
	if (HP <= 0) return true;
	else return false;
}
