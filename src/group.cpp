#include "group.h"

namespace lazyta {

using namespace std;
using std::string;

Group::~Group() { // cout << "Destructor visited! " << m_Questions.size() <<
                  // endl << endl;
  for (unsigned int i = 0; i < m_Questions.size(); i++) {
    delete m_Questions[i];
  }
  m_Questions.clear();
}

Question *Group::GetQuestion(const size_t i) {
  if (i >= m_Questions.size()) {
    return nullptr;
  }
  return m_Questions[i];
}

Question *Group::GetOrderedQuestion(const size_t i) {
  if (i >= m_Questions.size()) {
    return nullptr;
  }
  return m_Questions[m_Orders[i]];
}

ostream &operator<<(ostream &out, const Group &g) {
  //    cout << "The begining of group operator << "<< endl;

  // horizontal bar
  if (g.m_Questions.size() > 1 && g.m_DrawTopBar)
    out << "\n%%========================================================"
           "\n\\hspace{-1in} \\rule{8in}{1pt}\n";

  if (g.m_TextBefore != "")
    out << "%%Texts here are inserted before this group!! \n\n"
        << g.m_TextBefore << endl;
  if (g.m_FigBefore != "")
    out << "%% A Figure here is inserted before this group!! \n"
        << g.m_FigBefore << endl;

  // print question
  for (unsigned int i = 0; i < g.m_Questions.size(); i++) {
    out << "%% ----------------------------------------------------" << endl;
    out << *(g.m_Questions[g.m_Orders[i]]);
  }
  if (g.m_FigAfter != "")
    out << "%% A Figure here is inserted AFTER this group!! \n"
        << g.m_FigAfter << endl;
  if (g.m_TextAfter != "")
    out << "%%Texts here are inserted AFTER this group!! \n\n"
        << g.m_TextAfter << endl;
  // bottom horizontal bar
  if (g.m_Questions.size() > 1)
    out << "\n\\hspace{-1in} "
           "\\rule{8in}{1pt}\n%%==============================================="
           "============\n";
  return out;
}

//-----------------------------------------------------------------------------
void Group::Shuffle() {
  if (enable_shuffle_) {
    utils::ShuffleIndex(m_Orders, m_Questions.size());
  }
}
//-----------------------------------------------------------------------------
void Group::SetTAMode(const bool t) {
  for (unsigned int i = 0; i < m_Questions.size(); i++) {
    m_Questions[i]->set_TA_mode(t);
  }
}
//-----------------------------------------------------------------------------
void Group::PushBack(Question *q) { m_Questions.push_back(q); }
//-----------------------------------------------------------------------------
Group::Group() {
  m_FigBefore = "";
  m_FigAfter = "";
  m_TextBefore = "";
  m_TextAfter = "";
  enable_shuffle_ = true;
  for (size_t i = 0; i < 100; i++) {
    m_Orders[i] = i;
  }
  m_DrawTopBar = true;
}

}  // namespace lazyta
