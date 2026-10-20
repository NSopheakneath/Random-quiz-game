#pragma once
#include <string>
#include <vector>
#include "question.h"
using namespace std;

// Builds the opentdb URL. category 0 = any, difficulty "any" = any.
string buildUrl(int amount, int category, const string& difficulty);

// Uses curl (built into Windows 10/11) to save the response to questions.json.
bool downloadQuestions(const string& url);

// Replaces HTML entities like &quot; and &#039; with normal characters.
string cleanText(string text);

// Reads the JSON file into the vector.
// Returns the API response_code (0 = ok, 1 = not enough questions, 5 = rate limited)
// or -1 if the file is missing/empty/invalid (usually no internet).
int loadQuestions(const string& filename, vector<Question>& questions);
