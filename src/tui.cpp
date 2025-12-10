#include "tui.hpp"
#include <ftxui/component/component.hpp>       // Input, Menu, etc.
#include <ftxui/component/screen_interactive.hpp> // Screen
#include <ftxui/dom/elements.hpp>              // Border, hbox, etc.

using namespace ftxui;

std::string run_search_ui(HistoryDB& db) {
    auto screen = ScreenInteractive::Fullscreen();

    // State variables
    std::string query_text;
    std::string selected_command;
    int selected_index = 0;
    std::vector<SearchResult> results;
    std::vector<std::string> menu_entries; // Strings for the menu to display

    // 1. Input Component
    Component input_option = Input(&query_text, "Type to search...");

    // 2. Menu Component (The list of results)
    auto menu_option = MenuOption();
    menu_option.on_enter = [&] {
        if (selected_index >= 0 && selected_index < results.size()) {
            selected_command = results[selected_index].cmd;
            screen.ExitLoopClosure()(); // Exit the TUI
        }
    };
    Component menu = Menu(&menu_entries, &selected_index, menu_option);

    // 3. Container: Stack Input on top of Menu
    Component container = Container::Vertical({
        input_option,
        menu
    });

    // 4. Custom Renderer to glue logic + visuals
    auto renderer = Renderer(container, [&] {
        // -- LOGIC: Update results based on query --
        // (In a real app, you might debounce this to avoid lag)
        results = db.search(query_text);
        
        // Refresh menu entries
        menu_entries.clear();
        for (const auto& r : results) {
            // Format: "git status  [main]"
            std::string label = r.cmd;
            if (!r.branch.empty()) label += "  [" + r.branch + "]";
            menu_entries.push_back(label);
        }

        // -- RENDER --
        return vbox({
            text("BSH History Search") | bold | color(Color::Blue),
            separator(),
            hbox({
                text(" > "),
                input_option->Render()
            }) | border,
            vbox({
                menu->Render() | vscroll_indicator | frame | flex
            }) | border
        });
    });

    // 5. Run loop
    screen.Loop(renderer);

    return selected_command;
}