#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED

#include <boost/pool/pool.hpp>
#include <string>
#include <array>
#include <deque>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <map>
#include <random>

#include "global.h"

class Card;
class Cards;
class CardStatus;
class Deck;
class Field;
class Achievement;

extern unsigned turn_limit;

std::string skill_description(const Cards& cards, const SkillSpec& s);
bool is_alive(CardStatus* c);

//---------------------- Represent Simulation Results ----------------------------
template<typename result_type>
struct Results
{
    result_type wins;
    result_type draws;
    result_type losses;
    result_type points;
    template<typename other_result_type>
    Results& operator+=(const Results<other_result_type>& other)
    {
        wins += other.wins;
        draws += other.draws;
        losses += other.losses;
        points += other.points;
        return *this;
    }
};

typedef std::pair<std::vector<Results<int64_t>>, unsigned> EvaluatedResults;

template<typename result_type>
struct FinalResults
{
    result_type wins;
    result_type draws;
    result_type losses;
    result_type points;
    result_type points_lower_bound;
    result_type points_upper_bound;
    uint64_t n_sims;
};

void fill_skill_table();
Results<uint64_t> play(Field* fd);

#endif
