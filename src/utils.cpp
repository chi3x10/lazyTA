#include "utils.h"

#include <fstream>
#include <iostream>

namespace utils {


bool LoadTemplate(std::string fn, std::string &file) {
  std::ifstream ifs;
  ifs.open(fn.c_str());
  if (!ifs.is_open()) return false;

  std::string temp;
  while (ifs.good()) {
    getline(ifs, temp);
    file += temp + "\n";
  }

  file = file.substr(0, file.find("\\end{docu"));

  // std::cout << file << "\n\n";
  return true;
}
//
std::string InsertHeader(const std::string doc, const std::string header) {
  // insert header in the line right before \begin{document}
  std::string res;
  res = doc;
  // std::cout << "\n\n\nThe header is \n" << header << endl<< endl<< endl;
  res.insert(res.find("\\begin{documen"), "\n\n" + header + "\n\n");
  return res;
}
//-------------------------------------------------------
void FreeVector(std::vector<int *> &v) {
  for (size_t i = 0; i < v.size(); i++) delete[] v[i];
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

int GetOccurence(std::string s1, std::string sub) {
  int c = 0;
  while (s1.find(sub) < s1.length()) {
    c++;
    s1.replace(s1.find(sub), sub.length(), "");
  }

  return c;
}

bool CheckRep(std::string fn, int lineNo, std::string s1, bool *inTag,
              std::vector<std::string> tags) {
  // static bool inG=false,inQ=false,inC=false,inI=false, inB=false;
  //    static bool inTag[]={false,false,false,false,false};
  //    for(int i=0;i<5;i++) inTag[i] = false;

  int dif;

  std::string bTag, eTag;
  //    for(unsigned int i=1;i<2;i++)
  for (unsigned int i = 0; i < tags.size(); i++) {
    bTag = "<" + tags[i] + ">";
    eTag = "</" + tags[i] + ">";

    // std::cout << "bTag: " << bTag << "eTag: " << eTag << endl;
    //        std::cout <<"s1: " << s1 << endl;

    dif = GetOccurence(s1, bTag) - GetOccurence(s1, eTag);
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
