#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace utils {

std::string ConvertSol(const std::string &sol) {
  std::string solA = "";
  char ai[2];
  ai[1] = '\0';
  char a;
  for (size_t i = 0; i < sol.length(); i++) {
    if (sol[i] == '-') {
      solA += "?";
      i++;
    } else {
      ai[0] = sol[i];
      a = static_cast<char>(atoi(ai) + 65);
      ai[0] = a;
      solA += std::string(ai);
    }
  }
  std::string solB = "";
  for (size_t i = 0; i < solA.length(); i += 5) {
    solB += solA.substr(i, 5) + "\t";
  }
  return solB;
}

std::string Int2String(const int v) {
  std::stringstream out;
  out << v;
  return out.str();
}

bool Roll(const int n, const int m) {
  int a = rand() % m + 1;
  std::cout << "Time(NULL)= " << time(nullptr) << ", Dice rolled: " << a << " from "
       << n << "/" << m << "\n\n";
  return (a <= n);
}

bool IsNumber(const std::string &s) {
  for (unsigned int i = 0; i < s.length(); i++) {
    if (!std::isdigit(s[i])) { return false; }
  }
  return true;
}

bool StringContains(const std::string &s, const std::string &k) {
  return (s.find(k) < s.size());
}

void TrimSpaces(std::string &str) {
  // Trim Both leading and trailing spaces
  size_t startpos = str.find_first_not_of(" ");
  size_t endpos = str.find_last_not_of(" ");

  // if all spaces or empty return an empty string
  if ((std::string::npos == startpos) || (std::string::npos == endpos)) {
    str = "";
  } else {
    str = str.substr(startpos, endpos - startpos + 1);
  }
}

void ShuffleIndex(size_t *out_index, const size_t num) {
  // put the in order first
  for (size_t i = 0; i < num; i++) {
    out_index[i] = i;
  }
  // randomly swap with another one;
  for (size_t i = 0; i < num; i++) {
    size_t j = static_cast<size_t>(rand()) % num;
    std::swap(out_index[i], out_index[j]);
  }
}

bool LoadTemplate(const std::string &fn, std::string &file) {
  std::ifstream ifs;
  ifs.open(fn.c_str());
  if (!ifs.is_open()) {
    return false;
  }

  std::string temp;
  while (ifs.good()) {
    getline(ifs, temp);
    file += temp + "\n";
  }

  file = file.substr(0, file.find("\\end{docu"));
  return true;
}

std::string InsertHeader(const std::string &doc, const std::string &header) {
  // insert header in the line right before \begin{document}
  std::string res;
  res = doc;
  // std::cout << "\n\n\nThe header is \n" << header << endl<< endl<< endl;
  res.insert(res.find("\\begin{documen"), "\n\n" + header + "\n\n");
  return res;
}

void FreeVector(std::vector<int *> &v) {
  for (auto vec : v){
    delete [] vec;
  }
  v.clear();
}

void PrintMan() {
  std::cout << "Usage: examGen -i inputfile [OPTION...]\n\n";
  std::cout << "-i file_name\t\tread from exam repository \"file_name.\"\n";
  std::cout << "-i file_name n\t\tforce to read \"n\" questions from exam "
          "repository \"file_name.\"\n"
       << "\t\t\tOnce this number is provided in one file. You have to provide "
          "them for other input files too. \n";
  std::cout << "-fp file_name\t\tloads foot print file\"file_name\"\n";
  std::cout << "-o file_name\t\toutput filenames in latex format. \n";
  std::cout << "-nf n\t\t\tgenerate \"n\" number of different exams. \n\t\t\tThe "
          "output files will be out_filename_form_A, out_filename_B, "
          "..C,..D.....\n";
  std::cout << "-nq n\t\t\ttake a total of \"n\" numbers of questions from the "
          "input repository(s).\n";
  std::cout << "-tf file_name\t\tuse template file (in tex format) "
          "\"file_name\"\n\t\t\tThe default template file is template.tex.\n";
  std::cout << "-cgs\t\t\tCGS mode. It put different colored bars in the header and "
          "footer area. \n";
  std::cout << "-v\t\t\tturns on verbose mode.\n";
}

int GetOccurence(const std::string &s1, const std::string &sub) {
  int c = 0;
  std::string temp_str = s1;
  while (temp_str.find(sub) < temp_str.length()) {
    c++;
    temp_str.replace(temp_str.find(sub), sub.length(), "");
  }

  return c;
}

bool CheckRep(const std::string &fn, const int lineNo, std::string s1, bool *inTag,
              const std::vector<std::string> &tags) {
  std::string bTag, eTag;
  //    for(unsigned int i=1;i<2;i++)
  for (unsigned int i = 0; i < tags.size(); i++) {
    bTag = "<" + tags[i] + ">";
    eTag = "</" + tags[i] + ">";

    // std::cout << "bTag: " << bTag << "eTag: " << eTag << endl;
    //        std::cout <<"s1: " << s1 << endl;

    int dif = GetOccurence(s1, bTag) - GetOccurence(s1, eTag);
    if (dif < -1 || dif > 1) {
      std::cout << "ERROR:\tFound multiple occurence of tag " << bTag << " or "
           << eTag << "\n"
           << "\tat line: " << lineNo << " in repository \"" << fn << "\"\n"
           << "Did you type " << bTag << " instead of " << eTag
           << " by mistake? \n"
           << "PLEASE FIX THIS PROBLEM IMMEDIATELY!\n";
      return false;
    }
    if (StringContains(s1, bTag) && StringContains(s1, eTag)) {
      // group is before /group
      if (s1.find(bTag) < s1.find(eTag)) {
        s1.replace(s1.find(bTag), bTag.length(), "");
        s1.replace(s1.find(eTag), eTag.length(), "");
      }
    }
    //      std::cout << "s1 again: " << s1 << endl;
    if (StringContains(s1, bTag) && inTag[i]) {
      std::cout << "ERROR:\tFound tag " << bTag << " while expecting tag " << eTag
           << " \n"
           << "\tat line: " << lineNo << " in repository \"" << fn << "\"\n"
           << "PLEASE FIX THIS PROBLEM IMMEDIATELY!\n";
      return false;
    }
    if (StringContains(s1, eTag) && !inTag[i]) {
      std::cout << "ERROR:\tFound tag " << eTag << " while expecting tag " << bTag
           << " \n"
           << "\tat line: " << lineNo << " in repository \"" << fn << "\"\n"
           << "PLEASE FIX THIS PROBLEM IMMEDIATELY!\n";
      return false;
    }
    if (StringContains(s1, bTag) || StringContains(s1, eTag)) {
      inTag[i] = !inTag[i];
      // std::cout << "s: " << s1 << ",   INTag> " << inTag[i] << endl;
    }
  }
  return true;
}

}  // namespace utils
