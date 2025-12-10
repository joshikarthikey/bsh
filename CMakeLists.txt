cmake_minimum_required(VERSION 3.15)
project(bsh VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---

# 1. Internal SQLiteCpp (Avoids macOS system SQLite issues)
set(SQLITECPP_INTERNAL_SQLITE ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.1
)
FetchContent_MakeAvailable(SQLiteCpp)

# 2. LibGit2
find_package(PkgConfig REQUIRED)
# "IMPORTED_TARGET" automatically handles include/lib paths for us
pkg_check_modules(LIBGIT2 REQUIRED IMPORTED_TARGET libgit2) 

# --- Build the Executable ---
add_executable(bsh 
    src/main.cpp 
    src/db.cpp
    #src/git_utils.cpp 
)

# --- Link Libraries ---
target_link_libraries(bsh 
    PRIVATE 
    SQLiteCpp 
    PkgConfig::LIBGIT2 )