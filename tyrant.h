#ifndef TYRANT_H_INCLUDED
#define TYRANT_H_INCLUDED

#define TYRANT_OPTIMIZER_VERSION "2.25.0"

#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <boost/algorithm/string.hpp>

enum Faction
{
    allfactions,
    imperial,
    raider,
    bloodthirsty,
    xeno,
    righteous,
    progenitor,
    num_factions
};
extern const std::string faction_names[num_factions];

namespace Skill {
enum Skill
{
    // Placeholder for no-skill:
    no_skill,

    // Activation (harmful):
    enfeeble, jam, mortar, siege, strike, sunder, weaken,

    // Activation (helpful):
    enhance, evolve, heal, mend, overload, protect, rally, enrage, rush,

    // Defensive:
    armor, avenge, corrosive, counter, evade, payback, revenge, refresh, wall,

    // Combat-Modifier:
    legion, pierce, rupture, swipe, venom,

    // Damage-Dependent:
    berserk, inhibit, leech, poison,

    // Triggered:
    allegiance, flurry, valor,

    // End of skills
    num_skills
};
}
extern const std::string skill_names[Skill::num_skills];

namespace PassiveBGE {
enum PassiveBGE
{
    // Placeholder for no-bge:
    no_bge,

    // Passive BGEs
    bloodlust, brigade, counterflux, divert, enduringrage, fortification, heroism,
    zealotspreservation, metamorphosis, revenge, turningtides, virulence, haltedorders,

    // End of BGEs
    num_passive_bges
};
}
extern const std::string passive_bge_names[PassiveBGE::num_passive_bges];

inline bool is_activation_harmful_skill(Skill::Skill skill_id)
{
    switch(skill_id)
    {
    case Skill::enfeeble:
    case Skill::jam:
    case Skill::mortar:
    case Skill::siege:
    case Skill::strike:
    case Skill::sunder:
    case Skill::weaken:
        return true;
    default:
        return false;
    }
}

inline bool is_activation_helpful_skill(Skill::Skill skill_id)
{
    switch(skill_id)
    {
    case Skill::enhance:
    case Skill::evolve:
    case Skill::heal:
    case Skill::mend:
    case Skill::overload:
    case Skill::protect:
    case Skill::rally:
    case Skill::enrage:
    case Skill::rush:
        return true;
    default:
        return false;
    }
}

inline bool is_activation_skill(Skill::Skill skill_id)
{
    return is_activation_harmful_skill(skill_id)
        || is_activation_helpful_skill(skill_id);
}

inline bool is_defensive_skill(Skill::Skill skill_id)
{
    switch(skill_id)
    {
    case Skill::armor:
    case Skill::avenge:
    case Skill::corrosive:
    case Skill::counter:
    case Skill::evade:
    case Skill::payback:
    case Skill::revenge:
    case Skill::refresh:
    case Skill::wall:
        return true;
    default:
        return false;
    }
}

inline bool is_combat_modifier_skill(Skill::Skill skill_id)
{
    switch(skill_id)
    {
    case Skill::legion:
    case Skill::pierce:
    case Skill::rupture:
    case Skill::swipe:
    case Skill::venom:
        return true;
    default:
        return false;
    }
}

inline bool is_damage_dependent_skill(Skill::Skill skill_id)
{
    switch(skill_id)
    {
    case Skill::berserk:
    case Skill::inhibit:
    case Skill::leech:
    case Skill::poison:
        return true;
    default:
        return false;
    }
}

inline bool is_triggered_skill(Skill::Skill skill_id)
{
    switch(skill_id)
    {
    case Skill::allegiance:
    case Skill::flurry:
    case Skill::valor:
        return true;
    default:
        return false;
    }
}

inline PassiveBGE::PassiveBGE passive_bge_name_to_id(const std::string & name)
{
    for (unsigned i(PassiveBGE::no_bge); i < PassiveBGE::num_passive_bges; ++i)
    {
        if (boost::iequals(passive_bge_names[i], name))
        {
            return static_cast<PassiveBGE::PassiveBGE>(i);
        }
    }
    return PassiveBGE::no_bge;
}


namespace CardType {
enum CardType {
    commander,
    assault,
    structure,
    num_cardtypes
};
}

extern const std::string cardtype_names[CardType::num_cardtypes];

extern const std::string rarity_names[];

extern unsigned const upgrade_cost[];
extern unsigned const salvaging_income[][7];

namespace DeckType {
enum DeckType {
    deck,
    mission,
    raid,
    campaign,
    custom_deck,
    num_decktypes
};
}

extern std::string decktype_names[DeckType::num_decktypes];

enum gamemode_t
{
    fight,
    surge,
};

namespace QuestType
{
enum QuestType
{
    none,
    skill_use,
    skill_damage,
    faction_assault_card_use,
    type_card_use,
    faction_assault_card_kill,
    type_card_kill,
    card_survival,
    num_objective_types
};
}

enum class OptimizationMode
{
    notset,
    winrate,
    defense,
    war,
    brawl,
    brawl_defense,
    raid,
    campaign,
    quest,
    num_optimization_mode
};

extern unsigned min_possible_score[(size_t)OptimizationMode::num_optimization_mode];
extern unsigned max_possible_score[(size_t)OptimizationMode::num_optimization_mode];

struct SkillSpec
{
    Skill::Skill id;
    unsigned x;
    Faction y;
    unsigned n;
    unsigned c;
    Skill::Skill s;
    Skill::Skill s2;
    bool all;
};

// --------------------------------------------------------------------------------
// Common functions
template<typename T>
std::string to_string(const T val)
{
    std::stringstream s;
    s << val;
    return s.str();
}

//---------------------- Debugging stuff ---------------------------------------
extern signed debug_print;
extern unsigned debug_cached;
extern bool debug_line;
extern std::string debug_str;
#ifndef NDEBUG
#define _DEBUG_MSG(v, format, args...)                                  \
    {                                                                   \
        if(__builtin_expect(debug_print >= v, false))                   \
        {                                                               \
            if(debug_line) { printf("%i - " format, __LINE__ , ##args); }      \
            else if(debug_cached) {                                     \
                char buf[4096];                                         \
                snprintf(buf, sizeof(buf), format, ##args);             \
                debug_str += buf;                                       \
            }                                                           \
            else { printf(format, ##args); }                            \
            std::cout << std::flush;                                    \
        }                                                               \
    }
#define _DEBUG_SELECTION(format, args...)                               \
    {                                                                   \
        if(__builtin_expect(debug_print >= 2, 0))                       \
        {                                                               \
            _DEBUG_MSG(2, "Possible targets of " format ":\n", ##args); \
            fd->print_selection_array();                                \
        }                                                               \
    }
#else
#define _DEBUG_MSG(v, format, args...)
#define _DEBUG_SELECTION(format, args...)
#endif

#endif
