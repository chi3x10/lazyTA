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

  size_t GetNoQ() { return m_Questions.size(); }
  void SetShuffle(const bool t) { m_AbleShuffle = t; }
  void SetDrawTopBar(const bool l) { m_DrawTopBar = l; }
  // must contain at least one question
  Question *GetLastQuestion() { return m_Questions[m_Questions.size() - 1]; }
  friend ostream &operator<<(ostream &out, const Group &g);
  void SetFigure(const string &s, const bool before_group) {
    if (before_group) {
      m_FigBefore = s;
    } else {
      m_FigAfter = s;
    }
  }
  void SetText(const string &s, const bool before_group) {
    if (before_group) {
      m_TextBefore = s;
    } else {
      m_TextAfter = s;
    }
  }
private:
  static constexpr size_t kMaxQuestions = 200;

  // The figure or extra latex codes inserted before/after this question group
  string m_FigBefore;
  string m_FigAfter;
  // Texts inserted before/after this question group
  string m_TextBefore;
  string m_TextAfter;

  // TA modes will generate
  bool m_TAMode;

  // use to permute order of questions inside of this question group.
  size_t m_Orders[kMaxQuestions];
  bool m_AbleShuffle;
  vector<Question *> m_Questions;
  bool m_DrawTopBar;
};

}  // namespace lazyta
