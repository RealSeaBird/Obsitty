#include <iostream>
#include <fstream>
#include <string>
#include <vector>





using namespace std;


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


// Main function (duh)
int main(){

// Gets the filename for the file to open
string filename;



    

cout << "Please Enter the files name: ";
getline(cin, filename);

// Reads all the lines from vector readFile lines and prints them to the standard output
auto lines = readFileLines(filename);

    if (lines.empty()){
        return 1;
    }

    for (auto& line : lines)
        std::cout << line << '\n';

    // Returns 0 (duh)
    return 0;
}