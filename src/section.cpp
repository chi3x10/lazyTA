#include "section.h"

#include <algorithm>

namespace lazyta {
using namespace std;
using std::string;

string Section::GetSolution() {
  Group *gp;
  string sol = "";
  for (unsigned int i = 0; i < group_ptrs_.size(); i++) {
    gp = group_ptrs_[orders_[i]];
    for (size_t j = 0; j < gp->GetNoQ(); j++) {
      sol += string(utils::Int2String((gp->GetOrderedQuestion(j))->GetSolution()));
      if ((gp->GetOrderedQuestion(j))->GetSolution() == -1)
        cout << "Warning:   The following question does not have a key "
                "provided.\n\t---------------------------------------------"
                "\n\t|   "
             << gp->GetOrderedQuestion(j)->get_question()
             << "\n\t---------------------------------------------\n\tA '?' is "
                "generated. \n";
      // cout <<"->" << sol<< endl;
    }
  }
  return sol;
}

void Section::SetTAMode(bool t) {
  for(auto const g_ptr : group_ptrs_) {
    g_ptr->SetTAMode(t);
  }
}

ostream &operator<<(ostream &out, const Section &s) {
  Group *g;
  out << "%%Warning!!: There should not be any text between the "
         "\\begin{enumerate.....\n%%and the first \\item{... "
         "\n%% You can copy the text or the \\includegraphic lines above the "
         "\\begin{enumerate}.... line\n%% to prevent LaTEX compile error!! " << endl;

  // previous group is a multiple questions group
  bool preMG = false;
  for (unsigned int i = 0; i < s.group_ptrs_.size(); i++) {
    g = s.group_ptrs_[s.orders_[i]];
    if (preMG && g->GetNoQ() > 1)  // previous group and this group both have
                                   // more than one question
      g->SetDrawTopBar(false);
    else
      g->SetDrawTopBar(true);
    out << *g;
    preMG = (g->GetNoQ() > 1);  // this group have more than one questions ?
  }
  return out;
}

void Section::Shuffle() {
  // shuffle groups
  std::random_shuffle(orders_.begin(), orders_.end());

  // shuffle questions in each group if the group contains more than one
  // question
  Group *group;
  Question *ques;
  for (size_t i = 0; i < group_ptrs_.size(); i++) {
    group = group_ptrs_[i];
    group->Shuffle();
    for (size_t j = 0; j < group->GetNoQ(); j++) {
      // shffle choices.
      ques = group->GetQuestion(j);
      ques->Shuffle();
    }
  }
}

Section::~Section() {
  for (auto gp : group_ptrs_) {
    delete gp;
  }
  group_ptrs_.clear();
}

}  // namespace lazyta
