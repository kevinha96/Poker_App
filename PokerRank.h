#ifndef pokerrank_H
#define pokerrank_H

#include "Hand.h"

bool poker_rank(Hand &, Hand &);

//find cards with no pair in hand
std::vector<int> notPair(const Hand &);

#endif /* pokerrank_H */