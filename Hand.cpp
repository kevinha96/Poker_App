#include "stdafx.h"
#include "Hand.h"
#include "ErrorValues.h"

#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <algorithm>

//default constructor
Hand::Hand() : h_(), pokerH_(pokerHand::noRank) {}

//copy constructor
Hand::Hand(const Hand & h) : h_(h.h_), pokerH_(h.pokerH_) {}

//destructor
Hand::~Hand() {}

//assignment operator
Hand & Hand::operator= (const Hand & h) {
	//check for self assignment
	if (&h != this) {
		//clear hand vector
		h_.clear();

		//place new cards into hand
		for (auto it = h.h_.begin(); it != h.h_.end(); ++it) {
			h_.push_back(*it);
		}
	}
	return *this;
}

//size
size_t Hand::size() const {
	return h_.size();
}

//equivalence operator
bool Hand::operator== (const Hand & h) const {

	//check that the two hands are the same size
	if (h_.size() == h.size()) {
		//now check each card
		for (size_t i = 0; i < h_.size(); ++i) {
			if (h_[i] == h.h_[i]) {}
			else {
				return false;
			}
		}
		//all cards are equivalent
		return true;
	}
	else {
		return false;
	}
}

//less than operator
bool Hand::operator< (const Hand & h) const {

	//check if each hand has five cards
	if (size() == 5 && h.size() == 5) {
		//check each card
		for (int i = 0; i < 4; ++i) {
			if (h_[i] == h.h_[i]) {}
			else {
				return (h_[i] < h.h_[i]);
			}
		}
		//if all cards are equal doesn't matter which goes first
		return true;
	}
	//not five cards
	else {
		throw(ErrorValues::not_five_cards);
	}
}

std::string Hand::asString() const {
	std::string hand;

	for (auto it = h_.begin(); it != h_.end(); ++it) {
		hand += (*it).asString() + " ";
	}
	return hand;
}

std::string Hand::showWholeHand() const {
	std::string hand;
	for (auto it = h_.begin(); it != h_.end(); ++it) {
		hand += (*it).alwaysShowCard() + " ";
	}
	return hand;
}

//determine hand
void Hand::h() {

	//keep track of pairs
	std::vector<Card> count;

	//check if all suits are the same or all ranks are adjacent
	int flush_count = 0;
	int straight_count = 0;

	//vector of five cards
	for (auto vi = h_.begin(); vi != h_.end(); ++vi) {

		if (vi < (h_.end() - 1)) {
			//check suits
			if ((*vi).s() == (*(vi + 1)).s()) {
				++flush_count;
			}

			//check adjacent ranks
			if ((*vi).r() == (*(vi + 1)).r() - 1) {
				++straight_count;
			}
		}

		//count pairs
		if (vi == h_.begin()) {
			if ((*vi).r() == (*(vi + 1)).r()) {
				count.push_back(*vi);
			}
		}
		if (vi == (h_.end() - 1)) {
			if ((*vi).r() == (*(vi - 1)).r()) {
				count.push_back(*vi);
			}
		}
		if (vi == h_.begin() + 1 || vi == h_.begin() + 2 || vi == h_.begin() + 3) {
			if (((*vi).r() == (*(vi + 1)).r()) || (((*vi).r() == (*(vi - 1)).r()))) {
				count.push_back(*vi);
			}
		}
	}

	if (count.size() == 2) {
		pokerH_ = onePair;
	}
	else if (count.size() == 3) {
		pokerH_ = threeOfAKind;
	}
	//either two pair or four of a kind
	else if (count.size() == 4) {
		if (count[1].r() == count[2].r()) {
			pokerH_ = fourOfAKind;
		}
		else {
			pokerH_ = twoPair;
		}
	}
	else if (count.size() == 5) {
		pokerH_ = fullHouse;
	}

	//no pairs mean either flush or straight or nothing
	else if (flush_count == 4) {
		if (straight_count == 4) {
			pokerH_ = straightFlush;
		}
		else {
			pokerH_ = flush;
		}
	}
	else if (straight_count == 4) {
		pokerH_ = straight;
	}
	else {
		pokerH_ = noRank;
	}
}

//indexing operator
Card Hand::operator[] (size_t s) {
	Card temp;

	if (h_.size() >= s + 1) {
		temp = h_[s];
		return temp;
	}
	else {
		throw(ErrorValues::not_Enough_Cards);
	}
}

//remove card
void Hand::remove_card(size_t i) {
	//check if there is a card at that index
	if (h_.size() > i) {
		h_.erase(h_.begin() + i);
	}
	else {
		throw(ErrorValues::invalid_Card_Index);
	}
}

//hand insertion operator
std::ostream & operator<< (std::ostream & os, const Hand & h) {
	os << h.asString() << std::endl;
	return os;
}

//deck to hand insertion operator
void operator<< (Hand & h, Deck & d) {

	//no more cards in deck
	if (d.size() < 1) {
		throw(ErrorValues::not_Enough_Cards);
	}
	else {
		//add last card from deck to hand
		h.h_.push_back(d.d_.back());
		//sort hand
		sort(h.h_.begin(), h.h_.end());
		//remove last card from deck
		d.d_.pop_back();
	}
}

//take face up or down into consideration
void addCardWithFace(Hand & h, Deck & d, bool b) {
	//no more cards in deck
	if (d.size() < 1) {
		throw(ErrorValues::not_Enough_Cards);
	}
	else {
		//set last card in deck to face up or down
		d.d_.back().cardFacingUp_ = b;
		//add last card from deck to hand
		h.h_.push_back(d.d_.back());
		//sort hand
		sort(h.h_.begin(), h.h_.end());
		//remove last card from deck
		d.d_.pop_back();
	}
}

//hand to deck insertion operator
void operator<< (Deck & d, Hand & h) {

	//no more cards in hand
	if (h.size() < 1) {
		throw(ErrorValues::not_Enough_Cards);
	}
	else {
		//add last card from deck to hand
		d.add_card(h.h_.back());
		//remove last card from deck
		h.h_.pop_back();
	}
}



