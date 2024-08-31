// Pari K. Shah

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int MAX_TRIES = 100; // Maximum attempts to place a word in the grid

// Function to clean a string (lowercase and remove spaces)
string cleanString(const string& str) {
    string cleaned;
    for (char c : str) {
        if (c != ' ') {
            cleaned += tolower(c);
        }
    }
    return cleaned;
}

// Function to randomly place a word in the grid
bool placeWord(char** grid, int rows, int cols, const string& word) {
    int len = word.length();
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {0, -1}, {-1, 0}};
    random_shuffle(directions.begin(), directions.end());

    for (int attempt = 0; attempt < MAX_TRIES; ++attempt) {
        int row = rand() % rows;
        int col = rand() % cols;
        for (auto dir : directions) {
            int newRow = row, newCol = col, k;
            for (k = 0; k < len; ++k) {
                if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols || grid[newRow][newCol] != ' ') {
                    break;
                }
                newRow += dir.first;
                newCol += dir.second;
            }
            if (k == len) { // Place the word if it fits
                for (k = 0; k < len; ++k) {
                    grid[row][col] = word[k];
                    row += dir.first;
                    col += dir.second;
                }
                return true;
            }
        }
    }
    return false; // Failed to place the word
}

// Function to fill the grid with random letters
void fillGridWithRandomLetters(char** grid, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == ' ') {
                grid[i][j] = 'a' + rand() % 26;
            }
        }
    }
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
    srand(time(0));

    ifstream wordsFile("words.txt");

    if (!wordsFile) {
        cout << "Failed to open words.txt" << endl;
        return 1;
    }

    // Read the words to find
    vector<string> words;
    string word;

    while (getline(wordsFile, word)) {
        word = cleanString(word);
        if (!word.empty()) {
            words.push_back(word);
        }
    }

    // Fixed grid size
    const int rows = 16;
    const int cols = 16;

    // Dynamically allocate the grid
    char** grid = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new char[cols];
        fill(grid[i], grid[i] + cols, ' '); // Initialize with spaces
    }

    // Place words in the grid
    for (const string& w : words) {
        if (!placeWord(grid, rows, cols, w)) {
            cout << "Failed to place word: " << w << endl;
        }
    }

    // Fill remaining empty spaces with random letters
    fillGridWithRandomLetters(grid, rows, cols);

    // Search for the words and output results
    for (const string& w : words) {
        int row, col;
        if (searchWord(grid, rows, cols, w, row, col)) {
            cout << row << "," << col << endl;
        } else {
            cout << "-1,-1" << endl;
        }
    }

    // Print the generated grid (optional)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < rows; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}
