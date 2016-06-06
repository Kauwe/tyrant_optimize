#ifndef DECK_H_INCLUDED
#define DECK_H_INCLUDED

#include <deque>
#include <list>
#include <map>
#include <random>
#include <set>
#include <vector>
#include "card.h"
#include "cards.h"
#include "global.h"

//---------------------- $30 Deck: a commander + a sequence of cards -----------
// Can be shuffled.
// Implementations: random player and raid decks, ordered player decks.
//------------------------------------------------------------------------------
typedef void (*DeckDecoder)(const char* hash, std::vector<unsigned>& ids);
typedef void (*DeckEncoder)(std::stringstream &ios, const Card* commander, std::vector<const Card*> cards);
typedef std::map<std::string, long double> DeckList;

extern DeckDecoder hash_to_ids;
extern DeckEncoder encode_deck;

//------------------------------------------------------------------------------
// No support for ordered raid decks
class Deck
{
public:
    const Cards& all_cards;
    DeckType::DeckType decktype;
    unsigned id;
    std::string name;
    unsigned upgrade_points;
    unsigned upgrade_opportunities;
    DeckStrategy::DeckStrategy strategy;

    const Card* commander;
    unsigned commander_max_level;
    std::vector<const Card*> cards;
    std::map<signed, char> card_marks;  // <positions of card, prefix mark>: -1 indicating the commander. E.g, used as a mark to be kept in attacking deck when optimizing.

    const Card* shuffled_commander;
    std::deque<const Card*> shuffled_forts;
    std::deque<const Card*> shuffled_cards;

    // card id -> card order
    std::map<unsigned, std::list<unsigned>> order;
    std::vector<std::tuple<unsigned, unsigned, std::vector<const Card*>>> variable_cards;  // amount, replicates, card pool
    unsigned deck_size;
    unsigned mission_req;

    std::string deck_string;
    std::set<unsigned> vip_cards;
    std::set<unsigned> allowed_candidates;
    std::set<unsigned> disallowed_candidates;
    std::vector<unsigned> given_hand;
    std::vector<const Card*> fort_cards;

	// constructor
    Deck(
        const Cards& all_cards_,
        DeckType::DeckType decktype_ = DeckType::deck,
        unsigned id_ = 0,
        std::string name_ = "",
        unsigned upgrade_points_ = 0,
        unsigned upgrade_opportunities_ = 0,
        DeckStrategy::DeckStrategy strategy_ = DeckStrategy::random) :
        all_cards(all_cards_),
        decktype(decktype_),
        id(id_),
        name(name_),
        upgrade_points(upgrade_points_),
        upgrade_opportunities(upgrade_opportunities_),
        strategy(strategy_),
        commander(nullptr),
        shuffled_commander(nullptr),
        deck_size(0),
        mission_req(0)
    {
    }

	// destructor
    ~Deck() {}

	void set(
		const Card* commander_,
		unsigned commander_max_level_,
		const std::vector<const Card*>& cards_,
		std::vector<std::tuple<unsigned, unsigned, std::vector<const Card*>>> variable_cards_ = {},
		unsigned mission_req_ = 0);

    void set(const std::vector<unsigned>& ids, const std::map<signed, char> &marks);
    void set(const std::vector<unsigned>& ids)
    {
        std::map<signed, char> empty;
        set(ids, empty);
    }
    void set(const std::string& deck_string_);
    void resolve();
    void shrink(const unsigned deck_len);
    void set_vip_cards(const std::string& deck_string_);
    void set_allowed_candidates(const std::string& deck_string_);
    void set_disallowed_candidates(const std::string& deck_string_);
    void set_given_hand(const std::string& deck_string_);
    void add_forts(const std::string& deck_string_);

    Deck* clone() const;
    std::string hash() const;
    std::string short_description() const;
    std::string medium_description() const;
    std::string long_description() const;
    void show_upgrades(std::stringstream &ios, const Card* card, unsigned card_max_level, const char * leading_chars) const;
    const Card* next();
    const Card* upgrade_card(const Card* card, unsigned card_max_level, std::mt19937& re, unsigned &remaining_upgrade_points, unsigned &remaining_upgrade_opportunities);
    void shuffle(std::mt19937& re);
    void place_at_bottom(const Card* card);

	//static functions
	static void hash_to_ids_wmt_b64(const char* hash, std::vector<unsigned>& ids);
	static void encode_deck_wmt_b64(std::stringstream &ios, const Card* commander, std::vector<const Card*> cards);
	static void hash_to_ids_ext_b64(const char* hash, std::vector<unsigned>& ids);
	static void encode_deck_ext_b64(std::stringstream &ios, const Card* commander, std::vector<const Card*> cards);
	static void hash_to_ids_ddd_b64(const char* hash, std::vector<unsigned>& ids);
	static void encode_deck_ddd_b64(std::stringstream &ios, const Card* commander, std::vector<const Card*> cards);
};


// could be moved into own file at some point
class Decks
{
public:
    void add_deck(Deck* deck, const std::string& deck_name);
    Deck* find_deck_by_name(const std::string& deck_name);
    std::list<Deck> decks;
    std::map<std::pair<DeckType::DeckType, unsigned>, Deck*> by_type_id;
    std::map<std::string, Deck*> by_name;
};

#endif
