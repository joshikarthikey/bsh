#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>
#include <optional>

struct SearchResult {
    int id;
    std::string cmd;
    std::string branch;
    int timestamp;
};

class HistoryDB {
public:
    explicit HistoryDB(const std::string& db_path);
    void initSchema();
    void logCommand(const std::string& cmd, const std::string& session, 
                    const std::string& cwd, const std::string& branch, 
                    int exit_code, int duration);

    std::vector<SearchResult> search(const std::string& query, const std::string& cwd_filter = "");

private:
    std::string db_path_;
};