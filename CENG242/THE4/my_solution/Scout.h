#ifndef HW4_SCOUT_H
#define HW4_SCOUT_H

#include"Player.h"

class Scout : public Player {
	/**
	 * Attack damage 25
	 * Heal power 0
	 * Max HP 125
	 * Goal Priorities -> {CHEST,TO_ALLY,ATTACK} in decreasing order
	 * Class abbreviation -> "sc" or "SC"
	 * Can move to all adjacent squares, including diagonals.
	 * Can attack all adjacent squares, including diagonals.
	 *
	 */

	int attack_damage;
	int heal_power;
	int max_HP;
	std::string lowerCA = "sc";
	std::string upperCA = "SC";
	std::vector<Goal> goalPriorities;

public:
	Scout(unsigned int id, int x, int y, Team team) : Player(id, x, y, team), attack_damage(25), heal_power(0), max_HP(125)
	{
		goalPriorities.push_back(CHEST);
		goalPriorities.push_back(TO_ALLY);
		goalPriorities.push_back(ATTACK);
		HP = 125;
	}
	 int getAttackDamage() const override;
	 int getHealPower() const override;
	 int getMaxHP() const override;
	 std::vector<Goal> getGoalPriorityList() override;
	 const std::string getClassAbbreviation() const override;
	 std::vector<Coordinate> getAttackableCoordinates() override;
	 std::vector<Coordinate> getMoveableCoordinates() override;
	 std::vector<Coordinate> getHealableCoordinates() override;
	 
	 

};

#endif
