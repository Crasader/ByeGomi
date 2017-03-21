#include "GameManager.h"

GameManager* GameManager::_instance = nullptr;

GameManager* GameManager::getInstance() {
	if (_instance == nullptr)
		_instance = new GameManager;
	return _instance;
}
