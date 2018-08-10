#include "question.h"

using namespace std;
using std::string;

string ConvertSol(string sol) {
  // cout << "Answer key:" << endl;
  // cout << sol << endl;

  string solA = "";
  char ai[2];
  ai[1] = '\0';
  char a;
  for (unsigned int i = 0; i < sol.length(); i++) {
    if (sol[i] == '-') {
      solA += "?";
      i++;
      // cout << "ININ \n";
    } else {
      ai[0] = sol[i];
      a = atoi(ai) + 65;
      ai[0] = a;
      solA += string(ai);
    }
  }
  // cout << "solA = " << solA << endl;
  // cout << solA << endl;
  string solB = "";
  for (unsigned int i = 0; i < solA.length(); i += 5) {
    solB += solA.substr(i, 5) + "\t";
  }
  return solB;
}
string Int2String(int v) {
  std::stringstream out;
  out << v;
  return out.str();
}

bool Roll(int n, int m) {
  int a = rand() % m + 1;
  cout << "Time(NULL)= " << time(nullptr) << ", Dice rolled: " << a << " from "
       << n << "/" << m << "\n\n";
  return (a <= n);
}
bool IsNumber(const std::string &s) {
  for (unsigned int i = 0; i < s.length(); i++) {
    if (!std::isdigit(s[i])) return false;
  }
  return true;
}

bool StringContains(string s, string k) {
  //    if(k=="<ii>")
  //        cout << "\n\n\nI am checking III here \n\n\n" << s <<
  //        (s.find(k)<s.size())  << endl << endl;;
  return (s.find(k) < s.size());
}
void TrimSpaces(string &str) {
  // Trim Both leading and trailing spaces
  size_t startpos = str.find_first_not_of(" ");  // Find the first character
                                                 // position after excluding
                                                 // leading blank spaces
  size_t endpos = str.find_last_not_of(
      " ");  // Find the first character position from reverse af

  // if all spaces or empty return an empty string
  if ((string::npos == startpos) || (string::npos == endpos)) {
    str = "";
  } else
    str = str.substr(startpos, endpos - startpos + 1);
}

void ShuffleIndex(size_t *index, size_t num) {
  // put the in order first
  for (size_t i = 0; i < num; i++) {
    index[i] = i;
  }
  // randomly swap with another one;
  for (size_t i = 0; i < num; i++) {
    size_t j = static_cast<size_t>(rand()) % num;
    std::swap(index[i], index[j]);
  }
}

int Question::GetSolution() {
  for (int i = 0; i < 10; i++)
    if (m_Orders[i] == m_SolIndex) return i;
  return -1;
}

void Question::AddWholeQuestion(string str) {
  //
  // cout << "Inside AddWholeQuestion : " << str <<endl;
  bool sol = false;
  // go to the boday of the question
  int bbeg = str.find("<b>") + 3, bend = str.find("</b>");
  string s;

  // normal question.
  if (!StringContains(str, "<ii>")) {
    // remove the <b> </b>
    s = str.substr(bbeg, bend - bbeg);
    // remove leading and tailing spaces
    TrimSpaces(s);
    // cout << s << "     ooh lala" << endl << endl;
    SetQuestion(s);
    // remove the body part
    str = str.substr(bend + 4, str.length());
  } else {
    string t = str.substr(bbeg, str.find("<ii>") - bbeg);
    // remove leading space
    TrimSpaces(t);
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
      TrimSpaces(t);
      AddIII(t);
      s = s.substr(bend + 5, s.length());
    }
  }
  // do when you can still find </c> infront of </g>
  while (str.find("</c>") < str.find("</g>")) {
    bbeg = str.find("<c>") + 3;
    bend = str.find("</c>");
    s = str.substr(bbeg, bend - bbeg);
    TrimSpaces(s);
    if (StringContains(s, "<ANS>")) {
      sol = true;
      // remove <ANS> from the string
      s.replace(s.find("<ANS>"), 5, "");
    } else
      sol = false;

    // handle None of the above case
    if (StringContains(s, "<NOA>"))
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

//-----------------------------------------------------------------------------
void Question::Shuffle() {
  // int noc = (m_NOA)?m_NumOfChoices+1:m_NumOfChoices;
  if (m_AbleShuffle) {
    ShuffleIndex(m_Orders, m_NumOfChoices);
  }
}
//-------------------------------------------------------------
void Question::SetLabel(const string &s) { m_Label = s; }
//-------------------------------------------------
ostream &operator<<(ostream &out, const Question &q) {
  // print label here
  if (q.m_Label.length() > 0) out << "%% |  " << q.m_Label << endl;
  //    out << "\\begin{minipage}{\\textwidth}\n\\item " << q.m_Question
  //    <<"\\\\" << endl;
  //

  if (q.m_TAMode)
    out << "\\begin{minipage}{\\linewidth}\n\\item \\fbox{Q. " << q.m_QN << "} "
        << q.m_Question << "\\\\" << endl;
  else
    out << "\\begin{minipage}{\\linewidth}\n\\item " << q.m_Question << "\\\\"
        << endl;

  int nc, nr, i;
  // print III
  if (q.m_NumOfIII > 0) {
    // how many columns does the table should have?
    if (q.m_TotalIW < 60) {
      nc = 5;
      out << "    \\begin{tabular}{l l l l l}" << endl;
    } else if (q.m_MaxIW < 27) {
      nc = 3;
      out << "    \\begin{tabular}{l l l}" << endl;
    } else if (q.m_MaxIW < 45) {
      nc = 2;
      out << "    \\begin{tabular}{l l}" << endl;
    } else {
      nc = 1;
      out << "    \\begin{tabular}{p{\\linewidth}}" << endl;
    }

    // number of rows of this table.
    nr = (int)ceil((double)q.m_NumOfIII / (double)nc);

    // cout << "maxiw = " << m_MaxIW << endl;;
    for (int j = 0; j < nr; j++) {
      for (int c = 0; c < nc; c++) {
        i = j * nc + c;
        if (i < q.m_NumOfIII)
          out << "        \\textbf{" << q.m_RN[i] << "} " << q.m_III[i];
        // cout << "c=" <<c<< ", i="<< i<<endl;
        if (c == nc - 1)
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
    nc = 5;
    out << "    \\begin{tabular}{l l l l l}" << endl;
  } else if (q.m_MaxCW < 27) {
    nc = 3;
    out << "    \\begin{tabular}{l l l}" << endl;
  } else if (q.m_MaxCW < 39) {
    nc = 2;
    out << "    \\begin{tabular}{l l}" << endl;
  } else {
    nc = 1;
    out << "    \\begin{tabular}{p{\\linewidth}}" << endl;
  }

  int noc = (q.m_NOA) ? q.m_NumOfChoices + 1 : q.m_NumOfChoices;
  nr = (int)ceil((double)noc / (double)nc);

  char a;
  //  cout << "NOC = " << noc<< "nc=" << nc << ", nr=" << nr <<endl << endl;
  if (noc < q.m_MaxChoices) {
    cout << "ERROR! There are less than 5 choices for this question: \n"
         << q.m_Question << endl;
  }
  // cout << "maxiw = " << m_MaxIW << endl;;
  //
  // cout << "nr = " << nr << ", nc= " << nc;
  for (int j = 0; j < nr; j++) {
    for (int c = 0; c < nc; c++) {
      i = j * nc + c;
      a = (char)(65 + i);

      // cout << "i= " << i << ", ans = " << m_Choices[m_Orders[i]]<<endl;
      if (i < noc) {
        if (i == noc - 1 && q.m_NOA) {
          //                    cout << "HAHA: " << noc  << endl;
          if (q.m_TAMode && q.m_SolIndex == q.m_Orders[i]) {
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
          if (q.m_TAMode && q.m_SolIndex == q.m_Orders[i]) {
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
      if (c == nc - 1)
        out << "\\\\" << endl;
      else
        out << "&" << endl;
    }
  }

  // when number of columns is not equal 5, put an extra row in the end of the
  // table.
  // after using the minipage enviorement, the space between the end of table
  // and next question
  // became just way too small. This only happens when nc != 5
  //    if(nc==1)
  //        out << "  \\\\ \n";
  //    else if(nc ==3)
  //        out << "&   &   \\\\" << endl;
  //    else if(nc == 2)
  //        out << "&    \\\\" << endl;

  out << "    \\end{tabular} \n\\end{minipage}\n";
  out << "\\vskip 0.3cm\n";
  return out;
}

//-------------------------------------------------
bool Question::SetQuestion(string q) {
  m_Question = q;
  return true;
}
//-------------------------------------------------
bool Question::AddIII(string iii) {
  m_III[m_NumOfIII] = iii;
  m_NumOfIII++;

  m_TotalIW += iii.length();

  if (iii.length() > m_MaxIW) {
    m_MaxIW = iii.length();
  }

  return true;
}
//-------------------------------------------------
bool Question::AddAChoice(string c, bool sol) {
  if (c == "NOA") {
    m_NOA = true;

    // None of the above is the correct one
    if (sol) {
      m_SolIndex = m_MaxChoices - 1;
    }
    if (m_MaxCW < 16) m_MaxCW = 16;

    m_TotalCW += 16;
    return true;
  }

  m_Choices[m_NumOfChoices] = c;

  //   cout << "Answer: " << c << "  added to mCHoices[" << m_NumOfChoices <<
  //   endl;

  size_t l = c.length();
  m_TotalCW += l;

  if (l > m_MaxCW) {
    m_MaxCW = l;
  }

  if (sol) m_SolIndex = m_NumOfChoices;
  m_NumOfChoices++;

  return true;
}
//-------------------------------------------------
void Question::SetLayout(int i) { m_LayoutStyle = i; }
Question::Question() {
  m_NumOfChoices = 0;
  m_SolIndex = -1;
  m_NumOfIII = 0;

  //    m_QType = 1;
  m_NOA = false;
  m_MaxChoices = 5;
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

  for (int i = 0; i < 10; i++) {
    m_Orders[i] = i;
  }
}
