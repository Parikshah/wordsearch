// Pari K. Shah

#include <iostream>
#include <string>
#include <cctype>
#include <fstream>

using namespace std;

string piglatin(const string& s) {
    if (s.empty()) return "";

    string vowels = "aeiouAEIOU";
    string lower_word = s;
    for (char &c : lower_word) c = tolower(c);

    string result;
    if (vowels.find(lower_word[0]) != string::npos) {
        result = lower_word + "way";
    } else {
        size_t first_vowel_pos = lower_word.find_first_of(vowels);
        if (first_vowel_pos != string::npos) {
            result = lower_word.substr(first_vowel_pos) + lower_word.substr(0, first_vowel_pos) + "ay";
        } else {
            result = lower_word + "ay";
        }
    }

    // Capitalize the first letter of the result if the input was capitalized
    if (isupper(s[0])) {
        result[0] = toupper(result[0]);
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " input.txt" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    string word;
    while (file >> word) {
        cout << piglatin(word) << endl;
    }

    file.close();
    return 0;
}
