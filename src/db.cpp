#include "db.hpp"

HistoryDB::HistoryDB(const std::string& db_path) : db_path_(db_path) {}

void HistoryDB::initSchema() {
    try {
        SQLite::Database db(db_path_, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        
        // Table 1: Unique Command Strings
        db.exec("CREATE TABLE IF NOT EXISTS commands ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "cmd_text TEXT UNIQUE NOT NULL"
                ");");

        // Table 2: The Execution Timeline
        // Includes Git Branch and Success/Fail status
        db.exec("CREATE TABLE IF NOT EXISTS executions ("
                "id INTEGER PRIMARY KEY, "
                "command_id INTEGER, "
                "session_id TEXT, "
                "cwd TEXT, "
                "git_branch TEXT, " // Nullable active git branch
                "exit_code INTEGER, "
                "duration_ms INTEGER, "
                "timestamp INTEGER, "
                "FOREIGN KEY (command_id) REFERENCES commands (id)"
                ");");
                
        std::cout << "BSH Database initialized successfully.\n";
    } catch (std::exception& e) {
        std::cerr << "DB Init Error: " << e.what() << std::endl;
    }
}

// Logic to insert command and link it to execution
void HistoryDB::logCommand(const std::string& cmd, const std::string& session, 
                           const std::string& cwd, const std::string& branch, 
                           int exit_code, int duration) {
    try {
        SQLite::Database db(db_path_, SQLite::OPEN_READWRITE);
        
        // 1. Insert or Ignore the command string to keep it unique
        SQLite::Statement query(db, "INSERT OR IGNORE INTO commands (cmd_text) VALUES (?)");
        query.bind(1, cmd);
        query.exec();

        // 2. Get the Command ID
        SQLite::Statement idQuery(db, "SELECT id FROM commands WHERE cmd_text = ?");
        idQuery.bind(1, cmd);
        idQuery.executeStep();
        int cmd_id = idQuery.getColumn(0);

        // 3. Log the execution context (Git branch, CWD, etc.)
        SQLite::Statement logQuery(db, "INSERT INTO executions "
            "(command_id, session_id, cwd, git_branch, exit_code, duration_ms, timestamp) "
            "VALUES (?, ?, ?, ?, ?, ?, unixepoch())");
            
        logQuery.bind(1, cmd_id);
        logQuery.bind(2, session);
        logQuery.bind(3, cwd);
        logQuery.bind(4, branch);
        logQuery.bind(5, exit_code);
        logQuery.bind(6, duration);
        logQuery.exec();

    } catch (std::exception& e) {
        std::cerr << "Log Error: " << e.what() << std::endl;
    }
}