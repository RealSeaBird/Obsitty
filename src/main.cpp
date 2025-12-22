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

const char* home = std::getenv("HOME");





const string config_path = std::string(home) + "/.config/obsitty/obsitty.conf";
const string theme_path = string(home) + "/.config/obsitty/obsitty-theme.conf";


int random_number(){
    int rand_num = rand() % 100 + 1;
    return rand_num;
}

vector<string> settings_themes_options = {
  "Dark",
  "Light"


};


vector<string> menu_entries = {
    "Themes",
    "Folder location"




};


std::string settings_path_placeholder = std::string(home) + "/Documents";
string file_location = "";
int settings_themes_selected = 0;
int settings_selected = 0;


auto menu_settings = Radiobox(&menu_entries, &settings_selected);
auto menu_settings_themes = Radiobox(&settings_themes_options, &settings_themes_selected);
auto menu_settings_folder = Input(&file_location, InputOption{
    .placeholder = &settings_path_placeholder,
});






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

// Load config file on startup
void loadConfig() {
    auto lines = readFileLines(config_path);
    if (!lines.empty()) {
        file_location = lines[0];
    }
}



void saveFile(const std::string& filepath, const std::string& content) {
    // Extract directory path and create it if it doesn't exist
    std::filesystem::path file_path(filepath);
    std::filesystem::path dir_path = file_path.parent_path();

    // Create directories if they don't exist
    try {
        std::filesystem::create_directories(dir_path);
    } catch (const std::filesystem::filesystem_error& ex) {
        // Log to file since we're in TUI context
        std::ofstream log("/tmp/obsitty_debug.log", std::ios::app);
        log << "Error creating directories: " << ex.what() << std::endl;
        return;
    }

    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::ofstream log("/tmp/obsitty_debug.log", std::ios::app);
        log << "Error: unable to open file for writing: " << filepath << std::endl;
        return;
    }

    file << content;
    if (file.fail()) {
        std::ofstream log("/tmp/obsitty_debug.log", std::ios::app);
        log << "Error: failed to write content to file" << std::endl;
        return;
    }

    file.close();
    
    // Log success
    std::ofstream log("/tmp/obsitty_debug.log", std::ios::app);
    log << "File saved successfully: " << filepath << std::endl;
}









std::vector<std::string> getMarkdownFiles() {
    std::vector<std::string> files;
    string docs_path;

    if (file_location.empty()) {
        docs_path = std::string(home) + "/Documents";
    } else {
        docs_path = file_location;
    }

    // Check if directory exists
    if (!std::filesystem::exists(docs_path)) {
        return files;
    }



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













int main(){
    // Load configuration on startup
    loadConfig();

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

    vector<string> entries_3 = {
      "Config:options",
      "Save filepath",
      "Save theme options",




    };


    int entries_selected = 0;
    int entries_selected_2 = 0;
    int entries_selected_3 = 0;


    int selected = 0;
    auto markdown_files = getMarkdownFiles();
    MenuOption option;
    std::string content_1 = "";


option.on_change = [&]() {
    std::string selected_file = markdown_files[selected];
    string docs_path = file_location.empty() ? std::string(home) + "/Documents" : file_location;
    std::string filepath = docs_path + "/" + selected_file;

    auto lines = readFileLines(filepath);
    content_1 = "";
    for (const auto& line : lines) {
        content_1 += line + "\n";
    }
};




    auto menu = Menu(getMarkdownFiles(), &selected, option);

    // Load initial file if any exist
    if (!markdown_files.empty()) {
        std::string selected_file = markdown_files[selected];
        string docs_path = file_location.empty() ? std::string(home) + "/Documents" : file_location;
        std::string filepath = docs_path + "/" + selected_file;

        auto lines = readFileLines(filepath);
        for (const auto& line : lines) {
            content_1 += line + "\n";
        }
    }





    auto textarea_1 = Input(&content_1);
    auto screen = ScreenInteractive::Fullscreen();

    int cols = 5; // Define number of columns for layout



    auto dropdown = Dropdown(&entries, &entries_selected);
    auto dropdown2 = Dropdown(&entries_2, &entries_selected_2);
    auto dropdown3 = Dropdown(&entries_3, &entries_selected_3);


    // Store previous selection to detect changes
    int previous_selection = -1;
    int previous_selection_2 = -1;
    int previous_selection_3 = -1;

    auto container = Container::Vertical(Components{
        Container::Horizontal(Components{ Container::Vertical(Components{ dropdown, dropdown2, dropdown3 }) }),

        Container::Horizontal(Components{ menu, textarea_1 }),
        Container::Horizontal(Components{ menu_settings, menu_settings_themes }),
        Container::Horizontal(Components{ menu_settings_folder })
    });


    auto renderer = Renderer(container, [&] {
        // Handle dropdown selection changes
        if (entries_selected != previous_selection) {
            switch (entries_selected) {
                case 0: // Selection:Menu
                    // No action needed
                    break;
                case 1: { // Save
                    if (!markdown_files.empty()) {
                        std::string selected_file = markdown_files[selected];
                        string docs_path = file_location.empty() ? std::string(home) + "/Documents" : file_location;
                        std::string filepath = docs_path + "/" + selected_file;
                        saveFile(filepath, content_1);
                    }
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

        auto top_bar = (entries_selected_2 == 2)
            ? hbox({
                dropdown->Render(),
                dropdown2->Render(),
                dropdown3->Render(),
                separator(),
                filler(),
                text("Obsitty") | bold
            })
            : hbox({
                dropdown->Render(),
                dropdown2->Render(),
                separator(),
                filler(),
                text("Obsitty") | bold
            });






        auto left = filler() | border | flex;
        auto right = filler() | border | flex;

        if (entries_selected_2 != previous_selection_2)  {
            previous_selection_2 = entries_selected_2;
        }

        if (entries_selected_3 != previous_selection_3)  {
            previous_selection_3 = entries_selected_3;
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

            left = menu_settings->Render() | border | flex;





            switch (entries_selected_3) {

                case 0:
                    // stuff
                    // nothing needed here!
                    break;

                case 1:
                    // stuff
                    saveFile(config_path, file_location);
                    break;

                case 2:
                    //stuff
                    // save the theme options
                    break;




            }





                //Settings will be made next!
                switch (settings_selected) {
                    case 0:
                        right = menu_settings_themes->Render() | border | flex;
                        // stuff
                    break;

                    case 1:
                        right = menu_settings_folder->Render() | border | flex;
                        // more stuff
                    break;



                }




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
