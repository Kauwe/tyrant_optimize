#include "battle.h"
#include "card.h"
#include "sim.h"


// due to mysterious compilation errors, the method implementation for 
// class "Field" remains in sim.cpp for now







//template <typename CardsIter, typename Functor>
//unsigned Field::make_selection_array(CardsIter first, CardsIter last, Functor f)
//{
//	this->selection_array.clear();
//	for (auto c = first; c != last; ++c)
//	{
//		if (f(*c))
//		{
//			this->selection_array.push_back(*c);
//		}
//	}
//	return(this->selection_array.size());
//}
//CardStatus * Field::left_assault(const CardStatus * status)
//{
//	auto & assaults = this->players[status->m_player]->assaults;
//	if (status->m_index > 0)
//	{
//		auto left_status = &assaults[status->m_index - 1];
//		if (is_alive(left_status))
//		{
//			return left_status;
//		}
//	}
//	return nullptr;
//}
//CardStatus * Field::right_assault(const CardStatus * status)
//{
//	auto & assaults = this->players[status->m_player]->assaults;
//	if (status->m_index + 1 < assaults.size())
//	{
//		auto right_status = &assaults[status->m_index + 1];
//		if (is_alive(right_status))
//		{
//			return right_status;
//		}
//	}
//	return nullptr;
//}
//const std::vector<CardStatus *> Field::adjacent_assaults(const CardStatus * status)
//{
//	std::vector<CardStatus *> res;
//	auto left_status = left_assault(status);
//	auto right_status = right_assault(status);
//	if (left_status)
//	{
//		res.push_back(left_status);
//	}
//	if (right_status)
//	{
//		res.push_back(right_status);
//	}
//	return res;
//}
//void Field::print_selection_array()
//{
//#ifndef NDEBUG
//	for (auto c : this->selection_array)
//	{
//		_DEBUG_MSG(2, "+ %s\n", status_description(c).c_str());
//	}
//#endif
//}