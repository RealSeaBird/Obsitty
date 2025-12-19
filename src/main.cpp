#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <random>
#include <ctime>
#include <stdbool.h>
#include <algorithm>

using namespace std;
using namespace ftxui;

std::vector<std::string> getMarkdownFiles() {
    std::vector<std::string> files;

    // Get home directory
    const char* home = std::getenv("HOME");
    if (!home) return files;

    std::string docs_path = std::string(home) + "/Documents";

    // Iterate through directory
    for (const auto& entry : std::filesystem::directory_iterator(docs_path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            // Check if it ends with .md
            if (filename.size() >= 3 && filename.substr(filename.size() - 3) == ".md") {
                files.push_back(filename);
            }
        }
    }

    return files;
}



int random_number(){
    int rand_num = rand() % 100 + 1;
    return rand_num;
}












// Reads the file and checks if it opens
vector<std::string> readFileLines(const std::string& path) {
    vector<std::string> lines;
    ifstream file(path);





    // if the file fails to open displays error
    if (!file.is_open()){
        cout << "Error unable to find file";
        return lines;
    }
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }



    // Returns lines (duh)
    return lines;
}

// Saves content to a file
void saveFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error: unable to open file for writing: " << filepath << endl;
        return;
    }
    file << content;
    file.close();
    cout << "File saved: " << filepath << endl;
}


















int main(){

    // Initialize random seed once
    srand(time(0));

    int random_position_x;
    int random_position_y;
    int random_position_persistant_x;
    int random_position_persistant_y;
    bool random_active = false;
    
    // Storage for multiple markdown files' positions
    std::vector<std::pair<int, int>> markdown_positions;
    bool positions_initialized = false;











    std::vector<std::string> entries = {
        "Selection:Menu",
        "Save",
        "Save As",
        "quit"




    };


    std::vector<std::string> entries_2 = {

        "Editor view",
        "Graph view",
        "Settings",



    };


    int entries_selected = 0;
    int entries_selected_2 = 0;


    int selected = 0;
    auto markdown_files = getMarkdownFiles();
    MenuOption option;
    std::string content_1 = "";


option.on_change = [&]() {
    std::string selected_file = markdown_files[selected];
    const char* home = std::getenv("HOME");
    std::string filepath = std::string(home) + "/Documents/" + selected_file;

    auto lines = readFileLines(filepath);
    content_1 = "";
    for (const auto& line : lines) {
        content_1 += line + "\n";
    }
};




    auto menu = Menu(getMarkdownFiles(), &selected, option);

    const char* home = std::getenv("HOME");
    std::string selected_file = markdown_files[selected];
    std::string filepath = std::string(home) + "/Documents/" + selected_file;

    auto lines = readFileLines(filepath);


    for (const auto& line : lines) {
        content_1 += line + "\n";
    }





    auto textarea_1 = Input(&content_1);
    auto screen = ScreenInteractive::Fullscreen();

    int cols = 5; // Define number of columns for layout



    auto dropdown = Dropdown(&entries, &entries_selected);
    auto dropdown2 = Dropdown(&entries_2, &entries_selected_2);


    // Store previous selection to detect changes
    int previous_selection = -1;
    int previous_selection_2 = -1;

    auto container = Container::Vertical({
        Container::Horizontal({ Container::Vertical({ dropdown, dropdown2 }) }),

        Container::Horizontal({ menu, textarea_1 })
    });


    auto renderer = Renderer(container, [&] {
        // Handle dropdown selection changes
        if (entries_selected != previous_selection) {
            switch (entries_selected) {
                case 0: // Selection:Menu
                    // No action needed
                    break;
                case 1: { // Save
                    std::string selected_file = markdown_files[selected];
                    const char* home = std::getenv("HOME");
                    std::string filepath = std::string(home) + "/Documents/" + selected_file;
                    saveFile(filepath, content_1);
                    break;
                }
                case 2: // Save As
                    cout << "Save As not yet implemented" << endl;
                    break;
                case 3: // Quit
                    screen.Exit();
                    break;
            }
            previous_selection = entries_selected;
        }






        auto top_bar = hbox({
            dropdown->Render(),
            dropdown2->Render(),
            separator(),
            filler(),
            separator(),
            text("Obsitty") | bold
        });

        auto left = filler() | border | flex;
        auto right = filler() | border | flex;

        if (entries_selected_2 != previous_selection_2)  {
            previous_selection_2 = entries_selected_2;
        }

        switch (entries_selected_2) {
            case 0:
                left = menu->Render() | border | flex;
                right = textarea_1->Render() | border | flex;
                break;

            case 1: {
                left = filler() | border | flex;
                right = canvas([&](Canvas& c) {
                    // Get canvas dimensions
                    int width = c.width();
                    int height = c.height();
                    int center_x = width / 2;
                    int center_y = height / 2;

                    // Calculate center position

                    std::vector<std::string> markdownFiles = getMarkdownFiles();

                    // Initialize positions only once
                    if (!positions_initialized) {
                        markdown_positions.clear();
                        
                        // Grid-based positioning to prevent overlap
                        int cols = 3;
                        int rows = (markdownFiles.size() + cols - 1) / cols;  // Ceiling division
                        int cell_width = width / cols;
                        int cell_height = height / rows;
                        
                        for (size_t i = 0; i < markdownFiles.size(); i++) {
                            int grid_x = i % cols;
                            int grid_y = i / cols;
                            
                            // Add some randomness within each grid cell
                            int x = grid_x * cell_width + (rand() % (cell_width - 20)) + 10;
                            int y = grid_y * cell_height + (rand() % (cell_height - 10)) + 5;
                            
                            // Ensure we stay within canvas bounds
                            x = std::min(x, width - 20);
                            y = std::min(y, height - 5);
                            
                            markdown_positions.push_back({x, y});
                        }
                        positions_initialized = true;
                    }

                    // Draw a dot in the middle
                    c.DrawPointCircleFilled(center_x, center_y, 3);

                    // Draw each markdown file as text at its persistent random position
                    for (size_t i = 0; i < markdownFiles.size() && i < markdown_positions.size(); i++) {
                        // Draw a small dot next to each filename for visual debugging
                        c.DrawPointCircleFilled(markdown_positions[i].first - 3, markdown_positions[i].second, 1);
                        c.DrawText(markdown_positions[i].first, markdown_positions[i].second, markdownFiles[i]);
                    }
                }) | border | flex;
                break;
            }

            case 2:
                //Settings will be made next!
                left = filler() | border | flex;
                right = filler() | border | flex;
                break;
        }




        return vbox({
            top_bar,
            hbox({ left, right }) | flex
        });
    });

    screen.Loop(renderer);
    return 0;
}
