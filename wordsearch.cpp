// Pari K. Shah

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Function to load the grid from the file
char** loadGrid(const string& filename, int& rows, int& cols) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open grid file: " << filename << endl;
        exit(1);
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        // Remove spaces and convert to lowercase
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        lines.push_back(line);
    }

    rows = lines.size();
    cols = lines.empty() ? 0 : lines[0].size();

    // Dynamically allocate a 2D array for the grid
    char** grid = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new char[cols];
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = lines[i][j];
        }
    }

    file.close();
    return grid;
}

// Function to search for the word in the grid
bool searchWord(char** grid, int rows, int cols, const string& word, int& outRow, int& outCol) {
    int len = word.size();

    // Directions: right, left, down, up, diagonals
    int dir[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == word[0]) {  // Match first letter
                for (auto& d : dir) {
                    int k, rd = r, cd = c;
                    for (k = 1; k < len; ++k) {
                        rd += d[0];
                        cd += d[1];

                        // Out of bounds or mismatch
                        if (rd < 0 || rd >= rows || cd < 0 || cd >= cols || grid[rd][cd] != word[k]) {
                            break;
                        }
                    }
                    if (k == len) { // Found the word
                        outRow = r;
                        outCol = c;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Function to load words from the word list file
vector<string> loadWords(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open word file: " << filename << endl;
        exit(1);
    }

    vector<string> words;
    string word;
    while (getline(file, word)) {
        // Remove spaces and convert to lowercase
        word.erase(remove_if(word.begin(), word.end(), ::isspace), word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }

    file.close();
    return words;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " gridfile.txt wordfile.txt" << endl;
        return 1;
    }

    string gridFile = argv[1];
    string wordFile = argv[2];

    int rows, cols;
    char** grid = loadGrid(gridFile, rows, cols);
    vector<string> words = loadWords(wordFile);

    for (const string& word : words) {
        int row = -1, col = -1;
        if (searchWord(grid, rows, cols, word, row, col)) {
            cout << word << ": " << row << "," << col << endl;
        } else {
            cout << word << ": -1,-1" << endl;
        }
    }

    // Free dynamically allocated memory
    for (int i = 0; i < rows; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}
