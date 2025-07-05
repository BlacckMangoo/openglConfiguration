#include "Ball.h"


Ball::Ball ()
    : GameObject(), Radius(12.5f), isStuck(true) {
}

Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), isStuck(true) {
}

void Ball::Move(float dt, unsigned int windowWidth) 
{
	if (!this->isStuck)
	{
		this->Position += this->Velocity * dt;
		// if the ball goes out of bounds, reset it
		if (this->Position.x <= 0.0f)
		{
			this->Position.x = 0.0f;
			this->Velocity.x = -this->Velocity.x;
		}
		else if (this->Position.x + this->Size.x >= windowWidth)
		{
			this->Position.x = windowWidth - this->Size.x;
			this->Velocity.x = -this->Velocity.x;
		}
		if (this->Position.y <= 0.0f)
		{
			this->Position.y = 0.0f;
			this->Velocity.y = -this->Velocity.y;
		}
	}
}

void Ball::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->isStuck = true;
}



