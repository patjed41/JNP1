// Patryk Jędrzejczak i Jan Ossowski

#ifndef TREASURE_H
#define TREASURE_H

#include <type_traits>

template<class ValueType>
concept IntegralValue = std::is_integral_v<ValueType>;


template<IntegralValue ValueType, bool IsTrapped>
class Treasure {

  ValueType value;

public:

  constexpr Treasure(ValueType value) : value(value) {}

  constexpr ValueType evaluate() const {
    return value;
  }

  constexpr ValueType getLoot() {
    ValueType result = value;
    value = 0;
    return result;
  }

  static constexpr bool isTrapped = IsTrapped;

};


template<IntegralValue ValueType>
using SafeTreasure = Treasure<ValueType, false>;

template<IntegralValue ValueType>
using TrappedTreasure = Treasure<ValueType, true>;

#endif // TREASURE_H