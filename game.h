#pragma once
#include <vector>
#include "question.h"
using namespace std;

// Keeps asking until the user types a whole number between low and high.
int readInt(int low, int high);

// Shows one question, returns true if the user answered correctly.
bool askQuestion(const Question& q, int number, int total);

// Runs a full round: asks every question, shows score, updates high score.
void playGame(const vector<Question>& questions);

// High score (best percentage) stored in highscore.txt
int loadHighScore();
void saveHighScore(int percent);
