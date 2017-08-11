#ifndef player_H
#define player_H

#include "Hand.h"
#include <string>

struct Player {

	std::string name_;
	Hand hand_;
	unsigned int handsWon_;
	unsigned int handsLost_;
	unsigned int numChips_;

	//current total bet in game
	unsigned int playerBet_;

	//whether the player is in or out of the game
	bool status;

	//constructor
	Player(const char *);

};

//insertion operator
std::ostream & operator<< (std::ostream &, const Player &);

#endif /* player_H */