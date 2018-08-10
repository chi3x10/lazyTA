#ifndef __INCLUDE_QUESTION_H__
#define __INCLUDE_QUESTION_H__

#include <math.h>
#include <time.h>
#include <cstdlib>  // for srand and rand
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using namespace std;

string ConvertSol(string sol);
string Int2String(int v);
bool Roll(int n, int m);
bool IsNumber(const std::string &s);
bool StringContains(string s, string k);
void TrimSpaces(string &str);
void ShuffleIndex(size_t *index, size_t num);

class Question {
 public:
  Question();
  void SetLabel(const string &);
  bool SetQuestion(string q);
  bool AddAChoice(string c, bool sol);
  bool AddIII(string iii);
  //     void AddNoA(){m_NOA = true; m_MaxCW = 16;};
  void SetMaxChoices(int m) { m_MaxChoices = m; }
  //        void PrintQuestion(){;};
  void Shuffle();
  void SetTAMode(bool l) { m_TAMode = l; }
  int GetSolution();
  string GetQuestionBody() { return m_Question; }
  void AddWholeQuestion(string str);
  // set question number
  void SetQN(int n) { m_QN = n; }
  void SetLayout(int);
  void SetAbleShuffle(bool b) { m_AbleShuffle = b; }
  friend ostream &operator<<(ostream &out, const Question &q);

 private:
  static constexpr size_t kMaxNumChoices = 20;
  bool m_AbleShuffle;
  // question
  string m_Question;
  // I, II, .. .type of question
  string m_III[10];
  //
  size_t m_NumOfIII;

  // question label
  string m_Label;
  // you can have up to 20 choices.
  string m_Choices[kMaxNumChoices];
  //
  size_t m_NumOfChoices;
  // Index of the correct answer
  size_t m_SolIndex;

  // question type: 1: regular, 2: I, II, III,...
  // int m_QType;

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
  int m_MaxChoices;
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

#endif  // __INCLUDE_QUESTION_H__
