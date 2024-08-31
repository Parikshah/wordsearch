// Pari K. Shah

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Function to convert string to lowercase and remove spaces
string cleanString(const string& str) {
    string cleaned;
    for (char c : str) {
        if (c != ' ') {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

// Function to search for the word in the grid
bool searchWord(char** grid, int rows, int cols, string word, int& row, int& col) {
    int len = word.length();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (tolower(grid[i][j]) == word[0]) {
                // Directions: horizontal, vertical, diagonal
                int directions[8][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, -1}, {-1, 0}};
                for (auto dir : directions) {
                    int k, newRow = i, newCol = j;
                    for (k = 1; k < len; k++) {
                        newRow += dir[0];
                        newCol += dir[1];
                        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols || tolower(grid[newRow][newCol]) != word[k]) {
                            break;
                        }
                    }
                    if (k == len) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main() {
    ifstream gridFile("grid.txt");
    ifstream wordsFile("words.txt");

    if (!gridFile || !wordsFile) {
        cout << "Failed to open grid.txt or words.txt" << endl;
        return 1;
    }

    // Read the grid dimensions
    vector<string> gridLines;
    string line;
    while (getline(gridFile, line)) {
        gridLines.push_back(cleanString(line));
    }
    
    int rows = gridLines.size();
    int cols = gridLines[0].length();
    
    // Dynamically allocate the grid
    char** grid = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new char[cols];
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = gridLines[i][j];
        }
    }

    // Read the words to find
    string word;
    while (getline(wordsFile, word)) {
        word = cleanString(word);
        int row, col;
        if (searchWord(grid, rows, cols, word, row, col)) {
            cout << row << "," << col << endl;
        } else {
            cout << "-1,-1" << endl;
        }
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < rows; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}
