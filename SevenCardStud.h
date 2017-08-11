#ifndef sevencardstud_H
#define sevencardstud_H

#include "Game.h"

class SevenCardStud : public Game {

protected:
	//who is dealer
	size_t dealer_;

public:
	//constructor
	SevenCardStud();

	//turn methods
	virtual int before_turn(Player &);
	virtual int turn(Player &);
	virtual int after_turn(Player &);

	//round methods
	virtual int before_round();
	virtual int round();
	virtual int after_round();

	//dealing and betting pair
	void dealAndBet(unsigned int);

};

#endif /* sevencardstud_H */