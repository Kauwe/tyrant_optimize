#ifndef BATTLE_H_INCLUDED
#define BATTLE_H_INCLUDED

#include <random>
#include <deque>
#include <array>

#include "global.h"

class Card;
class Cards;
class CardStatus;
class Field;
class Hand;


// the data model of a battle:
// an attacker and a defender deck, list of assaults and structures, etc.
class Field
{
public:
	bool end;
	std::mt19937& re;
	const Cards& cards;
	// players[0]: the attacker, players[1]: the defender
	std::array<Hand*, 2> players;
	unsigned tapi; // current turn's active player index
	unsigned tipi; // and inactive
	Hand* tap;
	Hand* tip;
	std::vector<CardStatus*> selection_array;
	unsigned turn;
	gamemode_t gamemode;
	OptimizationMode optimization_mode;
	const Quest quest;
	std::unordered_map<unsigned, unsigned> bg_effects; // passive BGE
	std::vector<SkillSpec> bg_skills[2]; // active BGE, casted every turn
										 // With the introduction of on death skills, a single skill can trigger arbitrary many skills.
										 // They are stored in this, and cleared after all have been performed.
	std::deque<std::tuple<CardStatus*, SkillSpec>> skill_queue;
	std::vector<CardStatus*> killed_units;
	enum phase
	{
		playcard_phase,
		legion_phase,
		commander_phase,
		structures_phase,
		assaults_phase,
		end_phase,
	};
	// the current phase of the turn: starts with playcard_phase, then commander_phase, structures_phase, and assaults_phase
	phase current_phase;
	// the index of the card being evaluated in the current phase.
	// Meaningless in playcard_phase,
	// otherwise is the index of the current card in players->structures or players->assaults
	unsigned current_ci;

	bool assault_bloodlusted;
	unsigned bloodlust_value;
	unsigned quest_counter;

	Field(std::mt19937& re_, const Cards& cards_, Hand& hand1, Hand& hand2, gamemode_t gamemode_, OptimizationMode optimization_mode_, const Quest & quest_,
		std::unordered_map<unsigned, unsigned>& bg_effects_, std::vector<SkillSpec>& your_bg_skills_, std::vector<SkillSpec>& enemy_bg_skills_) :
		end{ false },
		re(re_),
		cards(cards_),
		players{ { &hand1, &hand2 } },
		turn(1),
		gamemode(gamemode_),
		optimization_mode(optimization_mode_),
		quest(quest_),
		bg_effects{ bg_effects_ },
		bg_skills{ your_bg_skills_, enemy_bg_skills_ },
		assault_bloodlusted(false),
		bloodlust_value(0),
		quest_counter(0)
	{
	}

	inline unsigned rand(unsigned x, unsigned y)
	{
		return(std::uniform_int_distribution<unsigned>(x, y)(re));
	}

	inline unsigned flip()
	{
		return(this->rand(0, 1));
	}

	template <typename T>
	inline T random_in_vector(const std::vector<T>& v)
	{
		assert(v.size() > 0);
		return(v[this->rand(0, v.size() - 1)]);
	}

	template <typename CardsIter, typename Functor>
	unsigned make_selection_array(CardsIter first, CardsIter last, Functor f);
	CardStatus * left_assault(const CardStatus * status);
	CardStatus * right_assault(const CardStatus * status);
	const std::vector<CardStatus *> adjacent_assaults(const CardStatus * status);
	void print_selection_array();

	inline void inc_counter(QuestType::QuestType quest_type, unsigned quest_key, unsigned quest_2nd_key = 0, unsigned value = 1)
	{
		if (quest.quest_type == quest_type && quest.quest_key == quest_key && (quest.quest_2nd_key == 0 || quest.quest_2nd_key == quest_2nd_key))
		{
			quest_counter += value;
		}
	}
};

#endif
