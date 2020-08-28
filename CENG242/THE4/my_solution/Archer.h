#ifndef HW4_ARCHER_H
#define HW4_ARCHER_H

#include"Player.h"

class Archer : public Player {
	/**
	 * Attack damage 50
	 * Heal power 0
	 * Max HP 200
	 * Goal Priorities -> {ATTACK}
	 * Class abbreviation -> "ar" or "AR"
	 * Not able to move at all.
	 * Can attack to a range of 2 squares directly up, down, left or right, from
	 * its coordinate.
	 *
	 */
	int attack_damage;
	int heal_power;
	int max_HP;
	std::string lowerCA = "ar";
	std::string upperCA = "AR";
	std::vector<Goal> goalPriorities;

public:
	Archer(unsigned int id, int x, int y, Team team) : Player(id, x, y, team), attack_damage(50), heal_power(0), max_HP(200)
	{
		goalPriorities.push_back(ATTACK);
		HP = 200;
	}
	int getAttackDamage() const override;
	int getHealPower() const override;
	int getMaxHP() const override;
	std::vector<Goal> getGoalPriorityList();
	const std::string getClassAbbreviation() const override;
	std::vector<Coordinate> getAttackableCoordinates() override;
	std::vector<Coordinate> getHealableCoordinates() override;
	std::vector<Coordinate> getMoveableCoordinates() override;
	
};

#endif
