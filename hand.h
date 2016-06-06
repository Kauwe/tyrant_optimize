#ifndef HAND_H_INCLUDED
#define HAND_H_INCLUDED

#include "global.h"

class CardStatus;
class Deck;

//------------------------------------------------------------------------------
// Represents a particular draw from a deck.
// Persistent object: call reset to get a new draw.
class Hand
{
public:

	Hand(Deck* deck_) :
		deck(deck_),
		assaults(15),
		structures(15)
	{
	}

	void reset(std::mt19937& re);

	Deck* deck;
	CardStatus commander;
	Storage<CardStatus> assaults;
	Storage<CardStatus> structures;
};

#endif