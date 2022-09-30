// autorzy: Patryk Jędrzejczak i Jowita Drozdowicz

#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <iostream>
#include <string.h>
#include <string>
#include "maptel.h"

using namespace std;

#ifdef NDEBUG
  static const bool debug = false;
#else
  static const bool debug = true;

  /* ****  FUNKCJE DIAGNOSTYCZNE **** */
  namespace {

    bool correct_chars(char const *tel) {
      for (size_t i = 0; i < strlen(tel); i++) {
        if (tel[i] < '0' || tel[i] > '9') {
          return false;
        }
      }
      return true;
    }

    // Sprawdza, czy numer telefonu tel spełnia założenia z treści.
    bool correct_tel(char const *tel) {
      if (tel == NULL) {
        return false;
      }

      size_t len = strnlen(tel, jnp1::TEL_NUM_MAX_LEN);
      return len > 0 && tel[len] == '\0' && correct_chars(tel);
    }
    
  }

#endif

/* ************** FUNKCJE I ZMIENNE POMOCNICZE ************* */
namespace {

  using tel_num = string;
  using tel_changes = unordered_map<tel_num, tel_num>;
  using tel_changes_dict = unordered_map<unsigned long, tel_changes>;

  tel_changes_dict& get_main_dict() {
    static tel_changes_dict main_dict;
    return main_dict;
  }

  // Zwraca ostatni numer w ciągu zmian. Jeśli nie ma zmiany numeru lub zmiany
  // tworzą cykl, zwraca start_tel.
  tel_num& find_final_tel(
    tel_changes& changes,
    tel_num& start_tel,
    tel_num& curr_tel,
    bool &is_cycle,
    unordered_set<tel_num> visited
  ) {
    auto curr_tel_it = changes.find(curr_tel);
    if (curr_tel_it == changes.end()) {
      return curr_tel;
    }

    tel_num& next_tel = curr_tel_it->second;

    if (visited.find(next_tel) != visited.end()) {
      is_cycle = true;
      return start_tel;
    }
    
    visited.insert(next_tel);
    return find_final_tel(changes, start_tel, next_tel, is_cycle, visited);
  }

}

/* **************** FUNKCJE Z INTERFEJSU **************** */

unsigned long jnp1::maptel_create(void) {
  static unsigned long next_tel_changes_id = 0;

  if (debug) {
    cerr << "maptel: maptel_create()\n";
    cerr << "maptel: maptel_create: new map id = " << next_tel_changes_id << '\n';
  }

  get_main_dict()[next_tel_changes_id] = tel_changes();
  
  return next_tel_changes_id++;
}

void jnp1::maptel_delete(unsigned long id) {
  auto id_it = get_main_dict().find(id);

  if (debug) {
    cerr << "maptel: maptel_delete(" << id << ")\n";
    assert(id_it != get_main_dict().end());
    cerr << "maptel: maptel_delete: map " << id << " deleted\n";
  }

  get_main_dict().erase(id_it);
}

void jnp1::maptel_insert(unsigned long id, char const *tel_src, char const *tel_dst) {
  auto id_it = get_main_dict().find(id);

  if (debug) {
    cerr << "maptel: maptel_insert(";
    cerr << id << ", " << tel_src << ", " << tel_dst << ")\n";
    assert(id_it != get_main_dict().end());
    assert(correct_tel(tel_src));
    assert(correct_tel(tel_dst));
    cerr << "maptel: maptel_insert: inserted\n";
  }

  tel_num from(tel_src);
  tel_num to(tel_dst);

  id_it->second[from] = to;
}

void jnp1::maptel_erase(unsigned long id, char const *tel_src) {
  auto id_it = get_main_dict().find(id);

  if (debug) {
    cerr << "maptel: maptel_erase(" << id << ", " << tel_src << ")\n";
    assert(id_it != get_main_dict().end());
    assert(correct_tel(tel_src));
  }

  auto tel_src_it = id_it->second.find(tel_src);

  if (tel_src_it != id_it->second.end()) {
    id_it->second.erase(tel_src_it);
    if (debug) {
      cerr << "maptel: maptel_erase: erased\n";
    }
  }
  else if (debug) {
    cerr << "maptel: maptel_erase: nothing to erase\n";
  }
}

void jnp1::maptel_transform(unsigned long id, char const *tel_src, char *tel_dst, size_t len) {
  auto id_it = get_main_dict().find(id);

  if (debug) {
    cerr << "maptel: maptel_transform(";
    cerr  << id << ", " << tel_src << ", " << &tel_dst << ", " << len << ")\n";
    assert(id_it != get_main_dict().end());
    assert(correct_tel(tel_src));
    assert(tel_dst != NULL);
  }
  
  bool is_cycle = false;
  unordered_set<tel_num> visited;

  tel_num tel_src_str(tel_src);
  visited.insert(tel_src_str);
  tel_num final_tel = find_final_tel(id_it->second, tel_src_str, tel_src_str, is_cycle, visited);

  assert(final_tel.length() < len);
  strcpy(tel_dst, final_tel.c_str());
   
  if (debug) {
    if (is_cycle) {
      cerr << "maptel: maptel_transform: cycle detected\n";
    }
    cerr << "maptel: maptel_transform: " << tel_src << " -> " << tel_dst << "\n";
  }
}
