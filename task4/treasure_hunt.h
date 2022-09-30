// Patryk Jędrzejczak i Jan Ossowski

#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <utility>
#include "member.h"

template<class T>
static concept IsTreasure =
  requires(T t) {
    { Treasure(t) } -> std::same_as<T>;
  };

template<class T>
static concept IsMember =
  requires(T t) {
    typename T::strength_t;

    { []() constexpr { return T::isArmed; }() };
    { T::isArmed } -> std::convertible_to<bool>;

    { t.pay() } -> IntegralValue;

    t.loot(Treasure<decltype(t.pay()), false>(0));
    t.loot(Treasure<decltype(t.pay()), true>(0));
  };

template<class T>
concept EncounterSide = IsTreasure<T> || IsMember<T>;

template<class T, class U>
static concept Meeting =
  IsMember<T> &&
  IsMember<U> &&
  requires(T t, U u) {
    requires std::same_as<decltype(t.pay()), decltype(u.pay())>;
  };

template<class T, class U>
static concept RightFinding =
  IsMember<T> &&
  IsTreasure<U> &&
  requires(T t, U u) {
    requires std::same_as<decltype(t.pay()), decltype(u.getLoot())>;
  };

template<class T, class U>
static concept LeftFinding =
  IsTreasure<T> &&
  IsMember<U> &&
  requires(T t, U u) {
    requires std::same_as<decltype(t.getLoot()), decltype(u.pay())>;
  };


template<EncounterSide sideA, EncounterSide sideB>
requires Meeting<sideA, sideB> || RightFinding<sideA, sideB> || LeftFinding<sideA, sideB>
struct Encounter {

  sideA& A;
  sideB& B;

  constexpr Encounter(sideA& A, sideB& B) : A(A), B(B) {}

};


template<class T>
static concept IsArmed = IsMember<T> && requires(T t) {t.getStrength();};

template<class T, class U>
requires Meeting<T, U> && IsArmed<T> && IsArmed<U>
constexpr void run(Encounter<T, U> e) {
  if (e.A.getStrength() > e.B.getStrength()) {
    auto payment = Treasure<decltype(e.B.pay()), false>(e.B.pay());
    e.A.loot(std::move(payment));
  }
  else if (e.A.getStrength() < e.B.getStrength()) {
    auto payment = Treasure<decltype(e.A.pay()), false>(e.A.pay());
    e.B.loot(std::move(payment));
  }
}

template<class T, class U>
requires Meeting<T, U> && IsArmed<T> && (!IsArmed<U>)
constexpr void run(Encounter<T, U> e) {
  auto payment = Treasure<decltype(e.B.pay()), false>(e.B.pay());
  e.A.loot(std::move(payment));
}

template<class T, class U>
requires Meeting<T, U> && (!IsArmed<T>) && IsArmed<U>
constexpr void run(Encounter<T, U> e) {
  auto payment = Treasure<decltype(e.A.pay()), false>(e.A.pay());
  e.B.loot(std::move(payment));
}

template<class T, class U>
requires Meeting<T, U> && (!IsArmed<T>) && (!IsArmed<U>)
constexpr void run([[maybe_unused]] Encounter<T, U> e) {
  return;
}

template<class T, class U>
requires RightFinding<T, U>
constexpr void run(Encounter<T, U> e) {
  e.A.loot(std::move(e.B));
}

template<class T, class U>
requires LeftFinding<T, U>
constexpr void run(Encounter<T, U> e) {
  e.B.loot(std::move(e.A));
}

constexpr void expedition() {
  return;
}

template<class T, typename... Args>
constexpr void expedition(T t, Args... args) {
  run(t);
  expedition(args...);
}

#endif // TREASURE_HUNT_H