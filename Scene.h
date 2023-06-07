#pragma once

enum class SceneNumber
{
	Title,
	Setting,
	NewGame,
	Game,
	Pause,
	Finish,
	None
};

static class SceneManager
{
public:
	SceneManager();

	static SceneNumber GetCurrentSceneNumber();
	static SceneNumber GetPrevSceneNumber();

	static void ChangeSceneTo(SceneNumber nextSceneNumber);

private:
	static SceneNumber currentSceneNumber;
	static SceneNumber prevSceneNumber;
};