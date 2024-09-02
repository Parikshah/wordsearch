// Pari K. Shah

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Function to load words from a file and create a dictionary
unordered_map<string, string> loadWords(const string& filename) {
    unordered_map<string, string> words_dict;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open words file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string forward_word = line.substr(0, pos);
            string backward_word = line.substr(pos + 1);
            words_dict[forward_word] = backward_word;
        }
    }

    file.close();
    return words_dict;
}

// Function to read the word grid from a file
vector<vector<string>> readWordGrid(const string& filename) {
    vector<vector<string>> word_grid;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open grid file: " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<string> row;
        size_t pos = 0;
        while ((pos = line.find(' ')) != string::npos) {
            row.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        row.push_back(line); // Add the last word in the row
        word_grid.push_back(row);
    }

    file.close();
    return word_grid;
}

// Function to search for a word within a line (horizontal or vertical)
pair<int, int> searchWithinLine(const vector<string>& line, const string& word) {
    string line_str;
    for (const auto& w : line) {
        line_str += w;
    }

    size_t pos = line_str.find(word);
    if (pos != string::npos) {
        int col = pos;
        for (size_t i = 0; i < line.size(); ++i) {
            if (col < line[i].size()) {
                return {col, static_cast<int>(i)};
            }
            col -= line[i].size();
        }
    }

    return {-1, -1};
}

// Function to search for words horizontally in the word grid
vector<pair<string, pair<int, int>>> searchHorizontal(const vector<vector<string>>& word_grid, const unordered_map<string, string>& words_dict) {
    vector<pair<string, pair<int, int>>> found_words;
    for (int row = 0; row < word_grid.size(); ++row) {
        for (const auto& pair : words_dict) {
            const string& word = pair.first;
            auto result = searchWithinLine(word_grid[row], word);
            if (result.first != -1) {
                found_words.push_back({word, {row, result.first}});
            }
        }
    }
    return found_words;
}

// Function to transpose the word grid for vertical search
vector<vector<string>> transposeGrid(const vector<vector<string>>& word_grid) {
    int rows = word_grid.size();
    int cols = word_grid[0].size();
    vector<vector<string>> transposed_grid(cols, vector<string>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed_grid[j][i] = word_grid[i][j];
        }
    }

    return transposed_grid;
}

// Function to search for words vertically in the transposed grid
vector<pair<string, pair<int, int>>> searchVertical(const vector<vector<string>>& word_grid, const unordered_map<string, string>& words_dict) {
    auto transposed_grid = transposeGrid(word_grid);
    vector<pair<string, pair<int, int>>> found_words;
    for (int col = 0; col < transposed_grid.size(); ++col) {
        for (const auto& pair : words_dict) {
            const string& word = pair.first;
            auto result = searchWithinLine(transposed_grid[col], word);
            if (result.first != -1) {
                found_words.push_back({word, {result.second, col}});
            }
        }
    }
    return found_words;
}

// Main function to execute the word search
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <words file> <grid file>" << endl;
        return 1;
    }

    string words_file = argv[1];
    string grid_file = argv[2];

    unordered_map<string, string> words_dict = loadWords(words_file);
    vector<vector<string>> word_grid = readWordGrid(grid_file);

    vector<pair<string, pair<int, int>>> found_words;

    // Search horizontally
    vector<pair<string, pair<int, int>>> horizontal_results = searchHorizontal(word_grid, words_dict);
    found_words.insert(found_words.end(), horizontal_results.begin(), horizontal_results.end());

    // Search vertically
    vector<pair<string, pair<int, int>>> vertical_results = searchVertical(word_grid, words_dict);
    found_words.insert(found_words.end(), vertical_results.begin(), vertical_results.end());

    // Print results
    for (const auto& pair : found_words) {
        cout << pair.second.first << "," << pair.second.second << endl;
    }

    return 0;
}

