// Patryk Jędrzejczak, Aleksandra Kurc

#ifndef FUZZY_H
#define FUZZY_H

#include <iostream>
#include <algorithm>
#include <cmath>
#include <compare>
#include <set>
#include <stdexcept>
#include <tuple>

using real_t = double;
using rank_t = std::tuple<real_t, real_t, real_t>;

class TriFuzzyNum {

  real_t l, m, u;

  constexpr void correct_order() {
    if (l > m) std::swap(l, m);
    if (m > u) std::swap(m, u);
    if (l > m) std::swap(l, m);
  }

  rank_t get_rank() const;

public:

  constexpr TriFuzzyNum(real_t v1, real_t v2, real_t v3) : l(v1), m(v2), u(v3) {
    correct_order();
  }

  TriFuzzyNum(const TriFuzzyNum& other) = default;

  TriFuzzyNum(TriFuzzyNum&& other) = default;

  TriFuzzyNum& operator=(const TriFuzzyNum& other) = default;

  TriFuzzyNum& operator=(TriFuzzyNum&& other) = default;

  constexpr real_t lower_value() const {
    return l;
  }

  constexpr real_t modal_value() const {
    return m;
  }

  constexpr real_t upper_value() const {
    return u;
  }

  TriFuzzyNum& operator+=(const TriFuzzyNum& num);

  TriFuzzyNum& operator-=(const TriFuzzyNum& num);

  TriFuzzyNum& operator*=(const TriFuzzyNum& num);

  const TriFuzzyNum operator+(const TriFuzzyNum& num) const;

  const TriFuzzyNum operator-(const TriFuzzyNum& num) const;

  const TriFuzzyNum operator*(const TriFuzzyNum& num) const;

  std::partial_ordering operator<=>(const TriFuzzyNum& num) const;

  constexpr bool operator==(const TriFuzzyNum& num) const {
    return l == num.l && m == num.m && u == num.u;
  }

  constexpr bool operator!=(const TriFuzzyNum& num) const {
    return l != num.l || m != num.m || u != num.u;
  }

};

std::ostream& operator<<(std::ostream& os, const TriFuzzyNum& num);

class TriFuzzyNumSet {

  std::multiset<TriFuzzyNum> num_set;

public:

  TriFuzzyNumSet() {}

  TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> nums) : num_set(nums) {}

  TriFuzzyNumSet(const TriFuzzyNumSet& other) = default;

  TriFuzzyNumSet(TriFuzzyNumSet&& other) = default;

  TriFuzzyNumSet& operator=(const TriFuzzyNumSet& other) = default;

  TriFuzzyNumSet& operator=(TriFuzzyNumSet&& other) = default;

  void insert(const TriFuzzyNum& num);

  void insert(const TriFuzzyNum&& num);

  // version of remove that removes only one element
  void remove(const TriFuzzyNum& num);

  TriFuzzyNum arithmetic_mean() const;

};

consteval TriFuzzyNum crisp_number(const real_t v) {
  return TriFuzzyNum(v, v, v);
}

static constinit TriFuzzyNum crisp_zero = crisp_number(0);

#endif // FUZZY_H
