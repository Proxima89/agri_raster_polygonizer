#include "EnvLoader.h"
#include <fstream>
#include <map>
#include <string>

std::map<std::string, std::string> loadEnv(const std::string& filename) {
  std::map<std::string, std::string> env;
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;
    auto pos = line.find('=');
    if (pos == std::string::npos) continue;
    std::string key = line.substr(0, pos);
    std::string value = line.substr(pos + 1);
    key.erase(0, key.find_first_not_of(" \t\r\n"));
    key.erase(key.find_last_not_of(" \t\r\n") + 1);
    value.erase(0, value.find_first_not_of(" \t\r\n"));
    value.erase(value.find_last_not_of(" \t\r\n") + 1);
    env[key] = value;
  }
  return env;
}
