#include "group.h"

#include <algorithm>

namespace lazyta {

using namespace std;
using std::string;

Group::Group()
  : fig_before_("")
  , fig_after_("")
  , text_before_("")
  , text_after_("")
  , enable_shuffle_(true)
  , draw_top_bar_(true) {
}

Group::~Group() {
  for (auto q : question_ptrs_) {
    delete q;
  }
  question_ptrs_.clear();
}

Question *Group::GetQuestion(const size_t i) {
  if (i >= question_ptrs_.size()) {
    return nullptr;
  }
  return question_ptrs_[i];
}

Question *Group::GetOrderedQuestion(const size_t i) {
  if (i >= question_ptrs_.size()) {
    return nullptr;
  }
  return question_ptrs_[orders_[i]];
}

ostream &operator<<(ostream &out, const Group &g) {

  // horizontal bar
  if (g.question_ptrs_.size() > 1 && g.draw_top_bar_)
    out << "\n%%========================================================"
           "\n\\hspace{-1in} \\rule{8in}{1pt}\n";

  if (g.text_before_ != "")
    out << "%%Texts here are inserted before this group!! \n\n"
        << g.text_before_ << endl;
  if (g.fig_before_ != "")
    out << "%% A Figure here is inserted before this group!! \n"
        << g.fig_before_ << endl;

  for (auto const ith : g.orders_) {
    out << "%% ----------------------------------------------------" << endl;
    out << *(g.question_ptrs_[ith]);
  }

  if (g.fig_after_ != "")
    out << "%% A Figure here is inserted AFTER this group!! \n"
        << g.fig_after_ << endl;
  if (g.text_after_ != "")
    out << "%%Texts here are inserted AFTER this group!! \n\n"
        << g.text_after_ << endl;
  // bottom horizontal bar
  if (g.question_ptrs_.size() > 1)
    out << "\n\\hspace{-1in} "
           "\\rule{8in}{1pt}\n%%==============================================="
           "============\n";
  return out;
}

void Group::Shuffle() {
  if (!enable_shuffle_) {
    return;
  }
  orders_.clear();
  for(size_t i = 0; i < question_ptrs_.size(); ++i){
    orders_.push_back(i);
  }
  std::random_shuffle(orders_.begin(), orders_.end());
}

void Group::SetTAMode(const bool t) {
  for (auto const q : question_ptrs_) {
    q->set_TA_mode(t);
  }
}

void Group::PushBack(Question *q) {
  orders_.push_back(question_ptrs_.size());
  question_ptrs_.push_back(q);
}

}  // namespace lazyta
