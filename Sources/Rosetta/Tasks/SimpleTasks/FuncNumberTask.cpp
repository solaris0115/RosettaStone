// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Tasks/SimpleTasks/FuncNumberTask.hpp>

#include <utility>

namespace RosettaStone::SimpleTasks
{
FuncNumberTask::FuncNumberTask(std::function<int(Playable*)> func)
    : m_func(std::move(func))
{
    // Do nothing
}

TaskStatus FuncNumberTask::Impl(Player* player)
{
    if (m_func != nullptr)
    {
        player->game->taskStack.num[0] =
            m_func(dynamic_cast<Playable*>(m_source));
    }

    return TaskStatus::COMPLETE;
}

std::unique_ptr<ITask> FuncNumberTask::CloneImpl()
{
    return std::make_unique<FuncNumberTask>(m_func);
}
}  // namespace RosettaStone::SimpleTasks
