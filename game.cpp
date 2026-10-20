#include "game.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

// ---------- Input validation ----------
int readInt(int low, int high) {
    int value;
    while (true) {
        cout << "> ";
        if (cin >> value && value >= low && value <= high)
            return value;

        cin.clear();                                          // reset error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // throw away bad input
        cout << "Please enter a number from " << low << " to " << high << ".\n";
    }
}

// ---------- Ask one question ----------
bool askQuestion(const Question& q, int number, int total) {
    cout << "\nQuestion " << number << "/" << total << "\n";
    cout << q.text << "\n\n";

    for (int i = 0; i < 4; i++)
        cout << "  " << (i + 1) << ") " << q.choices[i] << "\n";
    cout << "\n";

    int answer = readInt(1, 4) - 1;   // convert 1-4 to index 0-3

    if (answer == q.correctIndex) {
        cout << "Correct!\n";
        return true;
    }
    cout << "Wrong! The answer was: " << q.choices[q.correctIndex] << "\n";
    return false;
}

// ---------- High score file ----------
int loadHighScore() {
    ifstream file("highscore.txt");
    int best = 0;
    if (file.is_open())
        file >> best;
    return best;
}

void saveHighScore(int percent) {
    ofstream file("highscore.txt");
    file << percent;
}

// ---------- Full game round ----------
void playGame(const vector<Question>& questions) {
    int score = 0;
    int total = questions.size();

    for (int i = 0; i < total; i++) {
        if (askQuestion(questions[i], i + 1, total))
            score++;
    }

    int percent = (score * 100) / total;

    cout << "\n==============================\n";
    cout << " Final score: " << score << "/" << total << " (" << percent << "%)\n";

    if (percent >= 90)      cout << " Excellent!\n";
    else if (percent >= 70) cout << " Great job!\n";
    else if (percent >= 50) cout << " Not bad!\n";
    else                    cout << " Keep practicing!\n";

    if (percent > loadHighScore()) {
        saveHighScore(percent);
        cout << " New high score!\n";
    }
    cout << "==============================\n";
}
