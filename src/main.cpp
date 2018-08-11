#include <string.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "group.h"
#include "question.h"
#include "section.h"
#include "utils.h"

using namespace std;
using std::string;
using std::ifstream;
using std::ofstream;
using namespace lazyta;

int main(int argc, char *argv[]) {
  bool inGroup = false;
  bool collecting = false;
  // footprint mode. I don't know why I named it footprint.
  bool fpMode = false;
  // footprint filename
  string fpFilename;
  // strings that represent footprint.
  vector<string> fpString;
  // total number of questions in the repositories
  size_t nQDB = 0;
  // the path to store the image
  string imgPath = "";
  // the text inserted before the group
  string text = "";
  // temp groups
  Group *gp;
  // exam
  vector<Section *> exam;

  string questionLabel = "";
  // number of forms you want to generate. default value = 1;
  size_t nForm = 1;
  // number of questions in this exam.
  size_t nQExam = 0;
  // number of questions that are marked <keepme>
  size_t nKeepme = 0;
  // number of questions in each sections
  vector<size_t> nQS;
  // number of questions that will be used from each sections
  vector<size_t> nQU;
  // file name list
  vector<string> filenames;
  // user defined layout style;
  int userLayout = -1;
  ofstream ofs;
  // output file name (EXAM1_Form_i_time.tex)
  string ofn = "EXAM";
  //
  bool nQUProvided = false;
  // if a question from the repositories is used or not.
  vector<int *> fUsed;
  vector<int *> fKeep;
  // indices used to generate random order
  size_t *index;
  // rand seed
  srand(static_cast<unsigned int>(time(nullptr)));
  // template filename
  string templatefn = "template.tex";
  // content of template
  string tContent = "";
  // temp tContent used in cgsMode
  string tempTC;
  // verbose mode?
  bool verbose = false;
  // cgs mode
  bool cgsMode = false;

  bool noShuffleChoice = false;
  // begin enumerate
  string enuHeader =
      string("\n\\begin{enumerate}[1.\\underline{\\makebox[0.5in]{    }}]\n") +
      "\\setlength{\\itemsep}{0pt}\n" + "\\setlength{\\parskip}{0pt}\n" +
      "\\setlength{\\parsep}{0pt}\n";

  // special marker used only when cgsMode is ON
  string marker[] = {
      "\\lhead{\\color{mine}{\\Huge CGS2531 CGS2531 CGS2531 CGS2531 CGS2531 }}",
      "\\lhead{\\colorbox{mine}{\\color{mine}{\\Huge CGS2531 CGS2531 CGS2531 "
      "CGS2531 CGS2531 CGS2531 CGS2531}}}",
      "\\lfoot{\\colorbox{mine}{\\color{mine}{\\Huge CGS2531 CGS2531 CGS2531 "
      "CGS2531 CGS2531 CGS2531 CGS2531}}}",
      "\\lhead{\\Huge{\\colorbox{mine}{\\color{mine}{ CGS2531}} "
      "\\color{mine1}{CGS2531} \\colorbox{mine}{\\color{mine}{ CGS2531}} "
      "\\color{mine1}{CGS2531} \\colorbox{mine}{\\color{mine}{ CGS2}}} }",
      "\\lfoot{\\Huge{\\colorbox{mine}{\\color{mine}{ CGS2531}} "
      "\\color{mine1}{CGS2531} \\colorbox{mine}{\\color{mine}{ CGS2531}} "
      "\\color{mine1}{CGS2531} \\colorbox{mine}{\\color{mine}{ CGS2}}}}"};

  // read file line by line to this string
  string s1;

  if (argc < 3) {
    utils::PrintMan();
    return 0;
  }

  int count = argc;
  for (int i = 1; i < count; i++) {
    // cout << argv[i]<<endl;

    if (string(argv[i]) == "-i") {  // read the file name so skip one argc
      if (fpMode)
        cout
            << "Warning: Footprint mode is on. Input files will be omitted. \n";
      i++;
      filenames.push_back(string(argv[i]));
      if (i >= argc - 1) break;
      if (!utils::IsNumber(string(argv[i + 1])) && nQUProvided) {
        cout << "ERROR: You need to provide the number of questions you will "
                "use from this file."
             << endl;
        cout << endl;
        utils::PrintMan();
        return 1;
      }
      // check if nQU number is provided
      if (utils::IsNumber(string(argv[i + 1]))) {
        i++;
        nQU.push_back(atoi(argv[i]));
        nQUProvided = true;
      }
    } else if (string(argv[i]) == "-o") {
      ofn = string(argv[i + 1]);
      i++;
      cout << "Output filename is set to be: " << ofn << endl;

    } else if (string(argv[i]) == "-nf") {
      nForm = static_cast<size_t>(atoi(argv[i + 1]));
      i++;
    } else if (string(argv[i]) == "-nq") {
      nQExam = static_cast<size_t>(atoi(argv[i + 1]));
      i++;
    } else if (string(argv[i]) == "-fp") {
      fpMode = true;
      fpFilename = string(argv[i + 1]);
      if (filenames.size() > 0)
        cout << "Warning: FOOTPRINT MODE. Input files specified by the -t "
                "option will be omitted. "
             << endl;
      i++;
    } else if (string(argv[i]) == "-tf") {
      templatefn = string(argv[i + 1]);
      cout << "Cool! You have your own template file. \n";
      i++;
    } else if (string(argv[i]) == "-cgs")
      cgsMode = true;
    else if (string(argv[i]) == "-v")
      verbose = true;
    else {
      cout << "ERROR!\t\"" << argv[i] << "\" is not a valid option! \n \tWhat "
                                         "did you smoke this morning? \n\n";
      utils::PrintMan();
      return 3;
    }
  }
  if (filenames.size() == 0 && !fpMode) {
    cout << "ERROR: Input repository files not specified! Program terminated! "
         << "\n Remember the -i option???\n\n";

    utils::PrintMan();
    return 1;
  }
  if (!utils::LoadTemplate(templatefn, tContent)) {
    cout << "ERROR: template file\"" << templatefn
         << "\" not found! \nBYE BYE!\n\n";
    utils::PrintMan();
    return 1;
  }
  //

  ifstream ifs;
  // footprint mode
  // Load footprint file
  if (fpMode) {
    int ii = 0;

    filenames.clear();
    ifs.open(fpFilename.c_str());
    if (!ifs.is_open()) {
      cout << "ERROR: footprint file \"" << fpFilename << "\" no found!" << endl
           << endl;
      utils::PrintMan();
      return 1;
    }

    while (ifs.good()) {
      getline(ifs, s1);
      if (ii % 2 == 0) {
        if (s1.length() > 2) filenames.push_back(s1);
      } else
        fpString.push_back(s1);
      ii++;
    }
    ifs.close();
  }

  // nuber of questions in each sections;
  size_t nq = 0;
  // find out the total number of questions in the repositories.
  // cout << "what??? \n\n";

  // bool inG=false, inQ=false, inC=false, inII=false;

  int lineNo = 0;
  int *tempi;
  //    if(verbose)
  cout << "!!Checking syntax errors in repository...\n";

  for (size_t i = 0; i < filenames.size(); i++) {
    vector<string> tags;
    tags.push_back(string("group"));
    tags.push_back(string("q"));
    tags.push_back(string("c"));
    tags.push_back(string("ii"));
    tags.push_back(string("b"));

    ifs.open(filenames[i].c_str());
    if (!ifs.is_open()) {
      cout << "ERROR: input file \"" << filenames[i] << "\" not found. "
           << endl;
      return 1;
    }
    // reset line number
    lineNo = 0;
    //        cout << filenames[i] << "-->" << ifs.is_open() << endl;

    bool inTag[] = {false, false, false, false, false};
    // scan through the whole database to see how many questions are there
    while (ifs.good()) {
      getline(ifs, s1);
      lineNo++;
      if (s1.find("<q>") < s1.length()) {
        nq++;
      }
      // check repository
      if (!utils::CheckRep(filenames[i], lineNo, s1, inTag, tags)) {
        return 3;
      }
    }

    // final check
    for (size_t jj = 0; jj < 5; jj++) {
      if (inTag[jj]) {
        cout << "ERROR:\tMissing tag \"</" << tags[jj]
             << ">\" in the end of repository: \"" << filenames[i]
             << "\".\n\tDid you forget to put a \"</" << tags[jj]
             << ">\" after/in the last question??" << endl
             << "PLEASE FIX THIS PROBLEM1\n\n";
        return 3;
      }
    }
    tempi = new int[nq];
    memset(tempi, 0, sizeof(int) * nq);
    fUsed.push_back(tempi);
    tempi = new int[nq];
    memset(tempi, 0, sizeof(int) * nq);
    fKeep.push_back(tempi);
    // cout << "nQS = " << nq << endl;
    nQS.push_back(nq);
    nQDB += nq;
    nq = 0;
    // close and reopen it
    ifs.close();

    tags.clear();
  }

  if (nQExam == 0) {
    if (nQU.size() == 0) {
      cout << "Number of questions is not specified. It's set to be" << nQDB
           << ".\n";
      nQExam = nQDB;
    } else {
      for (unsigned int i = 0; i < nQU.size(); i++) {
        nQExam += nQU[i];
        //                cout << "nQU[" << i << " = " << nQU[i] << endl;
      }
    }
  } else if (nQU.size() > 0) {
    size_t tq = 0;
    for (unsigned int i = 0; i < nQU.size(); i++) {
      tq += nQU[i];
    }

    if (nQExam != tq) {
      cout << "Warning: \tNumber of questions provided by the -nq option is "
              "different from"
           << "\n\t    the sum of the numbers of questions specified by the -i "
              "option \n"
           << "\t    (-nq)  " << nQExam << " =\\= ";
      for (unsigned int i = 0; i < nQU.size(); i++) {
        cout << nQU[i];
        if (i != nQU.size() - 1) cout << " + ";
      }
      cout << "  (-i)\n\t    Number of questions of this exam is changed to "
           << tq << endl
           << endl;
    }
    nQExam = tq;
  }

  //--------------------------
  if (nQDB < nQExam) {
    cout << "Warning: Number of questions in the repositories is " << nQDB
         << ". \n";
    cout << "Only " << nQDB << "questions will be generated in this exam. \n\n";
    nQExam = nQDB;
  }
  // cout << "nQDB= " << nQDB << endl;
  // index for shuffling later
  index = new size_t[nQDB];
  for (size_t i = 0; i < nQDB; i++) {
    index[i] = i;
  }

  // number of questions that will used to generate the exam from each section
  // is not specified
  size_t nu;
  size_t totalnQU = 0;
  if (nQU.size() == 0)  // questions from each section is not specified by user
  {
    for (unsigned int i = 0; i < nQS.size(); i++) {
      nu = static_cast<size_t>(nQS[i] * nQExam / static_cast<float>(nQDB));
      // it a section has only one question keep it!
      if (nQS[i] == 1) nu = 1;
      // cout << nu << endl;
      nQU.push_back(nu);
      totalnQU += nu;
    }
    nu = 0;
  } else {
    totalnQU = nQExam;
  }

  // not enough question in nQU
  if (totalnQU < nQExam) {
    if (verbose) cout << "Not enough question in nQU[]. Adding more...\n\n";

    size_t diff = nQExam - totalnQU;
    for (unsigned int i = 0; i < nQU.size(); i++) {
      size_t d1 = nQS[i] - nQU[i];
      // cout << "i=" << i << "(nQS,nQU) = " << nQS[i] <<", " << nQU[i]<< ", d1=
      // " << d1 << ", diff=" << diff<<endl;
      // give it all to nQS[i]
      if (d1 > diff) {
        nQU[i] = nQU[i] + diff;
        break;

      } else {
        nQU[i] += d1;
        diff -= d1;
      }
    }
  }

  if (verbose) {
    cout << "Going through the repositories and look for the <keepme> "
            "tags....\n";
  }

  nKeepme = 0;
  s1 = "";

  // flag the <keepme> questions
  int *pKeep, *pUsed;
  for (unsigned int i = 0; i < filenames.size(); i++) {
    size_t nkm = 0;
    nq = 0;
    ifs.open(filenames[i].c_str());

    pKeep = fKeep[i];

    // scan through the whole database to see how many questions are there
    while (ifs.good()) {
      getline(ifs, s1);
      if (utils::StringContains(s1, "<q>")) {
        if (utils::StringContains(s1, "<keepme>")) {
          pKeep[nq] = 1;
          nKeepme++;
          nkm++;
          if (verbose) cout << "\tFound one <keepme> tag\n";
        }
        nq++;
      }
    }
    ifs.close();
    if (nQU.size() > 0) {
      if (nkm > nQU[i]) {
        cout << "\nWarning: The # of <keepme> questions in repository - \""
             << filenames[i] << "\"\nis greater than the # of questions that "
                                "will be extracted from this repository. "
             << "\nAll the <keepme> tags in this repository are ignored.\n\n";
        memset(pKeep, 0, nQS[i] * sizeof(int));
      }  // if
    }    // if
  }
  if (verbose) {
    cout << endl;
  }

  // cout << "nQS = " << nQS[0] << ", " << nQS[1] << endl;
  // cout << "nQU = " << nQU[0] << ", " << nQU[1] << endl;

  for (unsigned int i = 0; i < nQS.size(); i++) {
    cout << "(" << nQU[i] << "/" << nQS[i]
         << ") questions are selected from repository - " << filenames[i]
         << endl;
  }
  cout << endl;

  if (nKeepme > nQExam) {
    cout << "Warning: Number of questions that are marked <keepme> is greater "
            "than the number of question of this exam. \n All <keepme> labels "
            "are disabled.!! \n\n";
    for (unsigned int i = 0; i < fKeep.size(); i++)
      memset(fKeep[i], 0, sizeof(int) * nQS[i]);
  }

  if (fpMode) {
    // Free fUsed and fKeep;
    utils::FreeVector(fUsed);
    utils::FreeVector(fKeep);

    cout << "Handling footprintn data!! \n\n";
    for (unsigned int v = 0; v < fpString.size(); v++) {
      // cout << "HAHA\n";
      char a[2];
      a[1] = '\0';
      int *fu = new int[fpString[v].length()];
      cout << "The loaded footprint code:" << endl;
      for (unsigned int i = 0; i < fpString[v].length(); i++) {
        a[0] = fpString[v][i];
        fu[i] = atoi(a);
        cout << fu[i];
      }
      cout << endl;
      fUsed.push_back(fu);
    }
  } else {
    // randomly decide which question to use.
    cout << "Generated footprint code: \n\ti.e. randomly decide which "
            "questions to pick.\n";
    for (size_t v = 0; v < fUsed.size(); v++) {
      pUsed = fUsed[v];
      pKeep = fKeep[v];

      // randomly select the questions.
      utils::ShuffleIndex(index, nQS[v]);

      for (size_t i = 0; i < nQU[v]; i++) {
        pUsed[index[i]] = 1;
      }

      for (size_t i = nQU[v]; i < nQS[v]; i++) {
        if (pKeep[index[i]] == 1) {
          pUsed[index[i]] = 1;
          // go back to disable it.
          for (size_t j = 0; j < nQU[v]; j++) {
            if (pUsed[index[j]] == 1 && pKeep[index[j]] == 0) {
              pUsed[index[j]] = 0;
              break;
            }
          }
        }
      }

      for (size_t i = 0; i < nQS[v]; i++) {
        cout << pUsed[i] << " ";
      }
      cout << endl;
    }
  }
  cout << endl;

  //# of questions added already;
  int qAdded = 1;

  // iterate through sections
  for (size_t i = 0, secNum = 0; i < filenames.size(); i++, secNum++) {
    nq = 0;
    if (verbose)
      cout << "================================================================"
              "====\n"
           << "|   Loading questions from repository: " << filenames[i]
           << "\n--------------------------------------------------------------"
              "------\n";
    // create a section
    Section *temp = new Section();
    exam.push_back(temp);

    //
    pUsed = fUsed[i];
    ifs.open(filenames[i].c_str());

    // string that stores the whole question
    string wq;

    //
    int l = 0;
    // reading the file.
    while (ifs.good()) {
      getline(ifs, s1);
      // trim leading and tailing spaces.
      utils::TrimSpaces(s1);

      // increment line number
      l++;

      // the comments
      if (utils::StringContains(s1, "<!--")) {
        if (!utils::StringContains(s1, "-->")) {
          cout << "Warning: At line " << l << ": comments must be enclosed by "
                                              "\"<!--\" and \"-->\" and they "
                                              "must be in the same line. \n";
          cout << "This problem occurs in the repository: \"" << filenames[i]
               << "\".\n";
          cout << "Everything after \"<!--\" in Line:" << l
               << " is ignored due to lack of the comment ending symbol "
                  "\"-->\".\n\n ";
          // s1 = "";
          s1.replace(s1.find("<!--"), s1.length() - s1.find("<!--"), "");
          // cout << "s1: " << s1 << endl;
        } else {
          size_t b1 = s1.find("<!--"), e1 = s1.find("-->");
          // cout << "before:   " << s1 << endl;
          if (verbose)
            cout << "Comment: \"" << s1.substr(b1, e1 + 3)
                 << "\" is removed! \n";
          s1.replace(b1, e1 - b1 + 3, "");
          // cout << "after:   " << s1 << endl;
          utils::TrimSpaces(s1);
        }
      }

      if (collecting && utils::StringContains(s1, "<noshuffle>")) {
        noShuffleChoice = true;
        // remove <noshuffle> from s1
        s1.replace(s1.find("<noshuffle>"), 11, "");
        if (verbose)
          cout << "<noshuffle> found at line: " << l
               << ". No shuffle of options in this question.\n\n";
      }
      // put the whole question in one string
      if (collecting) {
        // if </q is there>
        if (utils::StringContains(s1, "</q>")) {
          // collecting must be true there
          // must put whatever is infront of this token in wq
          wq += s1.substr(0, s1.find("</q>") + 4);  // keep the </q> in the end
          // this will be the terminate condition when add the question in
          //
          collecting = false;

          // cout <<"\n\n ONE q is here" << endl;
          // cout << wq << endl << endl << endl;
          //  a group is created when <q> or <group> is encountered.
          gp = exam[secNum]->GetLastGroup();

          // cout << "Address: " << gp << endl;
          // add the question
          Question *q = new Question();

          // replace $ with \$
          // replace _ with \_
          // user has specifized the layout style{
          if (userLayout > 0) {
            q->SetLayout(userLayout);
            userLayout = -1;
          }
          if (questionLabel.length() > 0) {
            q->SetLabel(questionLabel);
            questionLabel = "";
          }

          q->AddWholeQuestion(wq);
          if (noShuffleChoice) {
            q->SetAbleShuffle(false);
            noShuffleChoice = false;
          }

          if (pUsed[nq] == 1) {
            q->SetQN(qAdded);
            qAdded++;
            gp->PushBack(q);
            if (verbose) {
              string ddd = q->GetQuestionBody();
              size_t n = (ddd.length() < 30) ? ddd.length() : 30;
              cout << "Question: \n\t" << ddd.substr(0, n)
                   << ".... is added. \n";
            }
          } else {
            if (verbose) {
              string ddd = q->GetQuestionBody();
              size_t n = (ddd.length() < 30) ? ddd.length() : 30;
              cout << "Haha! XD   Question: \n\t" << ddd.substr(0, n)
                   << ".... is not selected. \n";
            }
            delete q;

            // this means this deleted question is in a single question group.
            if (!inGroup) {
              exam[secNum]->PopBack();
              delete gp;
            }
          }
          nq++;
        } else {
          wq += s1;
        }
      }

      if (utils::StringContains(s1, "<group>")) {
        if (inGroup) {
          cout << "ERROR: This lame program does not support nested group "
                  "structure. \n\n BYE BYE!! \n\n";
          return 1;
        }

        inGroup = true;
        // create a new group
        gp = new Group();
        exam[secNum]->AddGroup(gp);
        // no suffle are in the same line as
        if (utils::StringContains(s1, "<noshuffle>")) {
          gp->SetShuffle(false);
          if (verbose)
            cout << "Questions in this group will not be shuffled. \n";
        }
      }
      //
      if (utils::StringContains(s1, "<noshuffle>") && inGroup) {
        gp = exam[secNum]->GetLastGroup();
        // you can only place <no suffle> right after <group>
        if (gp->GetNoQ() == 0) {
          gp->SetShuffle(false);
          if (verbose)
            cout << "Questions in this group will not be shuffled. \n";
        }
      }

      if (utils::StringContains(s1, "</group>")) {
        inGroup = false;
        gp = exam[secNum]->GetLastGroup();
        // no questions added in this group
        if (gp->GetNoQ() == 0) {
          cout << "    Notice: No questions from this group are selected\n    "
                  "Please refer to the footprint code for details.\n\n";
          delete gp;
          exam[secNum]->PopBack();
        }
      }

      // found the begining of a question
      if (utils::StringContains(s1, "<q>")) {
        // found <noshuffle> tag
        if (utils::StringContains(s1, "<noshuffle>")) {
          noShuffleChoice = true;
          if (verbose)
            cout << "<noshuffle> tag is found at line: " << l
                 << ". Options of this question will not be shuffled. \n";
        } else {
          noShuffleChoice = false;
        }

        wq = "";
        if (!utils::StringContains(s1, "</q>")) {
          wq += s1.substr(s1.find("<q>") + 3, s1.size());
          //    cout << "HAHA: "<< wq << endl;
          collecting = true;
          if (!inGroup) {
            gp = new Group();
            exam[secNum]->AddGroup(gp);
          }
        } else { // <q> and </q> are in the same line
          if (verbose) {
            cout << "At line #" << l << ". Putting a question in a single "
                                        "line? Don't you think it's too "
                                        "messy?? \n";
          }

          wq += s1.substr(s1.find("<q>") + 3,
                          s1.find("</q>") + 4);  // keep the </q> in the end
          // this will be the terminate condition when add the question in
          //
          collecting = false;
          // It's done collecting now. Add this question in
          // this question is a group question. add it in to the last group
          if (inGroup) {
            gp = exam[secNum]->GetLastGroup();
          // if it's not a group question, add
          } else {
            gp = new Group();
            exam[secNum]->AddGroup(gp);
          }

          if (pUsed[nq] == 1) {
            Question *q = new Question();

            //--------------------------------------------------------
            if (utils::StringContains(s1, "</layout=")) {
              string ttt = s1.substr(s1.find("</layout=") + 9, 1);
              q->SetLayout(atoi(ttt.c_str()));
              // cout << ttt << "\n\n";
              if (verbose)
                cout << "User defined style " << ttt << " found. " << endl;
            }
            //-------dfd--------------------------------------------
            q->AddWholeQuestion(wq);
            if (utils::StringContains(s1, "<noshuffle>")) {
              q->SetAbleShuffle(false);
              cout << "<noshuffle> found at line: " << l
                   << ". No shuffle of options in this question.\n\n";
            }

            if (verbose) {
              string ddd = q->GetQuestionBody();
              size_t n = (ddd.length() < 20) ? ddd.length() : 20;
              cout << "Question: \n\t" << ddd.substr(0, n)
                   << ".... is added. \n";
            }
            q->SetQN(qAdded);
            qAdded++;
            gp->PushBack(q);
          } else if (!inGroup)  // this means this deleted question is in a
                                // single question group.
          {
            // cout << "There you are?\n\n";
            exam[secNum]->PopBack();
            if (verbose) {
              cout << "This question is not selected....\n";
            }

            delete gp;
          }
          nq++;
        }
      }  // <q>

      if (utils::StringContains(s1, "</label")) {
        if (!collecting) {
          cout << "ERROR: Orphan </label=question label> command found in the "
                  "line:"
               << l << "of repository file: " << filenames[i]
               << ". Please fix this problem immediately!\n";
          cout << "FYI: </label=question label....> must be placed in between "
                  "the <q> and the <b> tags. \n";
          return 1;
        }

        string ttt = s1.substr(s1.find("</label") + 7,
                               s1.length() - s1.find("</label") - 7);
        utils::TrimSpaces(ttt);
        // get rid of = and >
        ttt = ttt.substr(1, ttt.length() - 2);
        questionLabel = ttt;

        // userLayout = atoi(ttt.c_str());
        // cout << ttt << "\n\n";
        if (verbose)
          cout << "\tQuestion label: \"" << ttt << "\" found. " << endl;
      }
      if (utils::StringContains(s1, "</layout=")) {
        if (!collecting) {
          cout << "ERROR: Orphan </layout=n> command found in the line #" << l
               << "of repository file: " << filenames[i]
               << ". Please fix this problem immediately. \n";
          cout << "FYI: </layout=n> must be placed in between the <q> and the "
                  "<b> tags. \n";
          cout << "     No space keys are allowed inside the </layout=n> "
                  "command. Ex: </layout = 3> is not valid!\n\n";
          return 1;
        }
        string ttt = s1.substr(s1.find("</layout=") + 9, 1);
        userLayout = atoi(ttt.c_str());

        // cout << ttt << "\n\n";
        if (verbose) cout << "User defined style " << ttt << " found. " << endl;
      }
      if (utils::StringContains(s1, "<img>")) {
        // remove this segment of string
        string temp1 = s1.substr(s1.find("<img>"),
                                 s1.find("</img>") - s1.find("<img>") + 6);
        s1.replace(s1.find("<img>"), temp1.length(), "");
        // cout << "<img> removed!!! " << s1 << endl << endl;
        gp = exam[secNum]->GetLastGroup();

        // cout << "temp1 = " << temp1 << endl;
        // remove the <img> and </img> labels
        temp1.replace(temp1.find("<img>"), 5, "");
        temp1.replace(temp1.find("</img>"), 6, "");
        if (verbose) cout << "<img> marker found. You insert an image here. \n";
        gp->SetFigure(temp1, 1);
      }
      if (utils::StringContains(s1, "<txt>")) {
        // remove this segment of string
        string temp1 = s1.substr(s1.find("<txt>"),
                                 s1.find("</txt>") - s1.find("<txt>") + 6);
        s1.replace(s1.find("<txt>"), temp1.length(), "");

        // cout << "<txt> removed!!! " << s1 << endl << endl;
        gp = exam[secNum]->GetLastGroup();
        // cout << "Address: " << gp << endl;

        // cout << "temp1 = " << temp1 << endl;
        // remove the <img> and </img> labels
        temp1.replace(temp1.find("<txt>"), 5, "");
        temp1.replace(temp1.find("</txt>"), 6, "");

        if (verbose) cout << "<txt> marker found. You insert texts here. \n";

        gp->SetText(temp1, 1);
        // cout << "TEXT set! " << endl;
      }

      // tt = tt+ s1;
    }  // end of while
    ifs.close();
    if (verbose)
      cout << "-------------------------------------------------------------\n"
           << "|    Finish loading from repository: " << filenames[i] << endl
           << "=============================================================\n";
  }  // end of for filename

  // write down footprint
  string fofn = ofn + "_footprint.txt";
  ofs.open(fofn.c_str());

  for (unsigned int i = 0; i < filenames.size(); i++) {
    ofs << filenames[i] << endl;
    pUsed = fUsed[i];
    for (size_t j = 0; j < nQS[i]; j++) {
      ofs << pUsed[j];
    }
    ofs << endl;
  }
  ofs.close();

  char fc[2];
  fc[1] = '\0';
  //
  cout << "Generating ..._FORM_0 , ..._TA_FORM_0 and the answer key for "
          "..._FORM_0.\n";
  // save the form without shuffling
  fofn = ofn + "_Form_0.tex";
  ofs.open(fofn.c_str());

  if (cgsMode) {
    tempTC = tContent;
    tContent = utils::InsertHeader(tempTC, marker[0]);
  }
  ofs << tContent << enuHeader << endl;
  if (cgsMode) tContent = tempTC;

  for (unsigned int i = 0; i < exam.size(); i++) ofs << *(exam[i]);
  ofs << "\\end{enumerate}\n\\end{document}\n\n";
  ofs.close();

  // save the above form with TAMode on
  for (unsigned int i = 0; i < exam.size(); i++)  // turn TA Mode ON
    exam[i]->SetTAMode(true);
  fofn = ofn + "_Form_0_TA.tex";  // filename
  ofs.open(fofn.c_str());

  if (cgsMode) {
    tempTC = tContent;
    tContent = utils::InsertHeader(tempTC, marker[0]);
  }
  ofs << tContent << enuHeader << endl;  // throw template in
  if (cgsMode) tContent = tempTC;

  for (unsigned int i = 0; i < exam.size(); i++) ofs << *exam[i];
  ofs << "\\end{enumerate}\n\\end{document}\n\n";
  ofs.close();
  for (unsigned int i = 0; i < exam.size(); i++)  // turn TA Mode off
    exam[i]->SetTAMode(false);

  // Generate solution file for form 0
  fofn = ofn + "_Form_0_SOLUTION.txt";
  ofs.open(fofn.c_str());
  // cout << "solution file opened!! " << endl;
  string sol = "";
  string solTemp = "";
  // get the solution
  for (unsigned int i = 0; i < exam.size(); i++) {
    solTemp = exam[i]->GetSolution();
    sol += solTemp;
    solTemp = utils::ConvertSol(solTemp);
    //        cout << "solTemp: " << solTemp << endl;
    if (utils::StringContains(solTemp, "?"))
      cout << "\tThis question is in repository: \"" << filenames[i]
           << "\".\n\n\n";
  }
  // convert sol from 0-4 to A-E

  //<< sol << endl;
  ofs << "Form 0" << endl;
  ofs << utils::ConvertSol(sol) << endl << endl;
  ofs.close();

  // generating forms
  for (size_t f = 0; f < nForm; f++) {
    cout << "Generating FORM " << char(65 + f) << ", TA FORM " << char(65 + f)
         << " and the answer key.\n";
    for (unsigned int i = 0; i < exam.size(); i++) exam[i]->Shuffle();

    fc[0] = 65 + static_cast<char>(f);
    fofn = ofn + "_Form_" + string(fc) + ".tex";
    ofs.open(fofn.c_str());
    if (!ofs.is_open()) {
      cout << "ERROR: Output text file \"" << fofn << "\" cannot be opened! \n"
           << "No files are generated! \n\n";
      return 1;
    }

    if (cgsMode) {  // CGS Mode
      tempTC = tContent;
      tContent = utils::InsertHeader(tempTC, marker[f % 5]);
    }
    ofs << tContent << enuHeader << endl;  // throw template in
    if (cgsMode) tContent = tempTC;

    for (unsigned int i = 0; i < exam.size(); i++) ofs << *exam[i];
    ofs << "\\end{enumerate}\n\\end{document}\n";
    ofs.close();

    // generate same form with TAMode on
    for (unsigned int i = 0; i < exam.size(); i++) exam[i]->SetTAMode(true);

    fofn = ofn + "_Form_" + string(fc) + "_TA.tex";
    ofs.open(fofn.c_str());
    if (cgsMode) {  // CGS Mode
      tempTC = tContent;
      tContent = "";
      tContent = utils::InsertHeader(tempTC, marker[f % 5]);
    }
    // cout << tContent << "\nTA\n"<<endl;
    ofs << tContent << enuHeader << endl;  // throw template in
    if (cgsMode) {
      tContent = tempTC;
    }
    // throw exam in
    for (unsigned int i = 0; i < exam.size(); i++) ofs << *exam[i];
    ofs << "\\end{enumerate}\n\\end{document}\n";
    ofs.close();

    for (unsigned int i = 0; i < exam.size(); i++) exam[i]->SetTAMode(false);

    // Generate solution files.
    fofn = ofn + "_SOLUTION.txt";
    if (f == 0)
      ofs.open(fofn.c_str());
    else
      ofs.open(fofn.c_str(), ios::out | ios::app);
    // cout << "solution file opened!! " << endl;
    string sol = "";
    // get the solution
    for (unsigned int i = 0; i < exam.size(); i++) {
      solTemp = exam[i]->GetSolution();
      sol += solTemp;
      solTemp = utils::ConvertSol(solTemp);
      if (utils::StringContains(solTemp, "?"))
        cout << "\tThis question is in repository: " << filenames[i]
             << ".\n\n\n";
    }
    //
    ofs << "Form " << char(65 + f) << endl << utils::ConvertSol(sol) << endl << endl;
    ofs.close();
  }

  // clear
  for (auto section : exam){
    delete section;
  }
  exam.clear();
  utils::FreeVector(fUsed);
  utils::FreeVector(fKeep);

  delete [] index;

  cout << "Program done!\n\n";
  return 0;
}
