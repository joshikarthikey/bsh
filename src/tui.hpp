#pragma once
#include "db.hpp"
#include <string>

// Launches the interactive UI.
// Returns the selected command string, or empty string if cancelled.
std::string run_search_ui(HistoryDB& db);