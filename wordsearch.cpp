// Pari K. Shah

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// Function prototypes
bool searchInDirection(char** grid, int rows, int cols, const char* word, int startRow, int startCol, int dirRow, int dirCol);
void searchWord(char** grid, int rows, int cols, const char* word, int& foundRow, int& foundCol);
char** loadGrid(const char* filename, int& rows, int& cols);
void loadWords(const char* filename, char**& words, int& numWords);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " gridfile.txt wordfile.txt" << endl;
        return 1;
    }

    const char* gridFile = argv[1];
    const char* wordsFile = argv[2];

    int rows, cols;
    char** grid = loadGrid(gridFile, rows, cols);

    int numWords = 0;
    char** words = nullptr;
    loadWords(wordsFile, words, numWords);

    for (int i = 0; i < numWords; ++i) {
        int foundRow = -1, foundCol = -1;
        searchWord(grid, rows, cols, words[i], foundRow, foundCol);
        cout << foundRow << "," << foundCol << endl;
        delete[] words[i];
    }
    delete[] words;

    // Free allocated memory for grid
    for (int i = 0; i < rows; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    return 0;
}

char** loadGrid(const char* filename, int& rows, int& cols) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        exit(1);
    }

    // Count rows and columns
    string line;
    rows = 0;
    cols = 0;
    while (getline(file, line)) {
        if (cols == 0) cols = line.length();
        ++rows;
    }

    // Rewind the file
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for grid
    char** grid = new char*[rows];
    for (int i = 0; i < rows; ++i) {
        grid[i] = new char[cols];
    }

    // Read grid into the allocated memory
    int row = 0;
    while (getline(file, line)) {
        int col = 0;
        for (char c : line) {
            if (!isspace(c)) {
                grid[row][col++] = tolower(c);
            }
        }
        ++row;
    }

    file.close();
    return grid;
}

void loadWords(const char* filename, char**& words, int& numWords) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        exit(1);
    }

    // Count number of words
    numWords = 0;
    string line;
    while (getline(file, line)) {
        numWords++;
    }

    // Rewind the file
    file.clear();
    file.seekg(0, ios::beg);

    // Allocate memory for words
    words = new char*[numWords];
    for (int i = 0; i < numWords; ++i) {
        getline(file, line);
        // Remove spaces and convert to lowercase
        string filteredWord;
        for (char c : line) {
            if (!isspace(c)) {
                filteredWord += tolower(c);
            }
        }
        // Allocate memory for each word
        words[i] = new char[filteredWord.length() + 1];
        strcpy(words[i], filteredWord.c_str());
    }

    file.close();
}

void searchWord(char** grid, int rows, int cols, const char* word, int& foundRow, int& foundCol) {
      int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    foundRow = -1;
    foundCol = -1;

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
