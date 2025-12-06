#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp> 
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>


using namespace std;

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














using namespace ftxui;



int main(){





    std::vector<std::string> entries = {
        "Selection:Menu",
        "Save",
        "Save As",
        "quit"




    };

    int entries_selected = 0;



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

    



    auto dropdown = Dropdown(&entries, &entries_selected);
    
    // Store previous selection to detect changes
    int previous_selection = -1;
    
    auto container = Container::Vertical({
        dropdown,
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
            separator(),
            filler(),
            separator(),
            text("Obsitty") | bold
        });


        auto file_manager = vbox({
            text("tEST"),
            text("Files and shit"),
            filler(),
        }) | border | flex;

        auto left = menu->Render() | border | flex;
        auto right = textarea_1->Render() | border | flex;
        return vbox({
            top_bar,
            hbox({ left, right }) | flex
        });
    });

    screen.Loop(renderer);
    return 0;
}