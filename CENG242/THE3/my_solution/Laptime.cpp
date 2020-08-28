#include "Laptime.h"

/*
YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. START YOUR IMPLEMENTATIONS BELOW THIS LINE
*/


#include<string>
Laptime::Laptime()
{
}

Laptime::Laptime(int laptime)
{
	this->laptime = laptime;
	this->next = NULL;
}

Laptime::Laptime(const Laptime & rhs)
{
	this->laptime = rhs.laptime;
	this->next = NULL;

}

Laptime::~Laptime()
{	
}

void Laptime::addLaptime(Laptime * next)
{
	/*Laptime* temp = this;
	if (temp == NULL) temp = next;
	while (temp->next) {
		temp = temp->next;
	}
	temp->next = next;
	temp->next->next = NULL;*/
	this->next = next;
}

bool Laptime::operator<(const Laptime & rhs) const
{
	if (this->laptime < rhs.laptime) return true;
	else return false;
	
}

bool Laptime::operator>(const Laptime & rhs) const
{
	if (this->laptime > rhs.laptime) return true;
	else return false;
	
}

Laptime & Laptime::operator+(const Laptime & rhs)
{
	this->laptime = this->laptime + rhs.laptime;
	return *this;
}

//nonmember functions
std::string milisecConversion(int laptime)
{
	int miliSec = laptime;
	int second = (miliSec / 1000) % 60;
	int minute = ((miliSec - second) / 1000) / 60;
	miliSec = miliSec - (1000 * second) - (60000 * minute);
	std::string mt = std::to_string(minute);
	std::string sc = std::to_string(second);
	std::string ml = std::to_string(miliSec);

	if (sc.length() == 1) sc = "0" + sc;
	if (ml.length() == 1) ml = "00" + ml;
	if (ml.length() == 2) ml = "0" + ml;

	std::string result = mt + ":" + sc + "." + ml;
	return result;
}

std::ostream & operator<<(std::ostream & os, const Laptime & laptime)
{
	os << milisecConversion(laptime.getLaptime());
	return os;
}

Laptime* Laptime::getNext() const
{
	return this->next;
}

void Laptime::setLaptime(int n)
{
	this->laptime = n;
}

int Laptime::getLaptime() const
{
	return laptime;
}

int Laptime::addNewLaptime(int newLaptime)
{
	return (this->getLaptime() + newLaptime);
}

Laptime* Laptime::getLastLap()  // ?
{
	Laptime* temp = this;
	if (temp) {
		while (temp->getNext()) {
			temp = temp->getNext();
		}
		return temp;
	}
	return temp;
}

void Laptime::setNext(Laptime * n)
{
	this->next = n;
}

