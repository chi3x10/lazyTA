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
  enum class Layout {
    NONE = -1,
    DEFAULT = 0,
    FIVE_PER_ROW = 1,
    THREE_PER_ROW = 2,   // One choice per row.
    TWO_PER_ROW = 3,
    ONE_PER_ROW = 4
  };
  Question();

  void Shuffle();

  bool AddAChoice(const string &choice, const bool is_solution);

  int GetSolution();

  bool AddOptionsInQBody(const string &iii);

  void AddWholeQuestion(string str);

  void set_noa_position(const size_t m) { noa_position_ = m; }
  void set_TA_mode(const bool l) { TA_mode_ = l; }
  string get_question() { return question_; }
  std::vector<std::string> get_options_in_q_body() {
    return options_in_q_body_;
  }
  void set_label(const string &label);
  bool set_question(const string &q);

  // set question number
  void set_q_id(const int n) { q_id_ = n; }
  void set_layout_style(const Layout l);
  void set_enable_shuffle(const bool b) { enable_shuffle_ = b; }
  friend ostream &operator<<(ostream &out, const Question &q);

 private:
  static constexpr size_t kMaxNumChoices = 20;

  bool enable_shuffle_;

  // question body.
  string question_;

  // This is to store the options in question body. For example, the lines between <ii> and </ii>
  // below are options in question body.
  /*<q>
    <b>Which of the following is/are correct?
    <ii>An endnote is placed at the end of each paragraph.</ii>
    <ii>A footnote is placed at the end of a page.</ii>
    <ii>You can convert a footnote to an endnote.</ii>
    <ii>Footnotes are numbered automatically.</ii></b>
    <c> II and III</c>
    <c> All of above.</c>
    <c> II only.</c>
    <c> II and IV.</c>
    <c> II, III and IV. <ANS></c>
    </q>
  */
  std::vector<string> options_in_q_body_;
  // max width of the options in question body.
  size_t max_width_options_in_q_body_;

  // Index of the correct answer
  int solution_index_;

  // question label
  string label_;

  // Choices of a question.
  std::vector<string> choices_;
  // Indices of the choices. Used for shuffling.
  std::vector<size_t> orders_;

  // max choice width -- used to determine the layout of choices.
  size_t max_width_choices_;

  size_t total_column_width_;
  size_t total_width_options_in_q_body_;

  // contains None of the Above choice
  bool is_none_of_above_;

  // Where to put the None of the above choice. Currently it's hardcoded to 5. TODO():
  // Need to figure out a way to always to None of the above in the last choice.
  size_t noa_position_;
  // Roman numbers
  //std::vector<string> roman_numbers_;
  std::vector<string> roman_numbers_;

  // TA MODE enclose the correct choice in a box
  bool TA_mode_;
  // Forced layout Style, This is usefull when the options are acutally long
  // latex code that represent short setence.
  Layout layout_style_;

  // question number before the shuffling. used in TA mode.
  int q_id_;
};

}  // namespace lazyta
