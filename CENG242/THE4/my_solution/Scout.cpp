#include"Scout.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

int Scout::getAttackDamage() const
{
	return attack_damage;
	
}

int Scout::getHealPower() const
{
	return heal_power;
}

int Scout::getMaxHP() const
{
	return max_HP;
}

std::vector<Goal> Scout::getGoalPriorityList()
{
	return goalPriorities;
}

const std::string Scout::getClassAbbreviation() const
{
	if (team == BARBARIANS) return upperCA;
	else return lowerCA;
}


std::vector<Coordinate> Scout::getAttackableCoordinates()
{
	std::vector<Coordinate> attackables;
	Coordinate left(coordinate.x - 1, coordinate.y);
	Coordinate right(coordinate.x + 1, coordinate.y);
	Coordinate up(coordinate.x, coordinate.y - 1);
	Coordinate down(coordinate.x, coordinate.y + 1);
	Coordinate topLeft(coordinate.x - 1, coordinate.y - 1);
	Coordinate bottomLeft(coordinate.x - 1, coordinate.y + 1);
	Coordinate topRight(coordinate.x + 1, coordinate.y - 1);
	Coordinate bottomRight(coordinate.x + 1, coordinate.y + 1);
	attackables.push_back(left);
	attackables.push_back(right);
	attackables.push_back(up);
	attackables.push_back(down);
	attackables.push_back(topLeft);
	attackables.push_back(bottomLeft);
	attackables.push_back(topRight);
	attackables.push_back(bottomRight);
	return attackables;
}

std::vector<Coordinate> Scout::getMoveableCoordinates()
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

std::vector<Coordinate> Scout::getHealableCoordinates()
{
	return std::vector<Coordinate>();
}

