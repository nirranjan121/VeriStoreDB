#include "cli/cli_parser.h"
#include <CLI/CLI.hpp>
#include <iostream>

namespace vsdb {

ParsedCommand CLIParser::parse(int argc, char** argv) {
    CLI::App app{"VSDB - Version-controlled Database System"};
    app.require_subcommand(1);
    
    ParsedCommand result;
    
    // INIT command
    auto* init_cmd = app.add_subcommand("init", "Initialize a new VSDB database");
    
    // CREATE TABLE command
    auto* create_cmd = app.add_subcommand("create", "Create a new table");
    std::string create_table;
    std::vector<std::string> create_cols;
    create_cmd->add_option("table", create_table, "Table name")->required();
    create_cmd->add_option("--columns,-c", create_cols, "Column definitions (name:type)")->required();
    
    // INSERT command
    auto* insert_cmd = app.add_subcommand("insert", "Insert data into a table");
    std::string insert_table;
    std::vector<std::string> insert_cols;
    std::vector<std::string> insert_vals;
    insert_cmd->add_option("table", insert_table, "Table name")->required();
    insert_cmd->add_option("--columns,-c", insert_cols, "Column names");
    insert_cmd->add_option("--values,-v", insert_vals, "Values to insert");

    // DELETE command
    auto* delete_cmd = app.add_subcommand("delete", "Delete rows from a table matching conditions");
    std::string delete_table;
    std::vector<std::string> delete_cols;
    std::vector<std::string> delete_vals;
    delete_cmd->add_option("table", delete_table, "Table name")->required();
    delete_cmd->add_option("--columns,-c", delete_cols, "Column names to match")->required();
    delete_cmd->add_option("--values,-v", delete_vals, "Values to match (same order as --columns)")->required();

    // SELECT command
    auto* select_cmd = app.add_subcommand("select", "Select data from a table");
    std::string select_table;
    select_cmd->add_option("table", select_table, "Table name")->required();
    
    // COMMIT command
    auto* commit_cmd = app.add_subcommand("commit", "Commit current changes");
    std::string commit_msg;
    commit_cmd->add_option("-m,--message", commit_msg, "Commit message")->required();
    
    // LOG command
    auto* log_cmd = app.add_subcommand("log", "Show commit history");
    
    // CHECKOUT command
    auto* checkout_cmd = app.add_subcommand("checkout", "Checkout a specific commit");
    std::string checkout_hash;
    checkout_cmd->add_option("commit", checkout_hash, "Commit hash")->required();
    
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        app.exit(e);
        return result;
    }
    
    // Determine which command was called
    if (app.got_subcommand(init_cmd)) {
        result.cmd = Command::INIT;
    } else if (app.got_subcommand(create_cmd)) {
        result.cmd = Command::CREATE_TABLE;
        result.table_name = create_table;
        result.columns = create_cols;
    } else if (app.got_subcommand(insert_cmd)) {
        result.cmd = Command::INSERT;
        result.table_name = insert_table;
        result.columns = insert_cols;
        result.values = insert_vals;

    } else if (app.got_subcommand(delete_cmd)) {
        result.cmd = Command::DELETE;
        result.table_name = delete_table;
        result.columns = delete_cols;
        result.values = delete_vals;
    }else if (app.got_subcommand(select_cmd)) {
        result.cmd = Command::SELECT;
        result.table_name = select_table;
    } else if (app.got_subcommand(commit_cmd)) {
        result.cmd = Command::COMMIT;
        result.commit_message = commit_msg;
    } else if (app.got_subcommand(log_cmd)) {
        result.cmd = Command::LOG;
    } else if (app.got_subcommand(checkout_cmd)) {
        result.cmd = Command::CHECKOUT;
        result.commit_hash = checkout_hash;
    }
    
    return result;
}

} // namespace vsdb