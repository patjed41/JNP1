// Patryk Jędrzejczak, Aleksandra Kurc

#include "fuzzy.h"

rank_t TriFuzzyNum::get_rank() const {
  real_t z = (u - l) + sqrt(1 + (u - m) * (u - m))
             + sqrt(1 + (m - l) * (m - l));
  real_t y = (u - l) / z;
  real_t x = ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l
             + sqrt(1 + (m - l) * (m - l)) * u) / z;

  return std::make_tuple(x - y / 2, 1 - y, m);
}

TriFuzzyNum& TriFuzzyNum::operator+=(const TriFuzzyNum& num) {
  l += num.l;
  m += num.m;
  u += num.u;

  return *this;
}

TriFuzzyNum& TriFuzzyNum::operator-=(const TriFuzzyNum& num) {
  l -= num.u;
  m -= num.m;
  u -= num.l;

  return *this;
}

TriFuzzyNum& TriFuzzyNum::operator*=(const TriFuzzyNum& num) {
  l *= num.l;
  m *= num.m;
  u *= num.u;

  correct_order();

  return *this;
}

const TriFuzzyNum TriFuzzyNum::operator-(const TriFuzzyNum& num) const {
  TriFuzzyNum result = *this;
  result -= num;

  return result;
}

const TriFuzzyNum TriFuzzyNum::operator*(const TriFuzzyNum& num) const {
  TriFuzzyNum result = *this;
  result *= num;

  return result;
}

const TriFuzzyNum TriFuzzyNum::operator+(const TriFuzzyNum& num) const {
  TriFuzzyNum result = *this;
  result += num;

  return result;
}

std::partial_ordering TriFuzzyNum::operator<=>(const TriFuzzyNum& num) const {
  return get_rank() <=> num.get_rank();
}

std::ostream& operator<<(std::ostream& os, const TriFuzzyNum& num) {
  os << '(' << num.lower_value() << ", " << num.modal_value()
     << ", " << num.upper_value() << ')';
  return os;
}

void TriFuzzyNumSet::insert(const TriFuzzyNum& num) {
  num_set.insert(num);
}

void TriFuzzyNumSet::insert(const TriFuzzyNum&& num) {
  num_set.insert(num);
}

void TriFuzzyNumSet::remove(const TriFuzzyNum& num) {
  const auto it = num_set.find(num);

  if (it != num_set.end()) {
    num_set.erase(it);
  }
}

TriFuzzyNum TriFuzzyNumSet::arithmetic_mean() const {
  size_t num_set_size = num_set.size();

  if (num_set_size == 0) {
    throw std::length_error("TriFuzzyNumSet::arithmetic_mean - the set is empty.");
  }

  TriFuzzyNum sum = crisp_zero;
  for (const TriFuzzyNum& num : num_set) {
    sum += num;
  }

  return TriFuzzyNum(sum.lower_value() / num_set_size,
                     sum.modal_value() / num_set_size,
                     sum.upper_value() / num_set_size);
}
