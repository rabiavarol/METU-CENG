#ifndef HW4_FIGHTER_H
#define HW4_FIGHTER_H

#include"Player.h"

class Fighter : public Player {
	/**
	 * Attack damage 100
	 * Heal power 0
	 * Max HP 400
	 * Goal Priorities -> {ATTACK,TO_ENEMY,CHEST} in decreasing order
	 * Class abbreviation -> "fi" or "FI"
	 * Can move to adjacent up, down, left or right square
	 * Can attack to adjacent up, down, left or right square
	 *
	 */
	int attack_damage;
	int heal_power;
	int max_HP;
	std::string lowerCA = "fi";
	std::string upperCA = "FI";
	std::vector<Goal> goalPriorities;

public:
	Fighter(unsigned int id, int x, int y, Team team) : Player(id, x, y, team), attack_damage(100), heal_power(0), max_HP(400)
	{
		goalPriorities.push_back(ATTACK);
		goalPriorities.push_back(TO_ENEMY);
		goalPriorities.push_back(CHEST);

		HP = 400;
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

