#pragma once
#include <map>
#include <string>

std::map<std::string, std::string> loadEnv(const std::string& filename = ".env");
