#include "stdafx.h"
#include "PokerRank.h"
#include "Card.h"
#include "ErrorValues.h"

bool poker_rank(Hand & a, Hand & b) {
	//5 cards
	if (a.size() == 5 & b.size() == 5) {
		//detemine hand
		a.h();
		b.h();

		//nonpaired card locations
		std::vector<int> aLoc = notPair(a);
		std::vector<int> bLoc = notPair(b);
		//same hands
		if (b.pokerH_ == a.pokerH_) {
			//straight flush
			if (b.pokerH_ == Hand::pokerHand::straightFlush) {
				return b.h_[4] < a.h_[4];
			}
			//four of a kind
			if (b.pokerH_ == Hand::pokerHand::fourOfAKind) {
				return b.h_[2].r_ < a.h_[2].r_;
			}
			//full house
			if (b.pokerH_ == Hand::pokerHand::fullHouse) {
				//same triple
				if (b.h_[2].r_ == a.h_[2].r_) {
					//triple cards are in first 3 cards
					if (b.h_[0].r_ == a.h_[0].r_) {
						return b.h_[4] < a.h_[4];
					}
					//triple cards are in last 3
					else if (b.h_[4].r_ == a.h_[4].r_) {
						return b.h_[4] < a.h_[4];
					}
					//a has in first 3; b in last 3
					else if (b.h_[4].r_ == a.h_[0].r_) {
						return true;
					}
					//a has in last 3; b in first 3
					else {
						return false;
					}
				}
				else {
					return b.h_[2] < a.h_[2];
				}
			}
			//flush
			if (b.pokerH_ == Hand::pokerHand::flush) {
				//sort by highest rank
				for (int i = 4; i >= 0; --i) {
					if (b.h_[i].r_ != a.h_[i].r_) {
						return b.h_[i].r_ < a.h_[i].r_;
					}
				}
				return false;
			}
			//straight
			if (b.pokerH_ == Hand::pokerHand::straight) {
				return b.h_[4] < a.h_[4];
			}
			//three of a kind
			if (b.pokerH_ == Hand::pokerHand::threeOfAKind) {
				return b.h_[2].r_ < a.h_[2].r_;
			}
			//two pair
			if (b.pokerH_ == Hand::pokerHand::twoPair) {
				//check higher pair
				if (b.h_[3].r_ == a.h_[3].r_) {
					//check lower pair
					if (b.h_[1].r_ == a.h_[1].r_) {
						//compare last card
						return b.h_[bLoc[0]].r_ < a.h_[aLoc[0]].r_;
					}
					else {
						return b.h_[1].r_ < a.h_[1].r_;
					}
				}
				else {
					return b.h_[3].r_ < a.h_[3].r_;
				}
			}
			//one pair
			if (b.pokerH_ == Hand::pokerHand::onePair) {
				//find paired cards 
				int aPair = 0;
				int bPair = 0;
				int aSum = 0;
				int bSum = 0;
				//find sum
				for (int i = 0; i < 3; ++i) {
					aSum += aLoc[i];
					bSum += bLoc[i];
				}
				//subtract total of 0,1,2,3,4 and subtract sum from it
				aPair = (9 - aSum) / 2;
				bPair = (9 - bSum) / 2;
				//pair ranks are equal
				if (b.h_[bPair].r_ == a.h_[aPair].r_) {
					//check single cards
					for (int i = 2; i >= 0; --i) {
						if (b.h_[bLoc[i]].r_ != a.h_[aLoc[i]].r_) {
							return b.h_[bLoc[i]].r_ < a.h_[aLoc[i]].r_;
						}
					}
					return false;
				}
				else {
					return b.h_[bPair].r_ < a.h_[aPair].r_;
				}
			}
			//unranked
			if (b.pokerH_ == Hand::pokerHand::noRank) {
				for (int i = 4; i >= 0; --i) {
					if (b.h_[i].r_ != a.h_[i].r_) {
						return b.h_[i].r_ < a.h_[i].r_;
					}
				}
				return false;
			}
		}
		else {
			return b.pokerH_ < a.pokerH_;
		}
	}
	//not five cards
	else {
		throw(ErrorValues::not_five_cards);
	}
}

std::vector<int> notPair(const Hand & h) {
	std::vector<int> single;
	//first card
	if (h.h_[0].r_ != h.h_[1].r_) {
		single.push_back(0);
	}
	//middle cards
	for (int i = 1; i < 4; ++i) {
		if (h.h_[i].r_ != h.h_[i + 1].r_
			&& h.h_[i].r_ != h.h_[i - 1].r_) {
			single.push_back(i);
		}
	}
	//last card
	if (h.h_[4].r_ != h.h_[3].r_) {
		single.push_back(4);
	}
	return single;
}