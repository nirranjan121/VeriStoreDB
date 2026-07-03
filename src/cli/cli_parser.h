#pragma once

#include <string>
#include <vector>

namespace vsdb {

enum class Command {
    NONE,
    INIT,
    CREATE_TABLE,
    INSERT,
    SELECT,
    DELETE,
    COMMIT,
    LOG,
    CHECKOUT
};

struct ParsedCommand {
    Command cmd = Command::NONE;
    std::string table_name;
    std::vector<std::string> columns;
    std::vector<std::string> values;
    std::string commit_message;
    std::string commit_hash;
};

class CLIParser {
public:
    ParsedCommand parse(int argc, char** argv);
};

} // namespace vsdb