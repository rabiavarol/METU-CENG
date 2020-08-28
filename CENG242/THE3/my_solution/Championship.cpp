#include "Championship.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/
#include <string>
Championship::Championship()
{
}

Championship::Championship(const Championship & rhs)
{
	this->races.clear();
	int numofRaces = rhs.races.size();
	for (int i = 0; i < numofRaces; i++) {
		this->races.push_back(new Race(*rhs.races[i]));
	}
}

Championship::~Championship()
{
	for (int i = 0; i < races.size(); ++ i)
	{
		delete races[i];
	}

	races.clear();
}

void Championship::addNewRace(Race & race)
{
	
}

void Championship::addNewRace(std::string race_name)
{

}

void Championship::removeRace(std::string race_name)
{
}

void Championship::addLap(std::string race_name)
{
}

Race Championship::operator[](std::string race_name)
{
	Race x = Race("spain");
	return x;
}

std::ostream & operator<<(std::ostream & os, const Championship & championship)
{
	os << std::endl;
	return os;
}
