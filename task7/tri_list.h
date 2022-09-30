// Patryk Jędrzejczak

#ifndef TRI_LIST_H
#define TRI_LIST_H

#include <ranges>
#include <functional>

#include "tri_list_concepts.h"

template<typename T>
T identity(T x) {
  return x;
}

template<typename T>
std::function<T(T)> compose(std::function<T(T)> f1, std::function<T(T)> f2) {
  return [=] (const T& x) { return f1(f2(x)); };
}

template<typename T1, typename T2, typename T3>
class tri_list {

private:

  using element_t = std::variant<T1, T2, T3>;

  std::vector<element_t> elements;

  template<typename T>
  using modifier_t = std::function<T(T)>;

  // Compositions of all functions added by modify_only() after last reset() call
  // that change tri_list's elements of corresponding type.
  modifier_t<T1> modifier1;
  modifier_t<T2> modifier2;
  modifier_t<T3> modifier3;

  template<typename T>
  const modifier_t<T>& get_modifier() const {
    if constexpr (std::same_as<T, T1>) {
      return modifier1;
    }
    else if constexpr (std::same_as<T, T2>) {
      return modifier2;
    }
    else if constexpr (std::same_as<T, T3>) {
      return modifier3;
    }
  }

  template<typename T>
  void set_modifier(const modifier_t<T>& new_modifier) {
    if constexpr (std::same_as<T, T1>) {
      modifier1 = new_modifier;
    }
    else if constexpr (std::same_as<T, T2>) {
      modifier2 = new_modifier;
    }
    else if constexpr (std::same_as<T, T3>) {
      modifier3 = new_modifier;
    }
  }

  template<typename T>
  void check_types() const {
    static_assert(std::same_as<T, T1> + std::same_as<T, T2> + std::same_as<T, T3> == 1);
  }

  class tri_list_iterator {

  private:

    std::vector<element_t>::const_iterator it_to_element;

    const tri_list& list;

  public:

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = element_t;
    using difference_type = std::ptrdiff_t;
    using pointer = element_t*;
    using reference = element_t;

    tri_list_iterator() {}

    tri_list_iterator(std::vector<element_t>::const_iterator it, const tri_list& l) : it_to_element(it), list(l) {}

    tri_list_iterator& operator=(const tri_list_iterator &other) {
      it_to_element = other.it_to_element;
      return *this;
    }

    element_t operator*() const {
      if ((*it_to_element).index() == 0) {
        return element_t(list.modifier1(std::get<T1>(*it_to_element)));
      }
      else if ((*it_to_element).index() == 1) {
        return element_t(list.modifier2(std::get<T2>(*it_to_element)));
      }
      else {
        return element_t(list.modifier3(std::get<T3>(*it_to_element)));
      }
    }

    tri_list_iterator& operator++() {
      it_to_element++;
      return *this;
    }

    tri_list_iterator operator++(int) {
      tri_list_iterator temp = *this;
      ++(*this);
      return temp;
    }

    bool operator==(const tri_list_iterator &other) const {
      return it_to_element == other.it_to_element;
    }

    bool operator!=(const tri_list_iterator &other) const {
      return !(*this == other);
    }

  }; // class tri_list_iterator

public:

  tri_list() : modifier1(identity<T1>),
               modifier2(identity<T2>),
               modifier3(identity<T3>) {}

  tri_list(std::initializer_list<element_t> list) : elements(list),
                                                    modifier1(identity<T1>),
                                                    modifier2(identity<T2>),
                                                    modifier3(identity<T3>) {}

  template<typename T>
  void push_back(const T& t) {
    check_types<T>();

    elements.push_back(element_t(t));
  }

  template<typename T, modifier<T> F>
  void modify_only(F m = F{}) {
    check_types<T>();

    set_modifier<T>(compose<T>(m, get_modifier<T>()));
  }

  template<typename T>
  void reset() {
    check_types<T>();

    set_modifier<T>(identity<T>);
  }

  template<typename T>
  auto range_over() const {
    check_types<T>();

    return elements
         | std::ranges::views::filter([&] (element_t x) {
             return std::holds_alternative<T>(x);
           } )
         | std::ranges::views::transform([&] (element_t x) {
             return get_modifier<T>()(std::get<T>(x));
           } );
  }

  tri_list_iterator begin() const {
    return tri_list_iterator(elements.begin(), *this);
  }

  tri_list_iterator end() const {
    return tri_list_iterator(elements.end(), *this);
  }

};

#endif // TRI_LIST_H