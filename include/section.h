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
  Section() { }
  // bool Load(string fn);
  void Shuffle();
  void AddGroup(Group* g) {
    orders_.push_back(group_ptrs_.size());
    group_ptrs_.push_back(g);
  }
  void PopBack() {  // cout << "Before: " << group_ptrs_.size() << endl;
    group_ptrs_.pop_back();
  }
  string GetSolution();
  Group* GetLastGroup() { return group_ptrs_[group_ptrs_.size() - 1]; }
  ~Section();
  friend ostream& operator<<(ostream& out, const Section& s);
  void SetTAMode(bool t);

  // get the number of groups
  size_t GetNoG() { return group_ptrs_.size(); }

  // Get i-th group
  Group* GetGroup(size_t i) {
    if (i >= group_ptrs_.size()) {
      return nullptr;
    }
    return group_ptrs_[i];
  }

 private:
  static constexpr size_t kMaxNumSections = 100;
  // ifstream ifs;
  std::vector<Group*> group_ptrs_;
  std::vector<size_t> orders_;
};

}  // namespace lazyta
