#include "stdafx.h"
#include "SevenCardStud.h"
#include "ErrorValues.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

SevenCardStud::SevenCardStud() : dealer_(0) {
	Card newCard;
	for (unsigned int suit = 0; suit < 4; ++suit) {
		for (unsigned int rank = 0; rank < 13; ++rank) {
			newCard.s(suit);
			newCard.r(rank);
			mainDeck_.add_card(newCard);
		}
	}
}

int SevenCardStud::before_turn(Player & p) {
	//print name and hand
	std::cout << "\n" << p.name_ << ": " << p.hand_.showWholeHand() << std::endl;
	
	//ask which cards to discard
	std::string input;
	std::string word;
	std::vector<std::string> posV;
	std::cout << "Which two cards would you like to discard? (Enter position of card from 1 - 7)" << std::endl;
	getline(std::cin, input);
	std::stringstream iss(input);
	while (iss >> word) {
		posV.push_back(word);
	}

	//check posV to see if it has correct number
	if (posV.size() == 2) {

		//check to see if inputs are numbers
		for (auto vit = posV.begin(); vit != posV.end(); ++vit) {
			try { stoi(*vit); }
			catch (...) { throw(ErrorValues::invalid_Response); }
		}

		//sort backwards so iterators don't get messed up
		std::sort(posV.rbegin(), posV.rend());

		//delete from hand and add to discard deck
		for (auto vit = posV.begin(); vit != posV.end(); ++vit) {
			size_t removePos = stoi(*vit) - 1;

			//check if number is within range
			if (removePos >= 0 && removePos < p.hand_.size()) {
				Card disCard = p.hand_[removePos];
				p.hand_.remove_card(removePos);
			}
			else {
				throw(ErrorValues::invalid_Response);
			}
		}
	}
	else {
		std::vector<std::string>().swap(posV);
		throw(ErrorValues::invalid_Response);
	}
	return 1; 
}

int SevenCardStud::turn(Player & p) { 
	// no need to deal out cards
	return 0; 
}
int SevenCardStud::after_turn(Player & p) {
	//print name and hand
	std::cout << "\n" << p.name_ << ": " << p.hand_ << std::endl;
	return 0;
}

int SevenCardStud::before_round() {
	ErrorValues error;

	//reset betting sums
	currentBet = 0;
	betCount = 0;

	//reset chip pots
	totalChipPot = chipCarryOver;
	roundChipPot = 0;

	mainDeck_.shuffleDeck();

	//everyone gets 3 cards, 2 face down 1 face up
	currP = dealer_ + 1;
	for (unsigned int i = 0; i < 3; ++i) {
		for (unsigned int j = 0; j < pVector_.size(); ++j) {
			//end of vector
			if (currP >= pVector_.size()) {
				currP = 0;
			}


			//make two of the cards face down for 2 of the cards
			if (i < 2) {
				//add card to player's hand
				try { addCardWithFace(pVector_[currP]->hand_, mainDeck_, false); }
				catch (ErrorValues::Error err) { throw(err); }

				if (i == 0) {
					//take one chip from each player
					pVector_[currP]->numChips_ -= 1;
					totalChipPot += 1;

					//set current round players
					cpVector_.push_back(pVector_[currP]);
				}
			}
			else {
				//add card to player's hand
				try { addCardWithFace(pVector_[currP]->hand_, mainDeck_, true); }
				catch (ErrorValues::Error err) { throw(err); }
			}
			++currP;
		}
	}
	
	//first round of betting
	while (betCount != cpVector_.size()) {
		//end of vector
		if (currP >= cpVector_.size()) {
			currP = 0;
		}

		while (true) {
			try { 
				//print out hand
				std::cout << "\n" << cpVector_[currP]->name_ << ": " << cpVector_[currP]->hand_;
				askBet(*(cpVector_[currP]));
				break; 
			}
			catch (ErrorValues::Error err) {
				error.p(err);
			}
		}
	}

	//if everyone folded out except one
	if (cpVector_.size() == 1) {
		after_round();
	}

	//the remaining turns to deal and bet
	for (unsigned int i = 0; i < 4; ++i) {
		dealAndBet(i);
	}

	//sum up total chip count
	totalChipPot += roundChipPot;

	//reset playerBet after betting round
	for (auto pvIT = pVector_.begin(); pvIT != pVector_.end(); ++pvIT) {
		(*pvIT)->playerBet_ = 0;
	}

	//execute before_turn method for each player
	for (unsigned int j = 0; j < cpVector_.size(); ++j) {
		//end of vector
		if (currP >= cpVector_.size()) {
			currP = 0;
		}

		while (true) {
			try { before_turn(*(cpVector_[currP])); break; }
			catch (ErrorValues::Error err) {
				error.p(err);
			}
		}

		++currP;
	}

	return 0;
}

int SevenCardStud::round() { 
	//no need for betting nor refilling cards
	return 0; 
}
int SevenCardStud::after_round() { 
	//temporary vector
	std::vector<std::shared_ptr<Player>> temp(cpVector_);

	//rank and print player info
	rankPlayers(temp);

	//move all cards back
	while (dDeck_.size() != 0) {
		mainDeck_ << dDeck_;
	}
	for (int i = 0; i < 5; ++i) {
		for (auto vit = pVector_.begin(); vit != pVector_.end(); ++vit) {
			mainDeck_ << (*vit)->hand_;
		}
	}

	//ask players with 0 chips to leave
	askPlayerToLeave();

	//leaving players
	leave();

	//joining players
	join();

	//increment dealer
	if (dealer_ == pVector_.size() - 1) {
		dealer_ = 0;
	}
	else {
		++dealer_;
	}

	//clear cpVector
	cpVector_.clear();

	return 0;
}

//dealing and betting
void SevenCardStud::dealAndBet(unsigned int turn) {
	ErrorValues error;

	for (unsigned int j = 0; j < cpVector_.size(); ++j) {
		//end of vector
		if (currP >= cpVector_.size()) {
			currP = 0;
		}

		//add faced up card
		if (turn < 3) {
			try { addCardWithFace(cpVector_[currP]->hand_, mainDeck_, true); }
			catch (ErrorValues::Error err) { throw(err); }
		}
		//add faced down card
		else {
			try { addCardWithFace(cpVector_[currP]->hand_, mainDeck_, false); }
			catch (ErrorValues::Error err) { throw(err); }
		}

		++currP;
	}

	//reset sums for new betting round
	totalChipPot += roundChipPot;
	betCount = 0;
	currentBet = 0;
	roundChipPot = 0;

	//reset playerBet after betting round
	for (auto pvIT = pVector_.begin(); pvIT != pVector_.end(); ++pvIT) {
		(*pvIT)->playerBet_ = 0;
	}

	//betting
	while (betCount != cpVector_.size()) {
		//end of vector
		if (currP >= cpVector_.size()) {
			currP = 0;
		}
		while (true) {
			try {
				//print out hand
				std::cout << "\n" << cpVector_[currP]->name_ << ": " << cpVector_[currP]->hand_;
				askBet(*(cpVector_[currP]));
				break;
			}
			catch (ErrorValues::Error err) {
				error.p(err);
			}
		}
	}

	//if everyone folded out except one
	if (cpVector_.size() == 1) {
		after_round();
	}
}