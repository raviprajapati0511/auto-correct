#include "autocorrect.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

// constructor
AutoCorrect::AutoCorrect(const std::string &dictionary_filename) noexcept : dictionary_filename(dictionary_filename) {}

// setter method for the word to check
void AutoCorrect::setWord(const std::string &wrong_word) noexcept {
    this->wrong_word = wrong_word;
}

// getter method for the word to check
std::string AutoCorrect::getWord() const noexcept {
    return wrong_word;
}

// getter method for the dictionary file name
std::string AutoCorrect::getDictionaryFilename() const noexcept {
    return dictionary_filename;
}

// 1. Exact match
bool AutoCorrect::checkSpelling() const noexcept {
    std::ifstream dictionary(dictionary_filename);
    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word < dict_word) return false;
        if (wrong_word == dict_word) return true;
    }
    return false;
}

// bool binarySearchWord(const string& word, const string& filename) {
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cerr << "Failed to open dictionary file." << endl;
//         return false;
//     }

//     vector<string> dict;
//     string dict_word;
//     while (file >> dict_word) {
//         dict.push_back(dict_word);
//     }
//     file.close();

//     // Ensure the dictionary is sorted
//     sort(dict.begin(), dict.end());

//     // Perform binary search using lower_bound
//     auto it = lower_bound(dict.begin(), dict.end(), word);
    
//     if (it != dict.end() && *it == word) {
//         int index = it - dict.begin();
//         cout << "Word found at index " << index << "." << endl;
//         return true;
//     } else {
//         cout << "Word not found in dictionary." << endl;
//         return false;
//     }
// }

// 2. Anagram suggestion
bool AutoCorrect::checkLetterArrangement() const noexcept {
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);
    std::cout << "*** Incorrect Arrangement ***\t:\t";
    std::string wrong_word_copy = wrong_word;
    std::sort(wrong_word_copy.begin(), wrong_word_copy.end());
    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() == dict_word.length()) {
            std::string dict_word_copy = dict_word;
            std::sort(dict_word_copy.begin(), dict_word_copy.end());
            if (wrong_word_copy == dict_word_copy) {
                std::cout << dict_word << ", ";
                found = true;
                ++word_count;
            }
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 3. Fix letters in wrong positions
bool AutoCorrect::checkExchangedLetters(const unsigned int &EXCHANGED) const noexcept {
    if (EXCHANGED == 0) return false;

    std::cout << "*** Exchanged " << EXCHANGED << " Character(s) ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() != dict_word.length()) continue;
        unsigned int diff = 0;
        for (size_t i = 0; i < wrong_word.length(); ++i)
            if (wrong_word[i] != dict_word[i]) ++diff;
        if (diff == EXCHANGED) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 4. Missing letter suggestions
bool AutoCorrect::checkMissingLetters(const unsigned int &MISSING) const noexcept {
    if (MISSING == 0) return false;
    std::cout << "*** Missing " << MISSING << " Character(s) ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() + MISSING != dict_word.length()) continue;
        size_t i = 0, j = 0, misses = 0;
        while (i < wrong_word.length() && j < dict_word.length()) {
            if (wrong_word[i] == dict_word[j]) {
                ++i; ++j;
            } else {
                ++j; ++misses;
            }
        }
        misses += dict_word.length() - j;
        if (misses == MISSING) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 5. Extra letters
bool AutoCorrect::checkExtraLetters(const unsigned int &EXTRA) const noexcept {
    if (EXTRA == 0) return false;
    std::cout << "*** Extra " << EXTRA << " Character(s) ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() != dict_word.length() + EXTRA) continue;
        size_t i = 0, j = 0, extras = 0;
        while (i < wrong_word.length() && j < dict_word.length()) {
            if (wrong_word[i] == dict_word[j]) {
                ++i; ++j;
            } else {
                ++i; ++extras;
            }
        }
        extras += wrong_word.length() - i;
        if (extras == EXTRA) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 6. Combined missing + extra
bool AutoCorrect::checkMissingAndExtraLetters(const unsigned int &MISSING, const unsigned int &EXTRA) const noexcept {
    if (MISSING == 0 || EXTRA == 0) return false;
    std::cout << "*** " << MISSING << " Missing & " << EXTRA << " Extra Characters ***\t:\t";
    bool found = false;
    unsigned int word_count = 0;
    std::ifstream dictionary(dictionary_filename);

    for (std::string dict_word; dictionary >> dict_word;) {
        if (wrong_word.length() + MISSING != dict_word.length() + EXTRA) continue;
        size_t i = 0, j = 0, miss = 0, extra = 0;
        while (i < wrong_word.length() && j < dict_word.length()) {
            if (wrong_word[i] == dict_word[j]) {
                ++i; ++j;
            } else {
                ++i; ++extra;
                ++miss;
                ++j;
            }
        }
        extra += wrong_word.length() - i;
        miss += dict_word.length() - j;
        if (extra == EXTRA && miss == MISSING) {
            std::cout << dict_word << ", ";
            found = true;
            ++word_count;
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 7. All-in-one checker
bool AutoCorrect::checkAll(const unsigned int &MISSING, const unsigned int &EXTRA, const bool &is_disarranged) const noexcept {
    if (!(MISSING || EXTRA)) return false;
    bool found = false;
    std::ifstream dictionary(dictionary_filename);

    std::string wrong = wrong_word;
    if (is_disarranged)
        std::sort(wrong.begin(), wrong.end());

    std::cout << "*** Max " << MISSING << " Missing, " << EXTRA << " Extra" << (is_disarranged ? ", Disarranged" : "") << " ***\t:\t";

    unsigned int word_count = 0;
    for (std::string dict_word; dictionary >> dict_word;) {
        std::string dict = dict_word;
        if (is_disarranged)
            std::sort(dict.begin(), dict.end());

        if ((wrong.length() <= dict.length() + EXTRA) && (wrong.length() + MISSING >= dict.length())) {
            size_t i = 0, j = 0, miss = 0, extra = 0;
            while (i < wrong.length() && j < dict.length()) {
                if (wrong[i] == dict[j]) {
                    ++i; ++j;
                } else {
                    ++i; ++extra;
                    ++miss; ++j;
                }
            }
            extra += wrong.length() - i;
            miss += dict.length() - j;
            if (miss <= MISSING && extra <= EXTRA) {
                std::cout << dict_word << ", ";
                found = true;
                ++word_count;
            }
        }
    }
    if (found) std::cout << "\b\b\t(" << word_count << ")" << std::endl;
    return found;
}

// 8. Sort the dictionary file and save to new one
void AutoCorrect::sortDictionary(const char *unsorted_dictionary_filename, const char *sorted_dictionary_filename) noexcept(false) {
    try {
        std::ifstream unsorted(unsorted_dictionary_filename);
        std::set<std::string> words;
        for (std::string w; unsorted >> w; words.insert(w));
        std::ofstream sorted(sorted_dictionary_filename);
        for (const auto &word : words)
            sorted << word << "\n";
    } catch (const std::exception &e) {
        std::cerr << "[ERROR]: " << e.what() << std::endl;
    }
}
