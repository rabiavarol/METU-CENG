#include "Race.h"
#include <string>
/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/

#include <algorithm>

Race::Race(std::string race_name) : average_laptime(Utilizer::generateAverageLaptime()), 
									race_name(race_name), 
									head(NULL){}

Race::Race(const Race & rhs)
{
	this->race_name = rhs.race_name;
	this->average_laptime = rhs.average_laptime;
	this->head = NULL;

	Car* rhsCurr = rhs.head->getNext();
	Car* curr = this->head->getNext();
	if (rhs.head) {
		this->head = new Car(*rhs.head);
		while (rhsCurr) {
			curr = new Car(*rhsCurr);
			curr = curr->getNext();
			rhsCurr = rhsCurr->getNext();
		}
		Car* temp = this->head;
		while (temp) {
			temp->deleteAllLaps();
			temp = temp->getNext();
		}
	}
	
}

Race::~Race()
{/*
	while(head){ 
		Car* temp = head->getNext();
		delete head;
		head = temp;
	}*/
}

std::string Race::getRaceName() const
{
	return race_name;
}

void Race::addCartoRace()
{
	std::string name = randomName();
	Car* newCar = new Car(name);
	addCartoRace(*newCar);
}

void Race::addCartoRace(Car & car)
{
	Car* temp = this->head;
	if (temp == NULL) this->head = &car;
	else {
		Car* carNext = temp->getNext();
		while (carNext) {
			temp = temp->getNext();
			carNext = carNext->getNext();
		}
		temp->setNext(&car);
	}

}	

int Race::getNumberOfCarsinRace()
{
	Car* temp = this->head;
	int count = 0;
	while (temp) {
		count++;
		temp = temp->getNext();
	}
	return count;
}

void Race::goBacktoLap(int lap)
{
	Car* temp = this->head;
	if (temp == NULL) return;
	for (int i = currLaptime; i > lap ; i--) {
		--(*this);
	}
}

void Race::operator++()
{
	Car* tempCar = this->head;
	while (tempCar) {
		tempCar->Lap(average_laptime);
		tempCar = tempCar->getNext();	
	}
	sortCars();
	currLaptime++;
}

void Race::operator--()
{
	Car* tempCar = this->head;
	while (tempCar) {
		tempCar->removeTheLastLaptime();
		tempCar = tempCar->getNext();
	}
	sortCars();
	currLaptime--;
}

Car Race::operator[](const int car_in_position)
{
	int index = car_in_position;
	Car* temp = this->head;
	while (temp && index > 0) {
		temp = temp->getNext();
		index--;
	}
	return *temp;
}

Car Race::operator[](std::string driver_name)
{
	Car* temp = this->head;
	while (temp) {
		if (temp->getDriverName() == driver_name) return *temp;
		temp = temp->getNext();
	}
	return *temp; //?
}

Race & Race::operator=(const Race & rhs)
{
	if (this != &rhs) {
		this->race_name = rhs.getRaceName();
		this->average_laptime = rhs.average_laptime;

		while (head) {
			Car* curr = this->head;
			head = head->getNext();
			delete curr;
		}
		Car* temp = this->head->getNext();
		Car* rhsTemp = rhs.head->getNext();
		if (rhs.head) {
			this->head = new Car(*rhs.head);
			while (rhsTemp) {
				temp = new Car(*rhsTemp);
				temp = temp->getNext();
				rhsTemp = rhsTemp->getNext();
			}
			
		}
	}
	return *this;
}

std::string Race::randomName()
{
	static int num = 0;
	std::string n = std::to_string(num);
	num++;
	return "Random " + n + "Name" ;
}

bool carsComparision(Car* first, Car* second) {
	return (*first < *second);
}
void Race::sortCars()
{
	Car* temp = this->head;
	std::vector<Car*> carsInTheRace;
	while(temp){
		carsInTheRace.push_back(temp);
		temp = temp->getNext();
	}
	sort(carsInTheRace.begin(), carsInTheRace.end(), carsComparision);
	int numOfCars = carsInTheRace.size();
	this->head = carsInTheRace[0];
	int i = 0;
	for (; i < numOfCars - 1 ; i++) {
		carsInTheRace[i]->setNext(carsInTheRace[i + 1]);
	}
	carsInTheRace[i]->setNext(NULL);
}



Car* Race::getLastCar() const
{
	Car* temp = this->head;
	if (temp) {
		while (temp->getNext()) {
			temp = temp->getNext();
		}
	}
	return temp;
	
}

Car* Race::getFirstCar() const
{
	return this->head;
}

int Race::getNumOfCars() const
{
	Car* temp = this->head;
	int count = 0;
	while (temp) {
		count++;
		temp = temp->getNext();
	}
	return count;
}

std::ostream & operator<<(std::ostream & os, const Race & race)
{
	int numsOfCars, point=0;
	int extraPoint = 0;
	std::string pos , driverName; 
	numsOfCars = race.getNumOfCars();

	Car* curr = race.head;
	//Laptime* fastest = curr->fastestLaptime();
	if (curr == NULL) {
		os << "0" << "--" << *curr << "--" << point;
		return os;
	}
	int fastest = curr->fastestTime();
	
	while (curr) {
		/*if ((*curr->fastestLaptime()) < (*fastest))
			fastest = curr->fastestLaptime();*/
		if (curr->fastestTime() < fastest)
			fastest = curr->fastestTime();
		curr = curr->getNext();
	}

	Car* temp = race.head;
	for (int i = 1; i <= numsOfCars; i++) {
		
		// For each car
		// First : find position
		if (numsOfCars < 100) {
			if (i < 10) {
				pos = "0" + std::to_string(i);
			}
			else {
				pos = std::to_string(i);
			}
		}
		else if (numsOfCars >= 100 && numsOfCars <= 999) {
			if (i < 10) {
				pos = "00" + std::to_string(i);
			}
			else if (i >= 10 && i <= 99) {
				pos = "0" + std::to_string(i);
			}
			else {
				pos = i;
			}
		}
		else if (numsOfCars >= 1000 && numsOfCars <= 9999) {
			if (i < 10) {
				pos = "000" + std::to_string(i);
			}
			else if (i >= 10 && i <= 99) {
				pos = "00" + std::to_string(i);
			}
			else if (i >= 100 && i <= 999) {
				pos = "0" + std::to_string(i);
			}
			else {
				pos = i;
			}
		}
		
		// Normal racing point
		switch (i) {
			case 1: point = 25; break;
			case 2: point = 18; break;
			case 3: point = 15; break;
			case 4: point = 12; break;
			case 5: point = 10; break;
			case 6: point = 8;  break;
			case 7: point = 6; break;
			case 8: point = 4; break;
			case 9: point =	2; break;
			case 10: point = 1; break;
			default: point = 0;
		}

		// Exstra Point
		if (i <= 10) {
			/*if (temp->fastestLaptime() == fastest) {
				extraPoint = 1;
				os << pos << "--" << *temp << "--" << point << "--" << extraPoint ;
			}*/
			if (temp->fastestTime() == fastest) {
				extraPoint = 1;
				os << pos << "--" << *temp << "--" << point << "--" << extraPoint;
			}
			else {
				os << pos << "--" << *temp << "--" << point;
			}
		}
		else
			os << pos << "--" << *temp << "--" << point;
		os << std::endl;
		temp = temp->getNext();	
	}
	return os;
}

