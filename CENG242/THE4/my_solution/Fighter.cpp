#include"Fighter.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

int Fighter::getAttackDamage() const
{
	return attack_damage;
}

int Fighter::getHealPower() const
{
	return heal_power;
}

int Fighter::getMaxHP() const
{
	return max_HP;
}

std::vector<Goal> Fighter::getGoalPriorityList()
{
	return goalPriorities;
}

const std::string Fighter::getClassAbbreviation() const
{
	if (team == BARBARIANS) return upperCA;
	else return lowerCA;
}

std::vector<Coordinate> Fighter::getAttackableCoordinates()
{
	std::vector<Coordinate> attackables;
	Coordinate left(coordinate.x - 1, coordinate.y);
	Coordinate right(coordinate.x + 1, coordinate.y);
	Coordinate up(coordinate.x, coordinate.y - 1);
	Coordinate down(coordinate.x, coordinate.y + 1);
	attackables.push_back(left);
	attackables.push_back(right);
	attackables.push_back(up);
	attackables.push_back(down);

	return attackables;
}

std::vector<Coordinate> Fighter::getMoveableCoordinates()
{
	std::vector<Coordinate> move;
	Coordinate left(coordinate.x - 1, coordinate.y);
	Coordinate right(coordinate.x + 1, coordinate.y);
	Coordinate up(coordinate.x, coordinate.y - 1);
	Coordinate down(coordinate.x, coordinate.y + 1);
	move.push_back(left);
	move.push_back(right);
	move.push_back(up);
	move.push_back(down);

	return move;
}

std::vector<Coordinate> Fighter::getHealableCoordinates()
{
	return std::vector<Coordinate>();
}

