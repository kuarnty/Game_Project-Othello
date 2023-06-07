#pragma once

enum Disk
{
	Black,
	White,
	None
};

enum Winner
{
	Black,
	White,
	Draw,
	None
};

struct BoardState
{
	int width;
	int height;
	Disk** disks;
};

struct Scores
{
	int black;
	int white;
};

struct TimeLimit
{
	float minutes;
	float seconds;
};

struct PlacablePoints
{
	int count;
	bool** isPlacables;
};

static class GameState
{
public:
	static bool isRunning;

	static BoardState boardstate;
	static Scores scores;

	static bool isTimeLimited;
	static TimeLimit timeLimit;
	static TimeLimit leftTime;

	static Disk currentTurn;
	static Winner winner;
	static PlacablePoints placablesOfBlack;
	static PlacablePoints placablesOfWhite;

	GameState();

	static int GetLeftMinutesInt();
	static int GetLeftSecondsInt();

	static Winner GetWinner();

	static bool IsPlacableAt(Disk disk, int i, int j);

	static void CalculateScores();
	static void CalculatePlacables();
	static bool CheckGameEnd();

	static Disk GetNextTurn();

	static void FinishGame();

	// update board with the recently placed disk
	static void UpdateBoard(int i, int j);
	// calculate things and check if the game end
	static void EvaluateGameState();
	static bool HandleTime(float deltaTime);
	static bool Update(float deltaTime);
};