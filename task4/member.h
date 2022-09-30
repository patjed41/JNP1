// Patryk Jędrzejczak i Jan Ossowski

#ifndef MEMBER_H
#define MEMBER_H

#include <cstdint>
#include "treasure.h"

template<IntegralValue ValueType, bool IsArmed>
class Adventurer {};


template<IntegralValue ValueType>
class Adventurer<ValueType, false> {

  ValueType lootsValue;

public:

  using strength_t = uint32_t;

  constexpr Adventurer() {
    lootsValue = 0;
  }

  static constexpr bool isArmed = false;

  template<IntegralValue V, bool B>
  constexpr void loot(Treasure<V, B>&& treasure) {
    if (!treasure.isTrapped) {
      lootsValue += treasure.getLoot();
    }
  }

  constexpr ValueType pay() {
    ValueType payment = lootsValue;
    lootsValue = 0;
    return payment;
  }

};


template<IntegralValue ValueType>
class Adventurer<ValueType, true> {

public:

  using strength_t = uint32_t;

private:

  ValueType lootsValue;
  strength_t strength;

public:

  constexpr Adventurer(strength_t strength) : strength(strength) {
    lootsValue = 0;
  }

  constexpr strength_t getStrength() const {
    return strength;
  }

  static constexpr bool isArmed = true;
  
  template<IntegralValue V, bool B>
  constexpr void loot(Treasure<V, B>&& treasure) {
    if (!treasure.isTrapped) {
      lootsValue += treasure.getLoot();
    }
    else if (strength > 0) {
      lootsValue += treasure.getLoot();
      strength /= 2;
    }
  }

  constexpr ValueType pay() {
    ValueType payment = lootsValue;
    lootsValue = 0;
    return payment;
  }

};


template<class ValueType>
using Explorer = Adventurer<ValueType, false>;


template<IntegralValue ValueType, std::size_t CompletedExpeditions>
requires(CompletedExpeditions < 25)
class Veteran {

public:

  using strength_t = uint32_t;

private:

  ValueType lootsValue;
  strength_t strength;

  // Sets strength to Fibonacci(CompletedExpeditions).
  constexpr void setStrength() {
    if (CompletedExpeditions == 0) {
      strength = 0;
    }
    else {
      ValueType lastFib = 0, currentFib = 1;
      for (std::size_t i = 2; i <= CompletedExpeditions; i++) {
        currentFib = currentFib + lastFib;
        lastFib = currentFib - lastFib;
      }
      strength = currentFib;
    }
  }

public:

  constexpr Veteran() {
    lootsValue = 0;
    setStrength();
  }

  static constexpr bool isArmed = true;

  template<IntegralValue TreasureValueType, bool IsTrapped>
  constexpr void loot(Treasure<TreasureValueType, IsTrapped>&& treasure) {
    if (!treasure.isTrapped || strength > 0) {
      lootsValue += treasure.getLoot();
    }
  }

  constexpr ValueType pay() {
    ValueType payment = lootsValue;
    lootsValue = 0;
    return payment;
  }

  constexpr strength_t getStrength() const {
    return strength;
  }

};

#endif // MEMBER_H