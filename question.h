#pragma once
#include <string>

// One quiz question. Always 4 choices because we only request type=multiple.
struct Question {
    std::string text{};          // default empty string
    std::string choices[4]{};    // already shuffled
    int correctIndex{0};         // 0-3, where the correct answer landed after shuffling
};
