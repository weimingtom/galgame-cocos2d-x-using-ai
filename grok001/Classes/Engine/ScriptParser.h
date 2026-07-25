#pragma once

#include "ScriptCommand.h"
#include <string>

namespace gal {

class ScriptParser {
public:
    /** Load and parse a JSON script from Resources path. */
    static bool parseFile(const std::string& relativePath, ScriptData& out, std::string& error);

    /** Parse JSON text already in memory. */
    static bool parseString(const std::string& jsonText, ScriptData& out, std::string& error);
};

} // namespace gal
