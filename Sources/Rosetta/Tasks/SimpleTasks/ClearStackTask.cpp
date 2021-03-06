// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Tasks/SimpleTasks/ClearStackTask.hpp>

namespace RosettaStone::SimpleTasks
{
TaskStatus ClearStackTask::Impl(Player* player)
{
    player->game->taskStack.playables.clear();

    return TaskStatus::COMPLETE;
}

std::unique_ptr<ITask> ClearStackTask::CloneImpl()
{
    return std::make_unique<ClearStackTask>();
}
}  // namespace RosettaStone::SimpleTasks
