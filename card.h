#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "global.h"

class Card
{

//public attributes of Card
public:
	unsigned m_attack;									// attack value of this card
	unsigned m_base_id;									// The id of the original card if a card is unique and alt/upgraded. The own id of the card otherwise.
	unsigned m_delay;									// delay until card comes into play
	Faction::Faction m_faction;							// faction this card belongs to (allfactions, imperial, raider, bloodthirsty, xeno, righteous, progenitor)
	unsigned m_health;									// health value of this card
	unsigned m_id;  
	unsigned m_level;
	unsigned m_fusion_level;
	std::string m_name;
	unsigned m_rarity;
	unsigned m_set;
	std::vector<SkillSpec> m_skills;					//skills of this card
	unsigned m_skill_value[Skill::num_skills];
	CardType::CardType m_type;							// commander (0), assault (1), structure (2)
	const Card* m_top_level_card;						// [TU] corresponding full-level card
	unsigned m_recipe_cost;
	std::map<const Card*, unsigned> m_recipe_cards;
	std::map<const Card*, unsigned> m_used_for_cards;

	

//public methods of Card
public:
	//constructor
	Card() :
		//set initial values
		m_attack(0),
		m_base_id(0),
		m_delay(0),
		m_faction(Faction::imperial),
		m_health(0),
		m_id(0),
		m_level(1),
		m_fusion_level(0),
		m_name(""),
		m_rarity(1),
		m_set(0),
		m_skills(),
		m_type(CardType::assault),
		m_top_level_card(this),
		m_recipe_cost(0),
		m_recipe_cards(),
		m_used_for_cards()
	{
		std::memset(m_skill_value, 0, sizeof m_skill_value);
	}

	void add_skill(Skill::Skill id, unsigned x, Faction::Faction y, unsigned n, unsigned c, Skill::Skill s, Skill::Skill s2, bool all);

	const Card* upgraded() const; 

	std::string card_description() const;

	static void encode_id_wmt_b64(std::stringstream &ios, unsigned card_id);
	static void encode_id_ext_b64(std::stringstream &ios, unsigned card_id);
	static void encode_id_ddd_b64(std::stringstream &ios, unsigned card_id);
};


// Card specific structures
class CardStatus
{
public:
	const Card* m_card;
    unsigned m_index;
    unsigned m_player;
    unsigned m_delay;
    Faction::Faction m_faction;
    unsigned m_attack;
    unsigned m_hp;
    unsigned m_max_hp;
    CardStep m_step;

    unsigned m_corroded_rate;
    unsigned m_corroded_weakened;
    unsigned m_enfeebled;
    unsigned m_evaded;
    unsigned m_inhibited;
    bool m_jammed;
    bool m_overloaded;
    unsigned m_paybacked;
    unsigned m_poisoned;
    unsigned m_protected;
    unsigned m_rallied;
    unsigned m_derallied;
    unsigned m_enraged;
    bool m_rush_attempted;
    bool m_sundered;
    unsigned m_weakened;

    signed m_primary_skill_offset[Skill::num_skills];
    signed m_evolved_skill_offset[Skill::num_skills];
    unsigned m_enhanced_value[Skill::num_skills];
    unsigned m_skill_cd[Skill::num_skills];

    CardStatus() {}

    void set(const Card* card);
    void set(const Card& card);
    std::string description() const;
    unsigned skill_base_value(Skill::Skill skill_id) const;
    unsigned skill(Skill::Skill skill_id) const;
    bool has_skill(Skill::Skill skill_id) const;
    unsigned enhanced(Skill::Skill skill) const;
    unsigned protected_value() const;
	unsigned attack_power(const CardStatus* card) const;
}; 

#endif
