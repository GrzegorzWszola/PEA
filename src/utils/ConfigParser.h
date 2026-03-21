#pragma once

#include <string>
#include "Config.h"

class ConfigParser {
    public:
        static Config* parseConfig(const std::string& filePath);
};