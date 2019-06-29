// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

// It is based on peter1591's hearthstone-ai repository.
// References: https://github.com/peter1591/hearthstone-ai

#include <Rosetta/Views/Board.hpp>

namespace RosettaStone
{
Board::Board(Game& game, PlayerType playerType)
    : m_game(game), m_playerType(playerType)
{
    // Do nothing
}

void Board::RefCopyFrom(const Board& rhs)
{
    m_game.RefCopyFrom(rhs.m_game);
}

Player& Board::GetCurrentPlayer() const
{
    return m_game.GetCurrentPlayer();
}

PlayerType Board::GetViewType() const
{
    return m_playerType;
}

ReducedBoardView Board::CreateView() const
{
    if (m_playerType == PlayerType::PLAYER1)
    {
        return ReducedBoardView(BoardRefView(m_game, PlayerType::PLAYER1));
    }
    else
    {
        return ReducedBoardView(BoardRefView(m_game, PlayerType::PLAYER2));
    }
}

template <class Functor>
auto Board::ApplyWithPlayerStateView(Functor&& functor) const
{
    if (m_playerType == PlayerType::PLAYER1)
    {
        return functor(
            ReducedBoardView(BoardRefView(m_game, PlayerType::PLAYER1)));
    }
    else
    {
        return functor(
            ReducedBoardView(BoardRefView(m_game, PlayerType::PLAYER2)));
    }
}

auto Board::GetCurrentPlayerStateRefView() const
{
    if (m_game.GetCurrentPlayer().playerType != m_playerType)
    {
        throw std::runtime_error("current player does not match.");
    }

    return CurrentPlayerBoardRefView(m_game);
}

PlayState Board::ApplyAction() const
{
    return PlayState::PLAYING;
}

const Game& Board::RevealHiddenInformationForSimulation() const
{
    return m_game;
}
}  // namespace RosettaStone