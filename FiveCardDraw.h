#ifndef fivecarddraw_H
#define fivecarddraw_H

#include "Game.h"

class FiveCardDraw : public Game {

protected:
	//who is dealer
	size_t dealer_;


public:
	//constructor
	FiveCardDraw();

	//turn methods
	virtual int before_turn(Player &);
	virtual int turn(Player &);
	virtual int after_turn(Player &);

	//round methods
	virtual int before_round();
	virtual int round();
	virtual int after_round();

};



#endif /* fivecarddraw_H */