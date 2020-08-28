#include"Priest.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

int Priest::getAttackDamage() const
{
	return attack_damage;
}

int Priest::getHealPower() const
{
	return heal_power;
}

int Priest::getMaxHP() const
{
	return max_HP;
}


std::vector<Goal> Priest::getGoalPriorityList()
{
	return goalPriorities;
}

const std::string Priest::getClassAbbreviation() const
{
	if (team == BARBARIANS) return upperCA;
	else return lowerCA;
}


std::vector<Coordinate> Priest::getMoveableCoordinates()
{
	std::vector<Coordinate> move;
	Coordinate left(coordinate.x - 1, coordinate.y);
	Coordinate right(coordinate.x + 1, coordinate.y);
	Coordinate up(coordinate.x, coordinate.y - 1);
	Coordinate down(coordinate.x, coordinate.y + 1);
	Coordinate topLeft(coordinate.x - 1, coordinate.y - 1);
	Coordinate bottomLeft(coordinate.x - 1, coordinate.y + 1);
	Coordinate topRight(coordinate.x + 1, coordinate.y - 1);
	Coordinate bottomRight(coordinate.x + 1, coordinate.y + 1);
	move.push_back(left);
	move.push_back(right);
	move.push_back(up);
	move.push_back(down);
	move.push_back(topLeft);
	move.push_back(bottomLeft);
	move.push_back(topRight);
	move.push_back(bottomRight);
	return move;
}

std::vector<Coordinate> Priest::getHealableCoordinates()
{
	std::vector<Coordinate> heal;
	Coordinate left(coordinate.x - 1, coordinate.y);
	Coordinate right(coordinate.x + 1, coordinate.y);
	Coordinate up(coordinate.x, coordinate.y - 1);
	Coordinate down(coordinate.x, coordinate.y + 1);
	Coordinate topLeft(coordinate.x - 1, coordinate.y - 1);
	Coordinate bottomLeft(coordinate.x - 1, coordinate.y + 1);
	Coordinate topRight(coordinate.x + 1, coordinate.y - 1);
	Coordinate bottomRight(coordinate.x + 1, coordinate.y + 1);
	heal.push_back(left);
	heal.push_back(right);
	heal.push_back(up);
	heal.push_back(down);
	heal.push_back(topLeft);
	heal.push_back(bottomLeft);
	heal.push_back(topRight);
	heal.push_back(bottomRight);
	return heal;
}

std::vector<Coordinate> Priest::getAttackableCoordinates()
{
	return std::vector<Coordinate>();
}
