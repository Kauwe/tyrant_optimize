#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#define TYRANT_OPTIMIZER_VERSION "2.25.0"

#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/pool/pool.hpp>

class SkillSpec;


// -------------------------------------------
// available Factions
// -------------------------------------------
namespace Faction {
	enum Faction
	{
		allfactions,
		imperial,
		raider,
		bloodthirsty,
		xeno,
		righteous,
		progenitor,

		// End of Factions
		num_factions
	};
}
extern const std::string faction_names[Faction::num_factions];


// -------------------------------------------
// available Skills
// -------------------------------------------
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


// -------------------------------------------
// available passive Battle Ground Effects
// -------------------------------------------
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


// -------------------------------------------
// available card types
// -------------------------------------------
namespace CardType {
	enum CardType {
		commander,
		assault,
		structure,

		// End of Card Types
		num_cardtypes
	};
}
extern const std::string cardtype_names[CardType::num_cardtypes];


enum class CardStep
{
	none,
	attacking,
	attacked,
};

// -------------------------------------------
// available quest types
// -------------------------------------------
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

		// End of Quest Types
		num_objective_types
	};
}


// -------------------------------------------
// available deck types
// -------------------------------------------
namespace DeckType {
	enum DeckType {
		deck,
		mission,
		raid,
		campaign,
		custom_deck,

		// End of Deck Types
		num_decktypes
	};
}
extern const std::string decktype_names[DeckType::num_decktypes];


// -------------------------------------------
// available deck strategies
// -------------------------------------------
namespace DeckStrategy
{
	enum DeckStrategy
	{
		random,
		ordered,
		exact_ordered,

		// End of deck strategies
		num_deckstrategies
	};
}

// -------------------------------------------
// TODO: rearrange...
// -------------------------------------------

extern const std::string rarity_names[];

extern unsigned const upgrade_cost[];
extern unsigned const salvaging_income[][7];

extern const char* base64_chars;
extern const char* wmt_b64_magic_chars;


enum gamemode_t
{
	fight,
	surge,
};


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

	// End of Optimization Modes
	num_optimization_mode
};

extern unsigned min_possible_score[(size_t)OptimizationMode::num_optimization_mode];
extern unsigned max_possible_score[(size_t)OptimizationMode::num_optimization_mode];


// -------------------------------------------
// structures
// -------------------------------------------
struct Quest
{
	QuestType::QuestType quest_type;
	unsigned quest_key;
	unsigned quest_2nd_key;
	unsigned quest_value;
	unsigned quest_score; // score for quest goal
	unsigned win_score;   // score for win regardless quest goal
	bool must_fulfill;  // true: score iff value is reached; false: score proportion to achieved value
	bool must_win;      // true: score only if win
	Quest() :
		quest_type(QuestType::none),
		quest_key(0),
		quest_value(0),
		quest_score(100),
		win_score(0),
		must_fulfill(false),
		must_win(false)
	{}
};

struct SkillSpec
{
	Skill::Skill id;
	unsigned x;
	Faction::Faction y;
	unsigned n;
	unsigned c;
	Skill::Skill s;
	Skill::Skill s2;
	bool all;
};

// -------------------------------------------
// global methods
// -------------------------------------------
inline unsigned safe_minus(unsigned x, unsigned y)
{
	return(x - std::min(x, y));
}

inline bool is_activation_harmful_skill(Skill::Skill skill_id)
{
	switch (skill_id)
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
	switch (skill_id)
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
	switch (skill_id)
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
	switch (skill_id)
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
	switch (skill_id)
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
	switch (skill_id)
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


template<typename T>
std::string to_string(const T val)
{
	std::stringstream s;
	s << val;
	return s.str();
}


std::string simplify_name(const std::string& card_name);

std::list<std::string> get_abbreviations(const std::string& name);

std::string skill_description(const SkillSpec& skill);

std::string skill_short_description(const SkillSpec& skill);



// ---------------------------------------------
// template functions/classes
// ---------------------------------------------


//---------------------- Pool-based indexed storage ----------------------------
template<typename T>
class Storage
{
public:
	typedef typename std::vector<T*>::size_type size_type;
	typedef T value_type;
	Storage(size_type size) :
		m_pool(sizeof(T))
	{
		m_indirect.reserve(size);
	}

	inline T& operator[](size_type i)
	{
		return(*m_indirect[i]);
	}

	inline T& add_back()
	{
		m_indirect.emplace_back((T*)m_pool.malloc());
		return(*m_indirect.back());
	}

	template<typename Pred>
	void remove(Pred p)
	{
		size_type head(0);
		for (size_type current(0); current < m_indirect.size(); ++current)
		{
			if (p((*this)[current]))
			{
				m_pool.free(m_indirect[current]);
			}
			else
			{
				if (current != head)
				{
					m_indirect[head] = m_indirect[current];
				}
				++head;
			}
		}
		m_indirect.erase(m_indirect.begin() + head, m_indirect.end());
	}

	void reset()
	{
		for (auto index : m_indirect)
		{
			m_pool.free(index);
		}
		m_indirect.clear();
	}

	inline size_type size() const
	{
		return(m_indirect.size());
	}

	std::vector<T*> m_indirect;
	boost::pool<> m_pool;
};

template<typename RandomAccessIterator, typename UniformRandomNumberGenerator>
void partial_shuffle(RandomAccessIterator first, RandomAccessIterator middle,
	RandomAccessIterator last,
	UniformRandomNumberGenerator&& g)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::difference_type diff_t;
	typedef typename std::make_unsigned<diff_t>::type udiff_t;
	typedef typename std::uniform_int_distribution<udiff_t> distr_t;
	typedef typename distr_t::param_type param_t;

	distr_t D;
	diff_t m = middle - first;
	diff_t n = last - first;
	for (diff_t i = 0; i < m; ++i)
	{
		std::swap(first[i], first[D(g, param_t(i, n - 1))]);
	}
}


template<typename Iterator, typename Functor> Iterator advance_until(Iterator it, Iterator it_end, Functor f)
{
	while (it != it_end)
	{
		if (f(*it))
		{
			break;
		}
		++it;
	}
	return(it);
}

// take care that "it" is 1 past current.
template<typename Iterator, typename Functor> Iterator recede_until(Iterator it, Iterator it_beg, Functor f)
{
	if (it == it_beg) { return(it_beg); }
	--it;
	do
	{
		if (f(*it))
		{
			return(++it);
		}
		--it;
	} while (it != it_beg);
	return(it_beg);
}

template<typename Iterator, typename Functor, typename Token> Iterator read_token(Iterator it, Iterator it_end, Functor f, Token& token)
{
	Iterator token_start = advance_until(it, it_end, [](const char& c) {return(c != ' '); });
	Iterator token_end_after_spaces = advance_until(token_start, it_end, f);
	if (token_start != token_end_after_spaces)
	{
		Iterator token_end = recede_until(token_end_after_spaces, token_start, [](const char& c) {return(c != ' '); });
		token = boost::lexical_cast<Token>(std::string{ token_start, token_end });
	}
	return(token_end_after_spaces);
}

// ---------------------------------------
// Debugging stuff 
// ---------------------------------------
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
