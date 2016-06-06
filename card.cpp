#include "card.h"

// class Card
void Card::encode_id_wmt_b64(std::stringstream &ios, unsigned card_id)
{
	if (card_id > 4000)
	{
		ios << wmt_b64_magic_chars[(card_id - 1) / 4000 - 1];
		card_id = (card_id - 1) % 4000 + 1;
	}
	ios << base64_chars[card_id / 64];
	ios << base64_chars[card_id % 64];
}

void Card::encode_id_ext_b64(std::stringstream &ios, unsigned card_id)
{
	while (card_id >= 32)
	{
		ios << base64_chars[card_id % 32];
		card_id /= 32;
	}
	ios << base64_chars[card_id + 32];
}

void Card::encode_id_ddd_b64(std::stringstream &ios, unsigned card_id)
{
	ios << base64_chars[card_id / 4096];
	ios << base64_chars[card_id % 4096 / 64];
	ios << base64_chars[card_id % 64];
}

void Card::add_skill(Skill::Skill id, unsigned x, Faction::Faction y, unsigned n, unsigned c, Skill::Skill s, Skill::Skill s2, bool all)
{
	for (auto it = m_skills.begin(); it != m_skills.end(); ++it)
	{
		if (it->id == id)
		{
			m_skills.erase(it);
			break;
		}
	}
	m_skills.push_back({ id, x, y, n, c, s, s2, all });
	m_skill_value[id] = x ? x : n ? n : 1;
}

const Card* Card::upgraded() const
{ 
	return this == m_top_level_card ? this : m_used_for_cards.begin()->first; 
}


std::string Card::card_description() const
{
	std::string desc;
	desc = m_name;
	switch (m_type)
	{
	case CardType::assault:
		desc += ": " + to_string(m_attack) + "/" + to_string(m_health) + "/" + to_string(m_delay);
		break;
	case CardType::structure:
		desc += ": " + to_string(m_health) + "/" + to_string(m_delay);
		break;
	case CardType::commander:
		desc += ": hp:" + to_string(m_health);
		break;
	case CardType::num_cardtypes:
		assert(false);
		break;
	}

	if (m_rarity >= 4) { desc += " " + rarity_names[m_rarity]; }
	if (m_faction != Faction::allfactions) { desc += " " + faction_names[m_faction]; }

	for (auto& skill : m_skills)
	{
		desc += ", " + skill_description(skill);
	}

	return(desc);
}


// class CardStatus
unsigned CardStatus::skill_base_value(Skill::Skill skill_id) const
{
	return m_card->m_skill_value[skill_id + m_primary_skill_offset[skill_id]]
		+ (skill_id == Skill::berserk ? m_enraged : 0);
}
//------------------------------------------------------------------------------
unsigned CardStatus::skill(Skill::Skill skill_id) const
{
	return skill_base_value(skill_id) + enhanced(skill_id);
}
//------------------------------------------------------------------------------
bool CardStatus::has_skill(Skill::Skill skill_id) const
{
	return skill_base_value(skill_id);
}
//------------------------------------------------------------------------------
unsigned CardStatus::enhanced(Skill::Skill skill_id) const
{
	return m_enhanced_value[skill_id + m_primary_skill_offset[skill_id]];
}
//------------------------------------------------------------------------------
unsigned CardStatus::protected_value() const
{
	return m_protected;
}
//------------------------------------------------------------------------------
void CardStatus::set(const Card* card)
{
	this->set(*card);
}
//------------------------------------------------------------------------------
void CardStatus::set(const Card& card)
{
	m_card = &card;
	m_index = 0;
	m_player = 0;
	m_delay = card.m_delay;
	m_faction = card.m_faction;
	m_attack = card.m_attack;
	m_hp = m_max_hp = card.m_health;
	m_step = CardStep::none;

	m_corroded_rate = 0;
	m_corroded_weakened = 0;
	m_enfeebled = 0;
	m_evaded = 0;
	m_inhibited = 0;
	m_jammed = false;
	m_overloaded = false;
	m_paybacked = 0;
	m_poisoned = 0;
	m_protected = 0;
	m_rallied = 0;
	m_enraged = 0;
	m_derallied = 0;
	m_rush_attempted = false;
	m_sundered = false;
	m_weakened = 0;

	std::memset(m_primary_skill_offset, 0, sizeof m_primary_skill_offset);
	std::memset(m_evolved_skill_offset, 0, sizeof m_evolved_skill_offset);
	std::memset(m_enhanced_value, 0, sizeof m_enhanced_value);
	std::memset(m_skill_cd, 0, sizeof m_skill_cd);
}
//------------------------------------------------------------------------------
std::string CardStatus::description() const
{
	std::string desc = "P" + to_string(m_player) + " ";
	switch (m_card->m_type)
	{
	case CardType::commander: desc += "Commander "; break;
	case CardType::assault: desc += "Assault " + to_string(m_index) + " "; break;
	case CardType::structure: desc += "Structure " + to_string(m_index) + " "; break;
	case CardType::num_cardtypes: assert(false); break;
	}
	desc += "[" + m_card->m_name;
	switch (m_card->m_type)
	{
	case CardType::assault:
		desc += " att:[" + to_string(m_attack);
		{
			std::string att_desc;
			if (m_weakened > 0) { att_desc += "-" + to_string(m_weakened) + "(weakened)"; }
			if (m_corroded_weakened > 0) { att_desc += "-" + to_string(m_corroded_weakened) + "(corroded)"; }
			att_desc += "]";
			if (m_rallied > 0) { att_desc += "+" + to_string(m_rallied) + "(rallied)"; }
			if (m_derallied > 0) { att_desc += "-" + to_string(m_derallied) + "(derallied)"; }
			//if (!att_desc.empty()) { desc += att_desc + "=" + to_string(attack_power()); }
			if (!att_desc.empty()) { desc += att_desc + "=" + to_string(0); }
		}
	case CardType::structure:
	case CardType::commander:
		desc += " hp:" + to_string(m_hp);
		break;
	case CardType::num_cardtypes:
		assert(false);
		break;
	}
	if (m_delay > 0) {
		desc += " cd:" + to_string(m_delay);
	}
	// Status w/o value
	if (m_jammed) { desc += ", jammed"; }
	if (m_overloaded) { desc += ", overloaded"; }
	if (m_sundered) { desc += ", sundered"; }
	// Status w/ value
	if (m_corroded_rate > 0) { desc += ", corroded " + to_string(m_corroded_rate); }
	if (m_enfeebled > 0) { desc += ", enfeebled " + to_string(m_enfeebled); }
	if (m_inhibited > 0) { desc += ", inhibited " + to_string(m_inhibited); }
	if (m_poisoned > 0) { desc += ", poisoned " + to_string(m_poisoned); }
	if (m_protected > 0) { desc += ", protected " + to_string(m_protected); }
	if (m_enraged > 0) { desc += ", enraged " + to_string(m_enraged); }
	//    if(m_step != CardStep::none) { desc += ", Step " + to_string(static_cast<int>(m_step)); }
	for (const auto & ss : m_card->m_skills)
	{
		std::string skill_desc;
		if (m_evolved_skill_offset[ss.id] != 0) { skill_desc += "->" + skill_names[ss.id + m_evolved_skill_offset[ss.id]]; }
		if (m_enhanced_value[ss.id] != 0) { skill_desc += " +" + to_string(m_enhanced_value[ss.id]); }
		if (!skill_desc.empty()) { desc += ", " + skill_names[ss.id] + skill_desc; }
	}
	desc += "]";
	return(desc);
}

unsigned CardStatus::attack_power(const CardStatus* card) const
{
	return safe_minus(safe_minus(card->m_attack, card->m_weakened + card->m_corroded_weakened) + card->m_rallied, m_derallied);
}