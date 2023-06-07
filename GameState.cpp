#include "GameState.h"
#include "Scene.h"

GameState::GameState()
{
	isRunning = false;
	boardstate.width = 8;
	boardstate.height = 8;
	boardstate.disks = new Disk * [8];
	for (int i = 0; i < 8; i++)
		boardstate.disks[i] = new Disk[8];
	scores.black = scores.white = 0;
	isTimeLimited = false;
	timeLimit = leftTime = { 0, 0 };
	currentTurn = Disk::Black;
	winner = Winner::None;
	placablesOfBlack.isPlacables = new bool * [8];
	for (int i = 0; i < 8; i++)
		placablesOfBlack.isPlacables[i] = new bool[8];
	placablesOfBlack.count = 0;
	placablesOfWhite.isPlacables = new bool * [8];
	for (int i = 0; i < 8; i++)
		placablesOfWhite.isPlacables[i] = new bool[8];
	placablesOfWhite.count = 0;
}


int GameState::GetLeftMinutesInt()
{
	return (int)leftTime.minutes;
}

int GameState::GetLeftSecondsInt()
{
	return (int)leftTime.seconds;
}


Winner GameState::GetWinner()
{
	// not finished yet
	return Winner::None;

	// draw
	return Winner::Draw;

	// winner is Black
	return Winner::Black;

	// winner is White
	return Winner::White;
}


bool GameState::IsPlacableAt(Disk disk, int i, int j)
{
	// if the "disk" is placable at boardstate.disks[i][j],
	return true;
}


void GameState::CalculateScores()
{
	scores = { 0, 0 };
	for (int i = 0; i < boardstate.height; i++)
	{
		for (int j = 0; j < boardstate.width; j++)
		{
			if (boardstate.disks[i][j] == Disk::Black)
				scores.black++;
			else if (boardstate.disks[i][j] == Disk::White)
				scores.white++;
		}
	}
}

void GameState::CalculatePlacables()
{
	// calculate points that is placable for each black and white
	// and set it to each struct (also count the number)

	// using isPlacableAt
}

bool GameState::CheckGameEnd()
{
	if (placablesOfBlack.count == 0 && placablesOfBlack.count == 0)
		return true;
	else
		return false;
}


Disk GameState::GetNextTurn()
{
	if (CheckGameEnd())
	{
		FinishGame();
		return Disk::None;
	}
	else
	{
		if (currentTurn == Disk::Black && placablesOfWhite.count != 0)
			return Disk::White;
		else if (currentTurn == Disk::White && placablesOfBlack.count != 0)
			return Disk::Black;
		else // actually, never occurs
			return Disk::None;;
	}
}


void GameState::FinishGame()
{
	winner = GetWinner();

	// score must be caculated already
	// CalculateScores();

	SceneManager::ChangeSceneTo(SceneNumber::Finish);
}


void GameState::UpdateBoard(int i, int j)
{
	// change board state (reverse disks)
}

void GameState::EvaluateGameState()
{
	// CalculatePlacables()
	// CalculateScores()
	// CheckGameEnd()
	// 
}

bool GameState::HandleTime(float deltaTime)
{
	return true;
}

bool GameState::Update(float deltaTime)
{
	// check input -> check whether placing happened
	
	// if placing happened, 
	// change board state by the disk
}