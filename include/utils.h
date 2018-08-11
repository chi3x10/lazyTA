#ifndef __INCLUDE_UTILS_H__
#define __INCLUDE_UTILS_H__

#include "question.h"

#include <string>
#include <vector>

namespace utils {

bool LoadTemplate(std::string fn, std::string &file);

std::string InsertHeader(const std::string doc, const std::string header);

void FreeVector(std::vector<int *> &v);

void PrintMan();

int GetOccurence(std::string s1, std::string sub);

bool CheckRep(std::string fn, int lineNo, std::string s1, bool *inTag,
              std::vector<std::string> tags);

}  // namespace utils
#endif  // __INCLUDE_UTILS_H__
