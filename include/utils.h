#pragma once

#include <string>
#include <vector>

namespace utils {

std::string ConvertSol(const std::string &sol);

std::string Int2String(const int v);

bool Roll(const int n, const int m);

bool IsNumber(const std::string &s);

bool StringContains(const std::string &s, const std::string &k);

void TrimSpaces(std::string &str);

void ShuffleIndex(size_t *out_index, const size_t num);

bool LoadTemplate(const std::string &fn, std::string &file);

std::string InsertHeader(const std::string &doc, const std::string &header);

void FreeVector(std::vector<int*> &v);

void PrintMan();

int GetOccurence(const std::string &s1, const std::string &sub);

bool CheckRep(const std::string &fn, const int lineNo, std::string s1, bool *inTag,
              const std::vector<std::string> &tags);

}  // namespace utils
