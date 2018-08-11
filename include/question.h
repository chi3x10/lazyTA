#pragma once

#include "utils.h"

#include <math.h>
#include <time.h>
#include <cstdlib>  // for srand and rand
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace lazyta {

using std::string;
using namespace std;

class Question {
 public:
  Question();
  void SetLabel(const string &);
  bool SetQuestion(const string &q);
  bool AddAChoice(const string &choice, const bool sol);

  bool AddIII(const string &iii);
  //     void AddNoA(){m_NOA = true; m_MaxCW = 16;};
  void SetMaxChoices(const size_t m) { m_MaxChoices = m; }
  //        void PrintQuestion(){;};
  void Shuffle();
  void SetTAMode(const bool l) { m_TAMode = l; }
  int GetSolution();
  string GetQuestionBody() { return m_Question; }
  void AddWholeQuestion(string str);
  // set question number
  void SetQN(const int n) { m_QN = n; }
  void SetLayout(const int);
  void SetAbleShuffle(const bool b) { m_AbleShuffle = b; }
  friend ostream &operator<<(ostream &out, const Question &q);

 private:
  static constexpr size_t kMaxNumChoices = 20;
  bool m_AbleShuffle;

  size_t m_NumOfChoices;

  // question
  string m_Question;
  // I, II, .. .type of question
  string m_III[10];

  // Index of the correct answer
  int m_SolIndex;

  //
  size_t m_NumOfIII;

  // question label
  string m_Label;
  // you can have up to 20 choices.
  string m_Choices[kMaxNumChoices];

  // max choice width -- used to determine the layout of choices.
  size_t m_MaxCW;
  // max III width
  size_t m_MaxIW;

  size_t m_TotalCW;
  size_t m_TotalIW;

  // contains None of the Above choice
  bool m_NOA;

  // order of the choices.
  size_t m_Orders[10];

  // maximum number choices.
  size_t m_MaxChoices;
  // Roman numbers
  string m_RN[10];

  // TA MODE enclose the correct choice in a box
  bool m_TAMode;
  // Forced layout Style, This is usefull when the options are acutally long
  // latex code that represent short setence.
  int m_LayoutStyle;

  // question number before the shuffling. used in TA mode.
  int m_QN;
};

}  // namespace lazyta
