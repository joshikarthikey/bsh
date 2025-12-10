#pragma once
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <iostream>

class HistoryDB {
public:
    explicit HistoryDB(const std::string& db_path);
    void initSchema();
    void logCommand(const std::string& cmd, const std::string& session, 
                    const std::string& cwd, const std::string& branch, 
                    int exit_code, int duration);

private:
    std::string db_path_;
};