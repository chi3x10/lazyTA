#include "question.h"

#include <algorithm>

namespace lazyta {

using namespace std;
using std::string;

int Question::GetSolution() {
  if (has_none_of_above_ && noa_is_ans_) {
    // Note: choices_ does not contain the actual "None of the above" so
    // it's size is 1 smaller. That's why we are return the size here.
    return static_cast<int>(choices_.size());
  }
  for (size_t i = 0; i < choices_.size(); i++) {
    if (static_cast<int>(orders_[i]) == solution_index_) {
      return static_cast<int>(i);
    }
  }
  return -1;
}

void Question::AddWholeQuestion(string str) {
  bool sol = false;
  // go to the boday of the question
  size_t bbeg = str.find("<b>") + 3, bend = str.find("</b>");
  string s;

  // normal question.
  if (!utils::StringContains(str, "<ii>")) {
    // remove the <b> </b>
    s = str.substr(bbeg, bend - bbeg);
    // remove leading and tailing spaces
    utils::TrimSpaces(s);
    // cout << s << "     ooh lala" << endl << endl;
    set_question(s);
    // remove the body part
    str = str.substr(bend + 4, str.length());
  } else {
    string t = str.substr(bbeg, str.find("<ii>") - bbeg);
    // remove leading space
    utils::TrimSpaces(t);
    set_question(t);
    // keep the </b> in the end. We uddse this to terminate when <ii>'s present
    bbeg = str.find("<ii>");
    bend = str.find("</b>");
    // get the III part.
    s = str.substr(bbeg, bend - bbeg + 4);
    // get the choices part
    str = str.substr(bend + 4, str.length());
    // cout << "\n\n III part: " << s << "str: " << str << "\n\n\n";
    while (s.find("</ii>") < s.find("</b>")) {
      bbeg = s.find("<ii>") + 4;
      bend = s.find("</ii>");
      t = s.substr(bbeg, bend - bbeg);
      // Trim
      utils::TrimSpaces(t);
      AddOptionsInQBody(t);
      s = s.substr(bend + 5, s.length());
    }
  }
  // do when you can still find </c> infront of </g>
  while (str.find("</c>") < str.find("</g>")) {
    bbeg = str.find("<c>") + 3;
    bend = str.find("</c>");
    s = str.substr(bbeg, bend - bbeg);
    utils::TrimSpaces(s);

    if (utils::StringContains(s, "<ANS>")) {
      sol = true;
      // remove <ANS> from the string
      s.replace(s.find("<ANS>"), 5, "");
    } else {
      sol = false;
    }

    // handle None of the above case
    if (utils::StringContains(s, "<NOA>")) {
      AddAChoice("NOA", sol);
    } else {
      AddAChoice(s, sol);
    }
    // remove the just added choice
    str = str.substr(bend + 4, str.length());
  }

  // user defined layout style
  if (layout_style_ == Layout::FIVE_PER_ROW) {
    max_width_choices_ = 20;
    total_column_width_ = 90;
  } else if (layout_style_ == Layout::THREE_PER_ROW) {
    max_width_choices_ = 20;
    total_column_width_ = 90;
  } else if (layout_style_ == Layout::TWO_PER_ROW) {
    max_width_choices_ = 38;
    total_column_width_ = 90;
  } else if (layout_style_ == Layout::ONE_PER_ROW) {
    max_width_choices_ = 60;
    total_column_width_ = 90;
  }
}

void Question::Shuffle() {
  if (enable_shuffle_) {
    std::random_shuffle(orders_.begin(), orders_.end());
  }
}

void Question::set_label(const string &s) {
  label_ = s;
}

ostream &operator<<(ostream &out, const Question &q) {
  // print label here
  if (q.label_.length() > 0) {
    out << "%% |  " << q.label_ << endl;
  }

  if (q.TA_mode_) {
    out << "\\begin{minipage}{\\linewidth}\n\\item \\fbox{Q. " << q.q_id_ << "} "
        << q.question_ << "\\\\" << endl;
  } else {
    out << "\\begin{minipage}{\\linewidth}\n\\item " << q.question_ << "\\\\"
        << endl;
  }

  size_t n_col, n_row;
  // print III
  if (q.options_in_q_body_.size() > 0) {
    // how many columns does the table should have?
    if (q.total_width_options_in_q_body_ < 60) {
      n_col = 5;
      out << "    \\begin{tabular}{l l l l l}" << endl;
    } else if (q.max_width_options_in_q_body_ < 27) {
      n_col = 3;
      out << "    \\begin{tabular}{l l l}" << endl;
    } else if (q.max_width_options_in_q_body_ < 45) {
      n_col = 2;
      out << "    \\begin{tabular}{l l}" << endl;
    } else {
      n_col = 1;
      out << "    \\begin{tabular}{p{\\linewidth}}" << endl;
    }

    // number of rows of this table.
    n_row = static_cast<size_t>(ceil(q.options_in_q_body_.size() /
                                     static_cast<double>(n_col)));

    for (size_t j = 0; j < n_row; j++) {
      for (size_t c = 0; c < n_col; c++) {
        size_t i = j * n_col + c;
        if (i < q.options_in_q_body_.size()) {
          out << "        \\textbf{" << q.roman_numbers_[i] << "} " << q.options_in_q_body_[i];
        }

        if (c == n_col - 1) {
          out << "\\\\" << endl;
        } else {
          out << "&" << endl;
        }
      }
    }
    out << "    \\end{tabular}\\\\\n";
  }

  // how many columns does the table should have?
  if (q.total_column_width_ < 60) {
    n_col = 5;
    out << "    \\begin{tabular}{l l l l l}" << endl;
  } else if (q.max_width_choices_ < 27) {
    n_col = 3;
    out << "    \\begin{tabular}{l l l}" << endl;
  } else if (q.max_width_choices_ < 39) {
    n_col = 2;
    out << "    \\begin{tabular}{l l}" << endl;
  } else {
    n_col = 1;
    out << "    \\begin{tabular}{p{\\linewidth}}" << endl;
  }

  size_t noc = (q.has_none_of_above_) ? q.choices_.size() + 1 : q.choices_.size();
  n_row = static_cast<size_t>(ceil(noc / static_cast<double>(n_col)));

  for (size_t j = 0; j < n_row; j++) {
    for (size_t c = 0; c < n_col; c++) {
      size_t i = j * n_col + c;
      char item_i = static_cast<char>(65 + i);

      // cout << "i= " << i << ", ans = " << choices_[orders_[i]]<<endl;
      if (i < noc) {
        // If this question contains None of the Above and we are at the last choice.
        if (i == noc - 1 && q.has_none_of_above_) {
          if (q.TA_mode_ && q.noa_is_ans_) {
            if (q.max_width_choices_ > 80) {
              out << "        \\textsf{\\textbf{(" << item_i
                  << ")} $\\bigcirc$ None of the above. }";
            } else {
              out << "        \\textsf{\\textbf{(" << item_i
                  << ")} \\fbox{ None of the above.} }";
            }
          } else {
            out << "        \\textsf{\\textbf{(" << item_i
                << ")} None of the above. }";
          }
        } else {
          if (q.TA_mode_ && q.solution_index_ == static_cast<int>(q.orders_[i])) {
            if (q.max_width_choices_ > 80)
              out << "        \\textsf{\\textbf{(" << item_i << ")} $\\bigcirc$"
                  << q.choices_[q.orders_[i]] << "}";
            else
              out << "        \\textsf{\\textbf{(" << item_i << ")} \\fbox{"
                  << q.choices_[q.orders_[i]] << "}}";
          } else
            out << "        \\textsf{\\textbf{(" << item_i<< ")} "
                << q.choices_[q.orders_[i]] << "}";
        }
      }
      // cout << "c=" <<c<< ", i="<< i<<endl;
      if (c == n_col - 1) {
        out << "\\\\" << endl;
      } else {
        out << "&" << endl;
      }
    }
  }

  out << "    \\end{tabular} \n\\end{minipage}\n";
  out << "\\vskip 0.3cm\n";
  return out;
}

//-------------------------------------------------
bool Question::set_question(const string &q) {
  question_ = q;
  return true;
}
//-------------------------------------------------
bool Question::AddOptionsInQBody(const string &iii) {
  options_in_q_body_.push_back(iii);

  total_width_options_in_q_body_ += iii.length();

  if (iii.length() > max_width_options_in_q_body_) {
    max_width_options_in_q_body_ = iii.length();
  }

  return true;
}
//-------------------------------------------------
bool Question::AddAChoice(const string &choice, const bool is_solution) {
  if (choice == "NOA") {
    has_none_of_above_ = true;

    // None of the above is the correct one
    if (is_solution) {
      // solution_index_ doesn't matter when the correct ans is NOA because
      // in GetSolution we will simply return choices_.size() -1
      solution_index_ = -1;
      noa_is_ans_ = true;
    } else {
      noa_is_ans_ = false;
    }
    if (max_width_choices_ < 16) {
      max_width_choices_ = 16;
    }

    total_column_width_ += 16;
    return true;
  }

  // Push choice index.
  orders_.push_back(choices_.size());

  choices_.push_back(choice);

  size_t l = choice.length();
  total_column_width_ += l;

  if (l > max_width_choices_) {
    max_width_choices_ = l;
  }

  if (is_solution) {
    solution_index_ = static_cast<int>(choices_.size() - 1);
  }
  return true;
}

void Question::set_layout_style(const Layout l) {
  layout_style_ = l;
}

Question::Question()
  : enable_shuffle_(true)
  , max_width_options_in_q_body_(0)
  , solution_index_(-1)
  , label_("")
  , max_width_choices_(0)
  , total_column_width_(0)
  , total_width_options_in_q_body_(0)
  , has_none_of_above_(false)
  , noa_is_ans_(false)
  , TA_mode_(false)
  , layout_style_(Layout::NONE)
  , q_id_(0) {

  roman_numbers_.push_back("I.");
  roman_numbers_.push_back("II.");
  roman_numbers_.push_back("III.");
  roman_numbers_.push_back("IV.");
  roman_numbers_.push_back("V.");
  roman_numbers_.push_back("VI.");
  roman_numbers_.push_back("VII.");
  roman_numbers_.push_back("VIII.");
  roman_numbers_.push_back("IX.");
  roman_numbers_.push_back("X.");
}

}  // namespace lazyta
