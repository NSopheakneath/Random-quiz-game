#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "question.h"
#include "api.h"
#include "game.h"
using namespace std;

// ---------- Menus ----------
int chooseCategory() {
    const int COUNT = 9;
    string names[COUNT] = { "Any", "General Knowledge", "Science & Nature", "Computers",
                            "Sports", "Geography", "History", "Film", "Video Games" };
    int ids[COUNT]      = { 0, 9, 17, 18, 21, 22, 23, 11, 15 };

    cout << "\nChoose a category:\n";
    for (int i = 0; i < COUNT; i++)
        cout << "  " << (i + 1) << ") " << names[i] << "\n";

    int choice = readInt(1, COUNT);
    return ids[choice - 1];
}

string chooseDifficulty() {
    cout << "\nChoose a difficulty:\n";
    cout << "  1) Any\n  2) Easy\n  3) Medium\n  4) Hard\n";

    switch (readInt(1, 4)) {
        case 2:  return "easy";
        case 3:  return "medium";
        case 4:  return "hard";
        default: return "any";
    }
}

int chooseAmount() {
    cout << "\nHow many questions? (1-20)\n";
    return readInt(1, 20);
}

// ---------- Start one quiz ----------
void startQuiz() {
    int category = chooseCategory();
    string difficulty = chooseDifficulty();
    int amount = chooseAmount();

    string url = buildUrl(amount, category, difficulty);

    cout << "\nLoading questions...\n";

    vector<Question> questions;
    int code = -1;

    // Try twice: the API rate-limits to about 1 request per 5 seconds
    for (int attempt = 0; attempt < 2; attempt++) {
        questions.clear();

        if (!downloadQuestions(url)) {
            code = -1;
            break;
        }

        code = loadQuestions("questions.json", questions);
        if (code != 5)
            break;

        cout << "Too many requests, waiting 5 seconds...\n";
        system("timeout /t 5 /nobreak >nul");   // Windows wait
    }

    if (code == 0)
        playGame(questions);
    else if (code == 1)
        cout << "\nNot enough questions for that combination. Try fewer questions or a different category.\n";
    else if (code == 5)
        cout << "\nThe API is busy. Wait a few seconds and try again.\n";
    else
        cout << "\nCouldn't load questions. Check your internet connection.\n";
}

// ---------- Main menu ----------
int main() {
    bool running = true;

    while (running) {
        cout << "\n==============================\n";
        cout << "        RANDOM QUIZ GAME\n";
        cout << "==============================\n";
        cout << "  1) Play\n";
        cout << "  2) View high score\n";
        cout << "  3) Quit\n";

        switch (readInt(1, 3)) {
            case 1:
                startQuiz();
                break;
            case 2:
                cout << "\nBest score: " << loadHighScore() << "%\n";
                break;
            case 3:
                running = false;
                cout << "Thanks for playing!\n";
                break;
        }
    }

    return 0;
}