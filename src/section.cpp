#include "section.h"

using namespace std;
using std::string;

string Section::GetSolution() {
  Group *gp;
  string sol = "";
  for (unsigned int i = 0; i < m_Groups.size(); i++) {
    gp = m_Groups[m_Orders[i]];
    for (int j = 0; j < gp->GetNoQ(); j++) {
      sol += string(Int2String((gp->GetOrderedQuestion(j))->GetSolution()));
      if ((gp->GetOrderedQuestion(j))->GetSolution() == -1)
        cout << "Warning:   The following question does not have a key "
                "provided.\n\t---------------------------------------------"
                "\n\t|   "
             << gp->GetOrderedQuestion(j)->GetQuestionBody()
             << "\n\t---------------------------------------------\n\tA '?' is "
                "generated. \n";
      // cout <<"->" << sol<< endl;
    }
  }
  return sol;
}
//----------------------------------------------------------------------------
void Section::SetTAMode(bool t) {
  for (unsigned int i = 0; i < m_Groups.size(); i++) m_Groups[i]->SetTAMode(t);
}
//----------------------------------------------------------------------------
ostream &operator<<(ostream &out, const Section &s) {
  Group *g;
  out << "\%\%Warning!!: There should not be any text between the "
         "\\begin{enumerate.....\n\%%and the first \\item{... "
      << "\n%% You can copy the text or the \\includegraphic lines above the "
         "\\begin{enumerate}.... line\n"
      << "%% to prevent LaTEX compile error!! " << endl;
  // previous group is a multiple questions group
  bool preMG = false;
  for (unsigned int i = 0; i < s.m_Groups.size(); i++) {
    g = s.m_Groups[s.m_Orders[i]];
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
//----------------------------------------------------------------------------
void Section::Shuffle() {
  // shuffle groups
  ShuffleIndex(m_Orders, m_Groups.size());
  // shuffle questions in each group if the group contains more than one
  // question
  Group *group;
  Question *ques;
  for (unsigned int i = 0; i < m_Groups.size(); i++) {
    group = m_Groups[i];
    group->Shuffle();
    for (int j = 0; j < group->GetNoQ(); j++) {
      // shffle choices.
      ques = group->GetQuestion(j);
      ques->Shuffle();
    }
  }
}
//----------------------------------------------------------------------------
/*bool Section::Load(string fn)
{
    //ifs.open(fn.c_str());
    return true;
}*/
//-----------------------------------------------------------------------------
Section::Section() {
  for (int i = 0; i < 100; i++) m_Orders[i] = i;
}
Section::~Section() {
  Group *gp;
  for (unsigned int i = 0; i < m_Groups.size(); i++) {
    gp = m_Groups[i];
    delete gp;
  }
  m_Groups.clear();
}
//-----------------------------------------------------------------------------
// Question* Group::GetQuestion(int i)
//{
//    if(i>= m_Questions.size())
//        return NULL;

//  return m_Questions[m_Orders[i]];
//}
