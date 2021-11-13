#ifndef PROP_MAP_H
#define PROP_MAP_H

#include <string>
#include <unordered_map>
#include <variant>

typedef std::unordered_map<std::string, std::variant<bool, int, float, std::string>> PropMap;

#endif // PROP_MAP_H