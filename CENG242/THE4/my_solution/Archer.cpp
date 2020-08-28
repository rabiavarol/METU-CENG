#include"Archer.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

int Archer::getAttackDamage() const
{
	return attack_damage;
}

int Archer::getHealPower() const
{
	return heal_power;
}

int Archer::getMaxHP() const
{
	return max_HP;
}

std::vector<Goal> Archer::getGoalPriorityList()
{
	return goalPriorities;
}

const std::string Archer::getClassAbbreviation() const
{
	if (team == BARBARIANS) return upperCA;
	else return lowerCA;
}


std::vector<Coordinate> Archer::getAttackableCoordinates()
{
	 std::vector<Coordinate> attackables;
	 Coordinate u1(coordinate.x, coordinate.y - 1);
	 Coordinate u2(coordinate.x, coordinate.y - 2);
	 Coordinate d1(coordinate.x, coordinate.y + 1);
	 Coordinate d2(coordinate.x, coordinate.y + 2);
	 Coordinate l1(coordinate.x - 1, coordinate.y);
	 Coordinate l2(coordinate.x - 2, coordinate.y);
	 Coordinate r1(coordinate.x + 1, coordinate.y);
	 Coordinate r2(coordinate.x + 2, coordinate.y);
	 attackables.push_back(u1);
	 attackables.push_back(u2);
	 attackables.push_back(d1);
	 attackables.push_back(d2);
	 attackables.push_back(r1);
	 attackables.push_back(r2);
	 attackables.push_back(l1);
	 attackables.push_back(l2);

	 return attackables;
}

std::vector<Coordinate> Archer::getHealableCoordinates()
{
	return std::vector<Coordinate>();
}

std::vector<Coordinate> Archer::getMoveableCoordinates()
{
	return std::vector<Coordinate>();
}

