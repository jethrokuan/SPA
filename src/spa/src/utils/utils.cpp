#include "utils/utils.h"
#include <regex>

namespace Utils {

bool has_only_digits(const std::string s) {
  return s.find_first_not_of("0123456789") == std::string::npos;
}

bool is_valid_name(const std::string s) {
  return std::regex_match(s, std::regex("[a-zA-Z](\\d|[a-zA-Z])*"));
}

std::vector<std::string> cartesianProduct(const std::vector<std::string> v1,
                                          const std::vector<std::string> v2,
                                          const char sep) {
  if (v1.empty()) {
    return v2;
  } else if (v2.empty()) {
    return v1;
  }

  std::vector<std::string> result;
  for (const auto& a1 : v1) {
    for (const auto& a2 : v2) {
      result.push_back(a1 + sep + a2);
    }
  }
  return result;
}

//! Finds the cartesian-product of multiple vectors of strings
std::vector<std::string> cartesianProduct(
    const std::vector<std::vector<std::string>> vecs, char sep) {
  std::vector<std::string> intermediate;
  for (const auto& vec : vecs) {
    intermediate = cartesianProduct(intermediate, vec, sep);
  }
  return intermediate;
}

}  // namespace Utils
