#pragma  once 
#include "GameObject.h"\
#include <glm/glm.hpp>

class Ball : public GameObject {
public:
	bool isStuck;
	float Radius;
	Ball();
	Ball::Ball(glm::vec2 Pos, float radius, glm::vec2 Velocity, Texture2D sprite);



	void Move(float dt, unsigned int windowWidth);
	void      Reset(glm::vec2 position, glm::vec2 velocity);

};