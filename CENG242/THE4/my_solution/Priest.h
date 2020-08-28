#ifndef HW4_PRIEST_H
#define HW4_PRIEST_H

#include"Player.h"

class Priest : public Player {
	/**
	 * Attack damage 0
	 * Heal power 50
	 * Max HP 150
	 * Goal Priorities -> {HEAL,TO_ALLY,CHEST} in decreasing order
	 * Class abbreviation -> "pr" or "PR"
	 * Can move to all adjacent squares, including diagonals.
	 * Can heal all adjacent squares, including diagonals.
	 *
	 */
	int attack_damage;
	int heal_power;
	int max_HP;
	
	std::string lowerCA = "pr";
	std::string upperCA = "PR";
	std::vector<Goal> goalPriorities;

public:
		Priest(unsigned int id, int x, int y, Team team) : Player(id,x,y,team), attack_damage(0), heal_power(50), max_HP(150)
		{
			goalPriorities.push_back(HEAL);
			goalPriorities.push_back(TO_ALLY);
			goalPriorities.push_back(CHEST);
			HP = 150;
		}
		int getAttackDamage() const override;
		int getHealPower() const override;
		int getMaxHP() const override;
		std::vector<Goal> getGoalPriorityList() override;
		const std::string getClassAbbreviation() const override;
		std::vector<Coordinate> getMoveableCoordinates() override;
		std::vector<Coordinate> getHealableCoordinates() override;
		std::vector<Coordinate> getAttackableCoordinates() override;
		

};

#endif

