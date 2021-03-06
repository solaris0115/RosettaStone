﻿// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// Hearthstone++ is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Actions/Draw.hpp>
#include <Rosetta/CardSets/DemonHunterInitCardsGen.hpp>
#include <Rosetta/Enchants/Enchants.hpp>
#include <Rosetta/Tasks/SimpleTasks/AddCardTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/AddEnchantmentTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/CustomTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/DamageNumberTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/DamageTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/DrawTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/FuncNumberTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/GetGameTagTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/MathAddTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/RandomMinionNumberTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/RandomTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/SetGameTagNumberTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/SetGameTagTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/SilenceTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/SummonCopyTask.hpp>
#include <Rosetta/Tasks/SimpleTasks/SummonTask.hpp>
#include <Rosetta/Zones/HandZone.hpp>

using namespace RosettaStone;
using namespace SimpleTasks;

namespace RosettaStone
{
using PlayReqs = std::map<PlayReq, int>;
using EffectList = std::vector<std::shared_ptr<IEffect>>;

void DemonHunterInitCardsGen::AddDemonHunter(
    std::map<std::string, CardDef>& cards)
{
    Power power;

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_173] Command the Illidari - COST:5
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: Summon six 1/1 Illidari with <b>Rush</b>.
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(std::make_shared<SummonTask>("BT_036t", 6));
    cards.emplace("BT_173", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_175] Twin Slice - COST:0
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: Give your hero +1 Attack this turn.
    //       Add 'Second Slice' to your hand.
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("BT_175e", EntityType::HERO));
    power.AddPowerTask(
        std::make_shared<AddCardTask>(EntityType::HAND, "BT_175t", 1));
    cards.emplace("BT_175", CardDef(power));

    // ----------------------------------- WEAPON - DEMONHUNTER
    // [BT_271] Flamereaper - COST:7 [ATK:4/HP:0]
    // - Set: Demon Hunter Initiate, Rarity: Epic
    // --------------------------------------------------------
    // Text: Also damages the minions next to whomever your hero attacks.
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // - DURABILITY = 3
    // --------------------------------------------------------
    power.ClearData();
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::AFTER_ATTACK));
    power.GetTrigger()->triggerSource = TriggerSource::HERO;
    power.GetTrigger()->tasks = {
        std::make_shared<FuncNumberTask>([](Playable* playable) {
            const auto target = dynamic_cast<Minion*>(
                playable->game->currentEventData->eventTarget);
            if (target == nullptr)
            {
                return 0;
            }

            auto& taskStack = playable->game->taskStack;
            for (auto& minion : target->GetAdjacentMinions())
            {
                taskStack.playables.emplace_back(minion);
            }

            return playable->player->GetHero()->GetAttack();
        }),
        std::make_shared<DamageNumberTask>(EntityType::STACK)
    };
    cards.emplace("BT_271", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_351] Battlefiend - COST:1 [ATK:2/HP:2]
    // - Race: Demon, Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: After your hero attacks, gain +1 Attack.
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::AFTER_ATTACK));
    power.GetTrigger()->triggerSource = TriggerSource::HERO;
    power.GetTrigger()->tasks = { std::make_shared<AddEnchantmentTask>(
        "BT_351e", EntityType::SOURCE) };
    cards.emplace("BT_351", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_354] Blade Dance - COST:2
    // - Set: Demon Hunter Initiate, Rarity: Rare
    // --------------------------------------------------------
    // Text: Deal damage equal to your hero's Attack
    //       to 3 random enemy minions.
    // --------------------------------------------------------
    // GameTag:
    // - AFFECTED_BY_SPELL_POWER = 1
    // --------------------------------------------------------
    // PlayReq:
    // - REQ_MINIMUM_ENEMY_MINIONS = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<RandomTask>(EntityType::ENEMY_MINIONS, 3));
    power.AddPowerTask(
        std::make_shared<GetGameTagTask>(EntityType::HERO, GameTag::ATK));
    power.AddPowerTask(
        std::make_shared<DamageNumberTask>(EntityType::STACK, true));
    cards.emplace(
        "BT_354",
        CardDef(power, PlayReqs{ { PlayReq::REQ_MINIMUM_ENEMY_MINIONS, 1 } }));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_355] Wrathscale Naga - COST:3 [ATK:3/HP:1]
    // - Set: Demon Hunter Initiate, Rarity: Epic
    // --------------------------------------------------------
    // Text: After a friendly minion dies,
    //       deal 3 damage to a random enemy.
    // --------------------------------------------------------
    // GameTag:
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::DEATH));
    power.GetTrigger()->triggerSource = TriggerSource::MINIONS;
    power.GetTrigger()->tasks = {
        std::make_shared<RandomTask>(EntityType::ENEMIES, 1),
        std::make_shared<DamageTask>(EntityType::STACK, 3)
    };
    cards.emplace("BT_355", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_407] Ur'zul Horror - COST:1 [ATK:2/HP:1]
    // - Race: Demon, Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: <b>Deathrattle:</b> Add a 2/1 Lost Soul to your hand.
    // --------------------------------------------------------
    // GameTag:
    // - DEATHRATTLE = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddDeathrattleTask(
        std::make_shared<AddCardTask>(EntityType::HAND, "BT_407t"));
    cards.emplace("BT_407", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_416] Raging Felscreamer - COST:4 [ATK:4/HP:4]
    // - Set: Demon Hunter Initiate, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> The next Demon you play costs (2) less.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("BT_416e", EntityType::SOURCE));
    cards.emplace("BT_416", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_427] Feast of Souls - COST:2
    // - Set: Demon Hunter Initiate, Rarity: Rare
    // --------------------------------------------------------
    // Text: Draw a card for each friendly minion that died this turn.
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(std::make_shared<CustomTask>(
        [](Player* player, [[maybe_unused]] Entity* source,
           [[maybe_unused]] Playable* target) {
            const int num = player->GetNumFriendlyMinionsDiedThisTurn();

            for (int i = 0; i < num; ++i)
            {
                Generic::Draw(player, nullptr);
            }
        }));
    cards.emplace("BT_427", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_481] Nethrandamus - COST:9 [ATK:8/HP:8]
    // - Race: Dragon, Set: Demon Hunter Initiate, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> Summon two random 0-Cost minions.
    //       <i>(Upgrades each time a friendly minion dies!)</i>.
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(std::make_shared<GetGameTagTask>(
        EntityType::SOURCE, GameTag::TAG_SCRIPT_DATA_NUM_1));
    power.AddPowerTask(std::make_shared<RandomMinionNumberTask>(GameTag::COST));
    power.AddPowerTask(std::make_shared<SummonTask>(SummonSide::LEFT));
    power.AddPowerTask(std::make_shared<GetGameTagTask>(
        EntityType::SOURCE, GameTag::TAG_SCRIPT_DATA_NUM_1));
    power.AddPowerTask(std::make_shared<RandomMinionNumberTask>(GameTag::COST));
    power.AddPowerTask(std::make_shared<SummonTask>(SummonSide::RIGHT));
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::DEATH));
    power.GetTrigger()->triggerSource = TriggerSource::MINIONS;
    power.GetTrigger()->triggerActivation = TriggerActivation::HAND;
    power.GetTrigger()->tasks = {
        std::make_shared<GetGameTagTask>(EntityType::SOURCE,
                                         GameTag::TAG_SCRIPT_DATA_NUM_1),
        std::make_shared<MathAddTask>(1),
        std::make_shared<SetGameTagNumberTask>(EntityType::SOURCE,
                                               GameTag::TAG_SCRIPT_DATA_NUM_1),
    };
    cards.emplace("BT_481", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_487] Hulking Overfiend - COST:8 [ATK:5/HP:10]
    // - Race: Demon, Set: Demon Hunter Initiate, Rarity: Rare
    // --------------------------------------------------------
    // Text: <b>Rush</b>. After this attacks and kills a minion,
    //       it may attack again.
    // --------------------------------------------------------
    // GameTag:
    // - RUSH = 1
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::AFTER_ATTACK));
    power.GetTrigger()->triggerSource = TriggerSource::SELF;
    power.GetTrigger()->condition =
        std::make_shared<SelfCondition>(SelfCondition::IsDefenderDead());
    power.GetTrigger()->tasks = { std::make_shared<SetGameTagTask>(
        EntityType::SOURCE, GameTag::EXHAUSTED, 0) };
    cards.emplace("BT_487", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_488] Soul Split - COST:4
    // - Set: Demon Hunter Initiate, Rarity: Rare
    // --------------------------------------------------------
    // Text: Choose a friendly Demon. Summon a copy of it.
    // --------------------------------------------------------
    // PlayReq:
    // - REQ_TARGET_TO_PLAY = 0
    // - REQ_FRIENDLY_TARGET = 0
    // - REQ_TARGET_WITH_RACE = 15
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(std::make_shared<SummonCopyTask>(EntityType::TARGET));
    cards.emplace(
        "BT_488",
        CardDef(power, PlayReqs{ { PlayReq::REQ_TARGET_TO_PLAY, 0 },
                                 { PlayReq::REQ_FRIENDLY_TARGET, 0 },
                                 { PlayReq::REQ_TARGET_WITH_RACE, 15 } }));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_490] Consume Magic - COST:1
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: <b>Silence</b> an enemy minion.
    //       <b>Outcast:</b> Draw a card.
    // --------------------------------------------------------
    // PlayReq:
    // - REQ_TARGET_TO_PLAY = 0
    // - REQ_MINION_TARGET = 0
    // - REQ_ENEMY_TARGET = 0
    // --------------------------------------------------------
    // GameTag:
    // - OUTCAST = 1
    // --------------------------------------------------------
    // RefTag:
    // - SILENCE = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(std::make_shared<SilenceTask>(EntityType::TARGET));
    power.AddOutcastTask(std::make_shared<DrawTask>(1));
    cards.emplace("BT_490",
                  CardDef(power, PlayReqs{ { PlayReq::REQ_TARGET_TO_PLAY, 0 },
                                           { PlayReq::REQ_MINION_TARGET, 0 },
                                           { PlayReq::REQ_ENEMY_TARGET, 0 } }));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_510] Wrathspike Brute - COST:5 [ATK:2/HP:6]
    // - Race: Demon, Set: Demon Hunter Initiate, Rarity: Epic
    // --------------------------------------------------------
    // Text: <b>Taunt</b> After this is attacked,
    //       deal 1 damage to all enemies.
    // --------------------------------------------------------
    // GameTag:
    // - TAUNT = 1
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::AFTER_ATTACKED));
    power.GetTrigger()->triggerSource = TriggerSource::SELF;
    power.GetTrigger()->tasks = { std::make_shared<DamageTask>(
        EntityType::ENEMIES, 1, false) };
    cards.emplace("BT_510", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_752] Blur - COST:0
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: Your hero can't take damage this turn.
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("BT_752e", EntityType::HERO));
    cards.emplace("BT_752", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_753] Mana Burn - COST:1
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: Your opponent has 2 fewer Mana Crystals next turn.
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("BT_753e", EntityType::PLAYER));
    cards.emplace("BT_753", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_801] Eye Beam - COST:3
    // - Set: Demon Hunter Initiate, Rarity: Epic
    // --------------------------------------------------------
    // Text: <b>Lifesteal</b>. Deal 3 damage to a minion.
    //       <b>Outcast:</b> This costs (0).
    // --------------------------------------------------------
    // GameTag:
    // - LIFESTEAL = 1
    // - OUTCAST = 1
    // --------------------------------------------------------
    // PlayReq:
    // - REQ_TARGET_TO_PLAY = 0
    // - REQ_MINION_TARGET = 0
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<DamageTask>(EntityType::TARGET, 3, true));
    power.AddAura(std::make_shared<AdaptiveCostEffect>([](Playable* playable) {
        if (playable->GetZonePosition() == 0 ||
            playable->GetZonePosition() ==
                playable->player->GetHandZone()->GetCount() - 1)
        {
            return playable->GetGameTag(GameTag::COST);
        }

        return 0;
    }));
    cards.emplace(
        "BT_801",
        CardDef(power, PlayReqs{ { PlayReq::REQ_TARGET_TO_PLAY, 0 },
                                 { PlayReq::REQ_MINION_TARGET, 0 } }));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_814] Illidari Felblade - COST:4 [ATK:5/HP:3]
    // - Race: Demon, Set: Demon Hunter Initiate, Rarity: Epic
    // --------------------------------------------------------
    // Text: <b>Rush</b> <b>Outcast:</b> Gain <b>Immune</b> this turn.
    // --------------------------------------------------------
    // GameTag:
    // - OUTCAST = 1
    // - RUSH = 1
    // --------------------------------------------------------
    // RefTag:
    // - IMMUNE = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddOutcastTask(
        std::make_shared<AddEnchantmentTask>("BT_814e", EntityType::SOURCE));
    cards.emplace("BT_814", CardDef(power));

    // ----------------------------------- WEAPON - DEMONHUNTER
    // [BT_922] Umberwing - COST:2 [ATK:1/HP:0]
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: <b>Battlecry:</b> Summon two 1/1 Felwings.
    // --------------------------------------------------------
    // GameTag:
    // - BATTLECRY = 1
    // - DURABILITY = 2
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(std::make_shared<SummonTask>("BT_922t", 2));
    cards.emplace("BT_922", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_937] Altruis the Outcast - COST:3 [ATK:3/HP:2]
    // - Set: Demon Hunter Initiate, Rarity: Legendary
    // --------------------------------------------------------
    // Text: After you play the left- or right-most card in your hand,
    //       deal 1 damage to all enemies.
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - TRIGGER_VISUAL = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddTrigger(std::make_shared<Trigger>(TriggerType::AFTER_PLAY_CARD));
    power.GetTrigger()->condition = std::make_shared<SelfCondition>(
        SelfCondition::IsLeftOrRightMostCardInHand());
    power.GetTrigger()->tasks = { std::make_shared<DamageTask>(
        EntityType::ENEMIES, 1) };
    cards.emplace("BT_937", CardDef(power));
}

void DemonHunterInitCardsGen::AddDemonHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
    Power power;

    // ------------------------------ ENCHANTMENT - DEMONHUNTER
    // [BT_175e] Twin Slice (*) - COST:0
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    // Text: Your hero has +1 Attack this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddEnchant(Enchants::GetEnchantFromText("BT_175e"));
    cards.emplace("BT_175e", CardDef(power));

    // ------------------------------------ SPELL - DEMONHUNTER
    // [BT_175t] Second Slice - COST:0
    // - Set: Demon Hunter Initiate, Rarity: Common
    // --------------------------------------------------------
    // Text: Give your hero +1 Attack this turn.
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(
        std::make_shared<AddEnchantmentTask>("BT_175e", EntityType::HERO));
    cards.emplace("BT_175t", CardDef(power));

    // ------------------------------ ENCHANTMENT - DEMONHUNTER
    // [BT_351e] Felheartened - COST:0
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    // Text: Increased attack.
    // --------------------------------------------------------
    power.ClearData();
    power.AddEnchant(std::make_shared<Enchant>(Effects::AttackN(1)));
    cards.emplace("BT_351e", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_407t] Lost Soul (*) - COST:1 [ATK:2/HP:1]
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(nullptr);
    cards.emplace("BT_407t", CardDef(power));

    // ------------------------------ ENCHANTMENT - DEMONHUNTER
    // [BT_416e] Felscream (*) - COST:0
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    // Text: Your next Demon costs (2) less.
    // --------------------------------------------------------
    power.ClearData();
    power.AddAura(std::make_shared<Aura>(AuraType::HAND,
                                         EffectList{ Effects::ReduceCost(2) }));
    {
        const auto aura = dynamic_cast<Aura*>(power.GetAura());
        aura->condition =
            std::make_shared<SelfCondition>(SelfCondition::IsRace(Race::DEMON));
        aura->removeTrigger = { TriggerType::PLAY_MINION,
                                std::make_shared<SelfCondition>(
                                    SelfCondition::IsRace(Race::DEMON)) };
    }
    cards.emplace("BT_416e", CardDef(power));

    // ------------------------------ ENCHANTMENT - DEMONHUNTER
    // [BT_752e] Blur (*) - COST:0
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    // Text: <b>Immune</b> this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddEnchant(Enchants::GetEnchantFromText("BT_752e"));
    cards.emplace("BT_752e", CardDef(power));

    // ------------------------------ ENCHANTMENT - DEMONHUNTER
    // [BT_753e] Mana Burned (*) - COST:0
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    // Text: Start with 2 fewer Mana Crystals this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddAura(std::make_shared<Aura>(
        AuraType::ENEMY_PLAYER,
        EffectList{ std::make_shared<Effect>(GameTag::RESOURCES_USED,
                                             EffectOperator::ADD, 2) }));
    {
        const auto aura = dynamic_cast<Aura*>(power.GetAura());
        aura->removeTrigger = { TriggerType::TURN_END,
                                std::make_shared<SelfCondition>(
                                    SelfCondition::IsEnemyTurn()) };
    }
    cards.emplace("BT_753e", CardDef(power));

    // ------------------------------ ENCHANTMENT - DEMONHUNTER
    // [BT_814e] Nimble (*) - COST:0
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    // Text: <b>Immune</b> this turn.
    // --------------------------------------------------------
    // GameTag:
    // - TAG_ONE_TURN_EFFECT = 1
    // --------------------------------------------------------
    power.ClearData();
    power.AddEnchant(Enchants::GetEnchantFromText("BT_814e"));
    cards.emplace("BT_814e", CardDef(power));

    // ----------------------------------- MINION - DEMONHUNTER
    // [BT_922t] Felwing (*) - COST:1 [ATK:1/HP:1]
    // - Set: Demon Hunter Initiate
    // --------------------------------------------------------
    power.ClearData();
    power.AddPowerTask(nullptr);
    cards.emplace("BT_922t", CardDef(power));
}

void DemonHunterInitCardsGen::AddAll(std::map<std::string, CardDef>& cards)
{
    AddDemonHunter(cards);
    AddDemonHunterNonCollect(cards);
}
}  // namespace RosettaStone
