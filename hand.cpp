#include <random>

#include "card.h"
#include "deck.h"
#include "hand.h"


void Hand::reset(std::mt19937& re)
{
	assaults.reset();
	structures.reset();
	deck->shuffle(re);
	commander.set(deck->shuffled_commander);
}