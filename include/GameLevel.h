#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SpriteRenderer.h>
#include <GameObject.h>
#include <vector>


class GameLevel {
public:

	std::vector<GameObject> Bricks;
	GameLevel() {};
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(SpriteRenderer& renderer);
	bool GameLevel::IsCompleted();

private:
	void Init(std::vector<std::vector<unsigned int >> tiledata, unsigned int levelWidth, unsigned int levelHeight);

};
