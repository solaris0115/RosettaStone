// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_ONGOING_ENCHANT_HPP
#define ROSETTASTONE_ONGOING_ENCHANT_HPP

#include <Rosetta/Auras/IAura.hpp>
#include <Rosetta/Enchants/Enchant.hpp>
#include <Rosetta/Games/Game.hpp>

#include <memory>

namespace RosettaStone
{
//!
//! \brief OngoingEnchant class.
//!
//! This class is implementation of a kind of enchantment that its effect
//! gradually grows due to a trigger. OngoingEnchant is narrowly used when the
//! source of the trigger and the target of the Enchantment is identical.
//! (e.g. Mana Wyrm)
//!
class OngoingEnchant : public Enchant, public IAura
{
 public:
    //! Constructs ongoing enchant with given \p effects.
    //! \param effects A list of effect.
    explicit OngoingEnchant(std::vector<std::shared_ptr<IEffect>> effects);

    //! Activates enchant to \p entity.
    //! \param entity An entity to which enchant is activated.
    //! \param num1 The number of GameTag::TAG_SCRIPT_DATA_NUM_1.
    //! \param num2 The number of GameTag::TAG_SCRIPT_DATA_NUM_2.
    void ActivateTo(Entity* entity, int num1 = 0, int num2 = -1) override;

    //! Activates this effect and add an instance to the game of given entity.
    //! \param owner The entity who owns this effect.
    //! \param cloning The flag to indicate that it is cloned.
    void Activate(Playable* owner, bool cloning = false) override;

    //! Updates this effect to apply the effect to recently modified entities.
    void Update() override;

    //! Removes this effect from the game to stop affecting entities.
    void Remove() override;

    //! Clones aura effect to \p clone.
    //! \param clone The entity to clone aura effect.
    void Clone(Playable* clone) override;

    //! Gets the count of ongoing enchants.
    //! \return The count of ongoing enchants.
    std::size_t GetCount() const;

    //! Sets the count of ongoing enchants.
    //! \param value the count of ongoing enchants.
    void SetCount(std::size_t value);

    Game* game = nullptr;
    Playable* target = nullptr;

 private:
    std::size_t m_count = 1;
    std::size_t m_lastCount = 1;
    bool m_toBeUpdated = false;
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_ONGOING_ENCHANT_HPP
