#ifndef game_H
#define game_H

#include "Deck.h"
#include "Player.h"

#include <memory>

class Game {

protected:
	//pointer to game
	static std::shared_ptr<Game> gameP_;

	//Main deck variable
	Deck mainDeck_;

	//discard Deck
	Deck dDeck_;

	//vector of pointers to player in game
	std::vector<std::shared_ptr<Player>> pVector_;

	//vector of pointers in the round
	std::vector<std::shared_ptr<Player>> cpVector_;

	//total chips in the middle
	unsigned int totalChipPot;
	//chips in the middle just during a single betting round
	unsigned int roundChipPot;

	//number of chips carried over form last game if the pot is split into a non integer number
	unsigned int chipCarryOver = 0;

	//current total bet
	unsigned int currentBet;

	//unsigned int number of bets
	unsigned int betCount;

	//position of the player the game is on
	unsigned int currP;

public:
	//get instance of game
	static std::shared_ptr<Game> instance();

	//start the game
	static void start_game(const std::string &);

	//stop the game
	static void stop_game();

	//add a player
	void add_player(const std::string &);

	//find a player
	std::shared_ptr<Player> find_player(const std::string &);

	//destructor
	virtual ~Game();

	//pure virtual turn methods
	virtual int before_turn(Player &) = 0;
	virtual int turn(Player &) = 0;
	virtual int after_turn(Player &) = 0;

	//pure virtual round methods
	virtual int before_round() = 0;
	virtual int round() = 0;
	virtual int after_round() = 0;

	//accessor
	size_t playerCount();

	//rank players
	void rankPlayers(std::vector<std::shared_ptr<Player>> &);

	//leaving players
	int leave();
	//joining players
	int join();

	//ask for bet
	void askBet(Player &);

	//ask players with zero chips to leave
	void askPlayerToLeave();
};
//overload poker_rank
bool new_poker_rank(std::shared_ptr<Player>, std::shared_ptr<Player>);

#endif /* game_H */
