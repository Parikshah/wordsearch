// Pari K. Shah

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

// Function prototypes
bool searchInDirection(char** grid, int rows, int cols, const char* word, int startRow, int startCol, int dirRow, int dirCol);
void searchWord(char** grid, int rows, int cols, const char* word, int& foundRow, int& foundCol);

int main() {
    ifstream gridFile("grid.txt");
    ifstream wordsFile("words.txt");

    if (!gridFile || !wordsFile) {
        cout << "Error opening file." << endl;
        return 1;
    }

    // Reading the grid
    int rows = 0, cols = 0;
    gridFile >> noskipws;

    // Determine the grid's dimensions by counting rows and columns
    string line;
    while (getline(gridFile, line)) {
        if (cols == 0) cols = line.length();
        rows++;
    }

    // Rewind the grid file to the beginning
    gridFile.clear();
    gridFile.seekg(0, ios::beg);

    // Allocate memory for the grid
    char** grid = new char*[rows];
    for (int i = 0; i < rows; i++) {
        grid[i] = new char[cols];
    }

    // Fill the grid
    int row = 0, col = 0;
    char ch;
    while (gridFile >> ch) {
        if (ch == '\n') continue; // Ignore newlines
        grid[row][col] = tolower(ch); // Convert to lowercase
        col++;
        if (col == cols) {
            col = 0;
            row++;
        }
    }

    // Close the grid file
    gridFile.close();

    // Process each word in the word list
    string word;
    while (getline(wordsFile, word)) {
        // Remove spaces and convert to lowercase
        string filteredWord;
        for (char c : word) {
            if (!isspace(c)) filteredWord += tolower(c);
        }

        int foundRow = -1, foundCol = -1;
        searchWord(grid, rows, cols, filteredWord.c_str(), foundRow, foundCol);
        cout << foundRow << "," << foundCol << endl;
    }

    // Close the word list file
    wordsFile.close();

    // Free allocated memory
    for (int i = 0; i < rows; i++) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}

void searchWord(char** grid, int rows, int cols, const char* word, int& foundRow, int& foundCol) {
    int wordLength = strlen(word);

    // Search in all 8 possible directions
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            for (int d = 0; d < 8; d++) {
                if (searchInDirection(grid, rows, cols, word, r, c, directions[d][0], directions[d][1])) {
                    foundRow = r;
                    foundCol = c;
                    return;
                }
            }
        }
    }
    foundRow = -1;
    foundCol = -1;
}

bool searchInDirection(char** grid, int rows, int cols, const char* word, int startRow, int startCol, int dirRow, int dirCol) {
    int wordLength = strlen(word);

    for (int i = 0; i < wordLength; i++) {
        int newRow = startRow + i * dirRow;
        int newCol = startCol + i * dirCol;

        // Check if the new position is within bounds
        if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols) {
            return false;
        }

        // Check if the character matches
        if (grid[newRow][newCol] != word[i]) {
            return false;
        }
    }
    return true;
}
