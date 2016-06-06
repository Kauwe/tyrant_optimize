#ifndef CARDS_H_INCLUDED
#define CARDS_H_INCLUDED

#include <map>
#include <list>
#include <string>
#include <unordered_set>
#include <vector>
#include "card.h"


//class Card;

class Cards
{
public:

	//public attributes of Cards
    std::vector<Card*> all_cards;
    std::map<unsigned, Card*> cards_by_id;
    std::vector<Card*> player_cards;
    std::map<std::string, Card*> cards_by_name;
    std::vector<Card*> player_commanders;
    std::vector<Card*> player_assaults;
    std::vector<Card*> player_structures;
    std::map<std::string, std::string> player_cards_abbr;
    std::unordered_set<unsigned> visible_cardset;
    std::unordered_set<std::string> ambiguous_names;


public:
	//public methods of Cards

	//destructor
	~Cards();

	const Card * by_id(unsigned id) const;

    void organize();

    void add_card(Card * card, const std::string & name);
};
#endif
