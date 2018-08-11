#include "question.h"

namespace lazyta {

using namespace std;
using std::string;

int Question::GetSolution() {
  for (int i = 0; i < 10; i++) {
    if (static_cast<int>(m_Orders[i]) == m_SolIndex) {
      return i;
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
    SetQuestion(s);
    // remove the body part
    str = str.substr(bend + 4, str.length());
  } else {
    string t = str.substr(bbeg, str.find("<ii>") - bbeg);
    // remove leading space
    utils::TrimSpaces(t);
    SetQuestion(t);
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
      AddIII(t);
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
    if (utils::StringContains(s, "<NOA>"))
      AddAChoice("NOA", sol);
    else
      AddAChoice(s, sol);
    // remove the just added choice
    str = str.substr(bend + 4, str.length());
    // cout << "Choice : "<< s << endl;
  }

  // user defined layout style
  if (m_LayoutStyle == 1)
    m_TotalCW = 0;
  else if (m_LayoutStyle == 2) {
    m_MaxCW = 20;
    m_TotalCW = 90;
  } else if (m_LayoutStyle == 3) {
    m_MaxCW = 38;
    m_TotalCW = 90;
  } else if (m_LayoutStyle == 4) {
    m_MaxCW = 100;
    m_TotalCW = 90;
  }
}

void Question::Shuffle() {
  // int noc = (m_NOA)?m_NumOfChoices+1:m_NumOfChoices;
  if (m_AbleShuffle) {
    utils::ShuffleIndex(m_Orders, m_NumOfChoices);
  }
}

void Question::SetLabel(const string &s) {
  m_Label = s;
}

ostream &operator<<(ostream &out, const Question &q) {
  // print label here
  if (q.m_Label.length() > 0) {
    out << "%% |  " << q.m_Label << endl;
  }

  if (q.m_TAMode) {
    out << "\\begin{minipage}{\\linewidth}\n\\item \\fbox{Q. " << q.m_QN << "} "
        << q.m_Question << "\\\\" << endl;
  } else {
    out << "\\begin{minipage}{\\linewidth}\n\\item " << q.m_Question << "\\\\"
        << endl;
  }

  size_t n_col, n_row;
  // print III
  if (q.m_NumOfIII > 0) {
    // how many columns does the table should have?
    if (q.m_TotalIW < 60) {
      n_col = 5;
      out << "    \\begin{tabular}{l l l l l}" << endl;
    } else if (q.m_MaxIW < 27) {
      n_col = 3;
      out << "    \\begin{tabular}{l l l}" << endl;
    } else if (q.m_MaxIW < 45) {
      n_col = 2;
      out << "    \\begin{tabular}{l l}" << endl;
    } else {
      n_col = 1;
      out << "    \\begin{tabular}{p{\\linewidth}}" << endl;
    }

    // number of rows of this table.
    n_row = static_cast<size_t>(ceil(q.m_NumOfIII / static_cast<double>(n_col)));

    // cout << "maxiw = " << m_MaxIW << endl;;
    for (size_t j = 0; j < n_row; j++) {
      for (size_t c = 0; c < n_col; c++) {
        size_t i = j * n_col + c;
        if (i < q.m_NumOfIII)
          out << "        \\textbf{" << q.m_RN[i] << "} " << q.m_III[i];
        // cout << "c=" <<c<< ", i="<< i<<endl;
        if (c == n_col - 1)
          out << "\\\\" << endl;
        else
          out << "&" << endl;
      }
    }
    out << "    \\end{tabular}\\\\\n";
    //        out << "    \\end{tabular}\\\n";
  }

  // how many columns does the table should have?
  if (q.m_TotalCW < 60) {
    n_col = 5;
    out << "    \\begin{tabular}{l l l l l}" << endl;
  } else if (q.m_MaxCW < 27) {
    n_col = 3;
    out << "    \\begin{tabular}{l l l}" << endl;
  } else if (q.m_MaxCW < 39) {
    n_col = 2;
    out << "    \\begin{tabular}{l l}" << endl;
  } else {
    n_col = 1;
    out << "    \\begin{tabular}{p{\\linewidth}}" << endl;
  }

  size_t noc = (q.m_NOA) ? q.m_NumOfChoices + 1 : q.m_NumOfChoices;
  n_row = static_cast<size_t>(ceil(noc / static_cast<double>(n_col)));

  char a;
  if (noc < q.m_MaxChoices) {
    cout << "ERROR! There are less than 5 choices for this question: \n"
         << q.m_Question << endl;
  }

  for (size_t j = 0; j < n_row; j++) {
    for (size_t c = 0; c < n_col; c++) {
      size_t i = j * n_col + c;
      a = static_cast<char>(65 + i);

      // cout << "i= " << i << ", ans = " << m_Choices[m_Orders[i]]<<endl;
      if (i < noc) {
        if (i == noc - 1 && q.m_NOA) {
          //                    cout << "HAHA: " << noc  << endl;
          if (q.m_TAMode && q.m_SolIndex == static_cast<int>(q.m_Orders[i])) {
            if (q.m_MaxCW > 80)
              out << "        \\textsf{\\textbf{(" << a
                  << ")} $\\bigcirc$ None of the above. }";
            else
              out << "        \\textsf{\\textbf{(" << a
                  << ")} \\fbox{ None of the above.} }";
          } else
            out << "        \\textsf{\\textbf{(" << a
                << ")} None of the above. }";
        } else {
          if (q.m_TAMode && q.m_SolIndex == static_cast<int>(q.m_Orders[i])) {
            if (q.m_MaxCW > 80)
              out << "        \\textsf{\\textbf{(" << a << ")} $\\bigcirc$"
                  << q.m_Choices[q.m_Orders[i]] << "}";
            else
              out << "        \\textsf{\\textbf{(" << a << ")} \\fbox{"
                  << q.m_Choices[q.m_Orders[i]] << "}}";
          } else
            out << "        \\textsf{\\textbf{(" << a << ")} "
                << q.m_Choices[q.m_Orders[i]] << "}";
        }
      }
      // cout << "c=" <<c<< ", i="<< i<<endl;
      if (c == n_col - 1)
        out << "\\\\" << endl;
      else
        out << "&" << endl;
    }
  }

  // when number of columns is not equal 5, put an extra row in the end of the
  // table.
  // after using the minipage enviorement, the space between the end of table
  // and next question
  // became just way too small. This only happens when n_col != 5
  //    if(n_col==1)
  //        out << "  \\\\ \n";
  //    else if(n_col ==3)
  //        out << "&   &   \\\\" << endl;
  //    else if(n_col == 2)
  //        out << "&    \\\\" << endl;

  out << "    \\end{tabular} \n\\end{minipage}\n";
  out << "\\vskip 0.3cm\n";
  return out;
}

//-------------------------------------------------
bool Question::SetQuestion(const string &q) {
  m_Question = q;
  return true;
}
//-------------------------------------------------
bool Question::AddIII(const string &iii) {
  m_III[m_NumOfIII] = iii;
  m_NumOfIII++;

  m_TotalIW += iii.length();

  if (iii.length() > m_MaxIW) {
    m_MaxIW = iii.length();
  }

  return true;
}
//-------------------------------------------------
bool Question::AddAChoice(const string &choice, const bool sol) {
  if (choice == "NOA") {
    m_NOA = true;

    // None of the above is the correct one
    if (sol) {
      m_SolIndex = static_cast<int>(m_MaxChoices - 1);
    }
    if (m_MaxCW < 16) {
      m_MaxCW = 16;
    }

    m_TotalCW += 16;
    return true;
  }

  m_Choices[m_NumOfChoices] = choice;

  size_t l = choice.length();
  m_TotalCW += l;

  if (l > m_MaxCW) {
    m_MaxCW = l;
  }

  if (sol) {
    m_SolIndex = static_cast<int>(m_NumOfChoices);
  }
  m_NumOfChoices++;

  return true;
}

void Question::SetLayout(const int i) {
  m_LayoutStyle = i;
}

Question::Question()
  : m_NumOfChoices(0)
  , m_SolIndex(-1)
  , m_NumOfIII(0)
  , m_NOA(false)
  , m_MaxChoices(5) {

  m_MaxCW = 0;
  m_TotalCW = 0;
  m_MaxIW = 0;
  m_TotalIW = 0;
  m_TAMode = false;

  m_LayoutStyle = 0;

  m_AbleShuffle = true;

  m_QN = 0;

  m_RN[0] = "I.";
  m_RN[1] = "II.";
  m_RN[2] = "III.";
  m_RN[3] = "IV.";
  m_RN[4] = "V.";
  m_RN[5] = "VI.";
  m_RN[6] = "VII.";
  m_RN[7] = "VIII.";
  m_RN[8] = "IX.";
  m_RN[9] = "X.";
  m_Label = "";

  for (size_t i = 0; i < 10; i++) {
    m_Orders[i] = i;
  }
}

}  // namespace lazyta
