// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Models/Enchantment.hpp>
#include <Rosetta/Tasks/SimpleTasks/ActivateCapturedDeathrattleTask.hpp>

namespace RosettaStone::SimpleTasks
{
TaskStatus ActivateCapturedDeathrattleTask::Impl(Player* player)
{
    const auto enchantment = dynamic_cast<Enchantment*>(m_target);
    if (enchantment == nullptr)
    {
        throw std::invalid_argument(
            "ActivateCapturedDeathrattleTask::Impl() - target is not "
            "Enchantment!");
    }

    Card* card = enchantment->GetCapturedCard();
    if (card == nullptr)
    {
        throw std::invalid_argument(
            "ActivateCapturedDeathrattleTask::Impl() - Enchantment doesn't "
            "have any captured card!");
    }

    auto tasks = card->power.GetDeathrattleTask();
    if (tasks.empty())
    {
        return TaskStatus::STOP;
    }

    for (auto& task : tasks)
    {
        std::unique_ptr<ITask> clonedTask = task->Clone();

        clonedTask->SetPlayer(player);
        clonedTask->SetSource(m_source);
        clonedTask->SetTarget(m_target);

        player->game->taskQueue.Enqueue(std::move(clonedTask));
    }

    return TaskStatus::COMPLETE;
}

std::unique_ptr<ITask> ActivateCapturedDeathrattleTask::CloneImpl()
{
    return std::make_unique<ActivateCapturedDeathrattleTask>();
}
}  // namespace RosettaStone::SimpleTasks
