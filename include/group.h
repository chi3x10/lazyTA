#pragma once

#include "question.h"
#include "utils.h"

#include <cstdlib> // for srand and rand
#include <math.h>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

namespace lazyta {

using std::string;
using namespace std;

// Qestion group.
// each question group contains at least one question.
class Group {
public:
  Group();
  ~Group();
  void PushBack(Question *q);
  void SetTAMode(const bool t);
  void Shuffle();

  Question *GetQuestion(const size_t i);
  Question *GetOrderedQuestion(const size_t i);

  size_t GetNoQ() { return question_ptrs_.size(); }
  void SetShuffle(const bool t) { enable_shuffle_ = t; }
  void SetDrawTopBar(const bool l) { draw_top_bar_ = l; }

  Question *GetLastQuestion() {
    if (question_ptrs_.size() == 0) {
      return nullptr;
    }
    return *question_ptrs_.end();
  }

  friend ostream &operator<<(ostream &out, const Group &g);

  void SetFigure(const string &s, const bool before_group) {
    if (before_group) {
      fig_before_ = s;
    } else {
      fig_after_ = s;
    }
  }
  void SetText(const string &s, const bool before_group) {
    if (before_group) {
      text_before_ = s;
    } else {
      text_after_ = s;
    }
  }
private:
  static constexpr size_t kMaxQuestionsPerGroup = 200;

  // The figure or extra latex codes inserted before/after this question group
  string fig_before_;
  string fig_after_;
  // Texts inserted before/after this question group
  string text_before_;
  string text_after_;

  // use to permute order of questions inside of this question group.
  std::vector<size_t> orders_;

  bool enable_shuffle_;
  std::vector<Question *> question_ptrs_;
  bool draw_top_bar_;
};

}  // namespace lazyta
