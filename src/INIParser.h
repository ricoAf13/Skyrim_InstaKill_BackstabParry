#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

namespace INIParser {
    inline std::vector<std::string> GetValues(const std::string& filePath, const std::string& section) {
        std::vector<std::string> results;
        std::ifstream file(filePath);
        if (!file.is_open()) return results;
        
        std::string line;
        bool inSection = false;
        std::string sectionHeader = "[" + section + "]";
        
        while (std::getline(file, line)) {
            // Trim whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            if (line.empty() || line[0] == '#' || line[0] == ';') continue;
            
            if (line[0] == '[') {
                inSection = (line == sectionHeader);
                continue;
            }
            
            if (inSection) {
                auto eqPos = line.find('=');
                if (eqPos != std::string::npos) {
                    std::string val = line.substr(eqPos + 1);
                    val.erase(0, val.find_first_not_of(" \t\r\n"));
                    results.push_back(val);
                }
            }
        }
        return results;
    }
}
