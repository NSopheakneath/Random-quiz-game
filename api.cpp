#include "api.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <random>
#include "json.hpp"   // download from github.com/nlohmann/json (single header)
using namespace std;
using json = nlohmann::json;

// ---------- Build the URL ----------
string buildUrl(int amount, int category, const string& difficulty) {
    string url = "https://opentdb.com/api.php?amount=" + to_string(amount) + "&type=multiple";

    if (category != 0)
        url += "&category=" + to_string(category);

    if (difficulty != "any")
        url += "&difficulty=" + difficulty;

    return url;
}

// ---------- Download with curl ----------
bool downloadQuestions(const string& url) {
    remove("questions.json");   // delete old file so we never read stale data

    // The escaped quotes around the URL matter: without them '&' breaks the command
    string cmd = "curl -s \"" + url + "\" -o questions.json";
    int result = system(cmd.c_str());

    return result == 0;
}

// ---------- Clean HTML entities ----------
string cleanText(string text) {
    const int COUNT = 11;
    string from[COUNT] = { "&quot;", "&#039;", "&rsquo;", "&ldquo;", "&rdquo;", "&hellip;",
                           "&eacute;", "&lt;", "&gt;", "&shy;", "&amp;" };
    string to[COUNT]   = { "\"", "'", "'", "\"", "\"", "...",
                           "e", "<", ">", "", "&" };
    // &amp; is last on purpose so we don't decode things twice

    for (int i = 0; i < COUNT; i++) {
        size_t pos = 0;
        while ((pos = text.find(from[i], pos)) != string::npos) {
            text.replace(pos, from[i].length(), to[i]);
            pos += to[i].length();
        }
    }
    return text;
}

// ---------- Parse the JSON file ----------
int loadQuestions(const string& filename, vector<Question>& questions) {
    static mt19937 rng(random_device{}());   // seeded once, reused

    ifstream file(filename);
    if (!file.is_open())
        return -1;

    json data;
    try {
        data = json::parse(file);
    } catch (...) {
        return -1;   // empty or broken file (no internet)
    }

    int code = data["response_code"];
    if (code != 0)
        return code;

    for (auto& item : data["results"]) {
        string correct = cleanText(item["correct_answer"].get<string>());

        vector<string> answers;
        answers.push_back(correct);
        for (auto& wrong : item["incorrect_answers"])
            answers.push_back(cleanText(wrong.get<string>()));

        if (answers.size() != 4)
            continue;   // skip anything unexpected

        shuffle(answers.begin(), answers.end(), rng);

        Question q;
        q.text = cleanText(item["question"].get<string>());
        for (int i = 0; i < 4; i++) {
            q.choices[i] = answers[i];
            if (answers[i] == correct)
                q.correctIndex = i;
        }
        questions.push_back(q);
    }

    return questions.empty() ? -1 : 0;
}
