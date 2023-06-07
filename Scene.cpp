#pragma once

#include "Scene.h"

SceneManager::SceneManager()
{
	currentSceneNumber = prevSceneNumber = SceneNumber::None;
}

SceneNumber SceneManager::GetCurrentSceneNumber()
{
	return currentSceneNumber;
}

SceneNumber SceneManager::GetPrevSceneNumber()
{
	return prevSceneNumber;
}

void SceneManager::ChangeSceneTo(SceneNumber nextSceneNumber)
{
	prevSceneNumber = currentSceneNumber;
	currentSceneNumber = nextSceneNumber;
}