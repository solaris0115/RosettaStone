// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/CardSets/BlackTempleCardsGen.hpp>
#include <Rosetta/CardSets/BoomsdayCardsGen.hpp>
#include <Rosetta/CardSets/CoreCardsGen.hpp>
#include <Rosetta/CardSets/DalaranCardsGen.hpp>
#include <Rosetta/CardSets/DemonHunterInitCardsGen.hpp>
#include <Rosetta/CardSets/DragonsCardsGen.hpp>
#include <Rosetta/CardSets/Expert1CardsGen.hpp>
#include <Rosetta/CardSets/GvgCardsGen.hpp>
#include <Rosetta/CardSets/HoFCardsGen.hpp>
#include <Rosetta/CardSets/LootapaloozaCardsGen.hpp>
#include <Rosetta/CardSets/UldumCardsGen.hpp>
#include <Rosetta/CardSets/YoDCardsGen.hpp>
#include <Rosetta/Cards/CardDefs.hpp>

namespace RosettaStone
{
std::map<std::string, CardDef> CardDefs::m_data;

CardDefs::CardDefs()
{
    CoreCardsGen::AddAll(m_data);
    Expert1CardsGen::AddAll(m_data);
    DemonHunterInitCardsGen::AddAll(m_data);
    HoFCardsGen::AddAll(m_data);
    GvgCardsGen::AddAll(m_data);
    LootapaloozaCardsGen::AddAll(m_data);
    BoomsdayCardsGen::AddAll(m_data);
    DalaranCardsGen::AddAll(m_data);
    UldumCardsGen::AddAll(m_data);
    DragonsCardsGen::AddAll(m_data);
    YoDCardsGen::AddAll(m_data);
    BlackTempleCardsGen::AddAll(m_data);
}

CardDefs::~CardDefs()
{
    m_data.clear();
}

CardDefs& CardDefs::GetInstance()
{
    static CardDefs instance;
    return instance;
}

CardDef CardDefs::FindCardDefByCardID(const std::string_view& cardID)
{
    for (auto& data : m_data)
    {
        if (data.first == cardID)
        {
            return data.second;
        }
    }

    return CardDef();
}
}  // namespace RosettaStone
