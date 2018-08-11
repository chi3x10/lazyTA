#pragma once

#include <math.h>
#include <time.h>
#include <cstdlib>  // for srand and rand
#include <sstream>
#include <string>
#include <vector>
#include "group.h"

namespace lazyta {

class Section {
 public:
  Section();
  // bool Load(string fn);
  void Shuffle();
  void AddGroup(Group* g) { m_Groups.push_back(g); }
  void PopBack() {  // cout << "Before: " << m_Groups.size() << endl;
    m_Groups.pop_back();
  }
  string GetSolution();
  Group* GetLastGroup() { return m_Groups[m_Groups.size() - 1]; }
  ~Section();
  friend ostream& operator<<(ostream& out, const Section& s);
  void SetTAMode(bool t);
  // get the number of groups
  size_t GetNoG() { return m_Groups.size(); }
  // Get i-th group
  Group* GetGroup(size_t i) {
    if (i >= m_Groups.size()) return nullptr;
    return m_Groups[i];
  }

 private:
  static constexpr size_t kMaxNumSections = 100;
  // ifstream ifs;
  std::vector<Group*> m_Groups;
  size_t m_Orders[kMaxNumSections];
};

}  // namespace lazyta
