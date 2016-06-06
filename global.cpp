#include "global.h"

#include <boost/tokenizer.hpp>
#include <string>

const std::string faction_names[Faction::num_factions] =
{ 
	"", "imperial", "raider", "bloodthirsty", "xeno", "righteous", "progenitor" 
};

const std::string skill_names[Skill::num_skills] =
{
    // Placeholder for no-skill:
    "<Error>",

    // Activation (harmful):
    "Enfeeble", "Jam", "Mortar", "Siege", "Strike", "Sunder", "Weaken",

    // Activation (helpful):
    "Enhance", "Evolve", "Heal", "Mend", "Overload", "Protect", "Rally", "Enrage", "Rush",

    // Defensive:
    "Armor", "Avenge", "Corrosive", "Counter", "Evade", "Payback", "Revenge", "Refresh", "Wall",

    // Combat-Modifier:
    "Legion", "Pierce", "Rupture", "Swipe", "Venom",

    // Damage-Dependant:
    "Berserk", "Inhibit", "Leech", "Poison",

    // Triggered:
    "Allegiance", "Flurry", "Valor",
};

const std::string passive_bge_names[PassiveBGE::num_passive_bges] =
{
    // Placeholder for no-bge:
    "<Error>",

    // Passive BGEs
    "Bloodlust", "Brigade", "Counterflux", "Divert", "EnduringRage", "Fortification", "Heroism",
    "ZealotsPreservation", "Metamorphosis", "Revenge", "TurningTides", "Virulence", "HaltedOrders"
};

const std::string cardtype_names[CardType::num_cardtypes]
{
	"Commander", "Assault", "Structure" 
};

const std::string rarity_names[6]
{
	"", "common", "rare", "epic", "legend", "vindi" 
};

const std::string decktype_names[DeckType::num_decktypes]
{ 
	"Deck", "Mission", "Raid", "Campaign", "Custom Deck" 
};

unsigned const upgrade_cost[]
{
	0, 5, 15, 30, 75, 150
};

unsigned const salvaging_income[][7]
{
	{},									//commander
	{0, 1, 2, 5},						//common
	{0, 5, 10, 15, 20},					//rare
	{0, 20, 25, 30, 40, 50, 65},		//epic
	{0, 40, 45, 60, 75, 100, 125},		//legendary
	{0, 80, 85, 100, 125, 175, 250}		//vindicator
};

unsigned min_possible_score[]
{
	0, 0, 0, 10, 5, 5, 5, 0, 0
};

unsigned max_possible_score[]
{
	100, 100, 100, 100, 67, 67, 100, 100, 100
};


//....
signed debug_print(0);
unsigned debug_cached(0);
bool debug_line(false);
std::string debug_str("");


const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
const char* wmt_b64_magic_chars = "-.~!*";

// ---------------------------------------------
// common functions
// ---------------------------------------------
std::string simplify_name(const std::string& card_name)
{
	std::string simple_name;
	for (auto c : card_name)
	{
		if (!strchr(";:,\"'! ", c))
		{
			simple_name += ::tolower(c);
		}
	}
	return(simple_name);
}

std::list<std::string> get_abbreviations(const std::string& name)
{
	std::list<std::string> abbr_list;
	std::string initial;

	boost::tokenizer<boost::char_delimiters_separator<char>> word_token(name, boost::char_delimiters_separator<char>{false, " -", ""});

	auto token_iter = word_token.begin();

	for (; token_iter != word_token.end(); ++token_iter)
	{
		abbr_list.push_back(simplify_name(std::string{ token_iter->begin(), token_iter->end() }));
		initial += *token_iter->begin();
	}

	abbr_list.push_back(simplify_name(initial));
	return(abbr_list);
}



// TODO: create Skill class??
std::string skill_description(const SkillSpec& skill)
{
	return skill_names[skill.id] +
		(skill.all ? " all" : skill.n == 0 ? "" : std::string(" ") + to_string(skill.n)) +
		(skill.y == Faction::allfactions ? "" : std::string(" ") + faction_names[skill.y]) +
		(skill.s == Skill::no_skill ? "" : std::string(" ") + skill_names[skill.s]) +
		(skill.s2 == Skill::no_skill ? "" : std::string(" ") + skill_names[skill.s2]) +
		(skill.x == 0 ? "" : std::string(" ") + to_string(skill.x)) +
		(skill.c == 0 ? "" : std::string(" every ") + to_string(skill.c));
}

std::string skill_short_description(const SkillSpec& skill)
{
	// NOTE: not support summon
	return skill_names[skill.id] +
		(skill.s == Skill::no_skill ? "" : std::string(" ") + skill_names[skill.s]) +
		(skill.s2 == Skill::no_skill ? "" : std::string(" ") + skill_names[skill.s2]) +
		(skill.x == 0 ? "" : std::string(" ") + to_string(skill.x));
}
