#include "game.h"
#include "SpriteRenderer.h"
#include "ResourceManger.h"
#include "GameLevel.h"
#include "Ball.h"
#include <iostream>

#define LEVEL 3

SpriteRenderer* Renderer;
GameObject* player; 
Ball* ball; 

const glm::vec2 INITIAL_BALL_VELOCITY(150.0f, -450.0f);
const float BALL_RADIUS = 12.5f;



Game::Game(unsigned int width, unsigned int height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{


}

Game::~Game()
{

}


void Game::Init(){

   
	
    




    //Load Textures
    
	ResourceManager::LoadTexture(RESOURCES_PATH"Textures/paddle.png", true, "player");
    ResourceManager::LoadTexture(RESOURCES_PATH"Textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture(RESOURCES_PATH"Textures/block.png", false, "block");
    ResourceManager::LoadTexture(RESOURCES_PATH "Textures /block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture(RESOURCES_PATH "Textures/background.jpg", false, "background");
    ResourceManager::LoadTexture(RESOURCES_PATH "Textures/ball.png", true, "ball");
	GameLevel one; one.Load(RESOURCES_PATH "Levels/level1.lvl", this->Width,this->Height/2);
	GameLevel two; two.Load(RESOURCES_PATH "Levels/level2.lvl", this->Width, this->Height / 2);
	GameLevel three; three.Load(RESOURCES_PATH "Levels/level3.lvl", this->Width, this->Height / 2);
	GameLevel four; four.Load(RESOURCES_PATH "Levels/level4.lvl", this->Width, this->Height / 2);

	this->levels.push_back(one);
	this->levels.push_back(two);
	this->levels.push_back(three);
	this->levels.push_back(four);

    this->currentLevel = LEVEL; 

	glm::vec2 playerPos = glm::vec2(
		this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->Height - PLAYER_SIZE.y
	);

    glm::vec2 ballPos = glm::vec2(
        playerPos.x + PLAYER_SIZE.x / 2.0f - BALL_RADIUS / 2.0f,
        playerPos.y - BALL_RADIUS * 2.0f
	);



	
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));
	ball = new Ball(ballPos,BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
	std::vector<glm::vec2> PLAYER_COLLIDER_POINTS = {
		glm::vec2(-PLAYER_SIZE.x / 2.0f, -PLAYER_SIZE.y / 2.0f),
		glm::vec2(PLAYER_SIZE.x / 2.0f, -PLAYER_SIZE.y / 2.0f),
		glm::vec2(PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y / 2.0f),
		glm::vec2(-PLAYER_SIZE.x / 2.0f, PLAYER_SIZE.y / 2.0f)
	
	};
	std::vector<glm::vec2> BALL_COLLIDER_POINTS = { //6 point collider
		glm::vec2(-BALL_RADIUS, -BALL_RADIUS),
		glm::vec2(BALL_RADIUS, -BALL_RADIUS),
		glm::vec2(BALL_RADIUS, BALL_RADIUS),
		glm::vec2(-BALL_RADIUS, BALL_RADIUS),
		glm::vec2(-BALL_RADIUS, 0.0f),
		glm::vec2(BALL_RADIUS, 0.0f)
	};
	
	// Create colliders
	Collider ballCollider(BALL_COLLIDER_POINTS);
	Collider playerCollider(PLAYER_COLLIDER_POINTS);
	
	ball->AddCollider(ballCollider);
	glm::vec2 ballCenter = ball->Position + ball->Size / 2.0f;
	ball->collider.PlacePointsAroundCentre(ballCenter);
	
    player->AddCollider(playerCollider);
	glm::vec2 playerCenter = player->Position + player->Size / 2.0f;
	player->collider.PlacePointsAroundCentre(playerCenter);
   // print the collider points 

	//add a collider to each brick 
	for (GameObject& brick : this->levels[this->currentLevel].Bricks)
	{
		std::vector<glm::vec2> brickColliderPoints = {
			glm::vec2(-brick.Size.x / 2.0f, -brick.Size.y / 2.0f),
			glm::vec2(brick.Size.x / 2.0f, -brick.Size.y / 2.0f),
			glm::vec2(brick.Size.x / 2.0f, brick.Size.y / 2.0f),
			glm::vec2(-brick.Size.x / 2.0f, brick.Size.y / 2.0f)
		};
		Collider brickCollider(brickColliderPoints);
		brick.AddCollider(brickCollider);
		// Place collider points around the center of the brick
		glm::vec2 brickCenter = brick.Position + brick.Size / 2.0f;
		brick.collider.PlacePointsAroundCentre(brickCenter);
	}



    // load shaders
    ResourceManager::LoadShader( RESOURCES_PATH "Shaders/sprite.vert",RESOURCES_PATH "Shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
 
}

void Game::Update(float dt)
{
    ball->Move(dt, this->Width);
    
    // Update ball collider position when ball moves
    if (!ball->isStuck) {
        ball->collider.PlacePointsAroundCentre(ball->Position + ball->Size / 2.0f);
    }
    
    if (ball->isStuck)
    {
		ball->Position = glm::vec2(
			player->Position.x + player->Size.x / 2.0f - ball->Radius,
			player->Position.y - ball->Radius * 2.0f
		);
		// Update ball collider position when stuck to player
		ball->collider.PlacePointsAroundCentre(ball->Position + ball->Size / 2.0f);
    }

	if (ball->Position.y >= this->Height)
	{
		// reset ball position
		ball->Reset(
			glm::vec2(player->Position.x + player->Size.x / 2.0f - ball->Radius, player->Position.y - ball->Radius * 3.0f),
			INITIAL_BALL_VELOCITY
		);
		// Update collider position after reset
		ball->collider.PlacePointsAroundCentre(ball->Position + ball->Size / 2.0f);
	}

	if (ball->collider.CheckCollision(player->collider) && !ball->isStuck)
	{
		// Simple collision response - reverse Y velocity and add some X variation based on hit position
		ball->Velocity.y = -abs(ball->Velocity.y); // Always bounce upward
		
		// Add horizontal velocity based on where the ball hits the paddle
		float paddleCenter = player->Position.x + player->Size.x / 2.0f;
		float ballCenter = ball->Position.x + ball->Size.x / 2.0f;
		float distance = ballCenter - paddleCenter;
		float percentage = distance / (player->Size.x / 2.0f);
		
		// Add horizontal velocity based on hit position (max 150 units)
		ball->Velocity.x += percentage * 200.0f;
	}
	//check collision with each brick 

	for (GameObject& brick : this->levels[this->currentLevel].Bricks)
	{
		if (!brick.Destroyed && ball->collider.CheckCollision(brick.collider))
		{
			// Simple collision response - reverse Y velocity
			ball->Velocity.y = -ball->Velocity.y;
			// Mark the brick as destroyed
			brick.Destroyed = true;
			// Update the collider position after destruction
			brick.collider.PlacePointsAroundCentre(brick.Position + brick.Size / 2.0f);
		}
	}
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
			if (player->Position.x >= 0.0f)
			{
				player->Position.x -= velocity;
				player->collider.Move(glm::vec2(-velocity, 0.0f));
				
			}
            
				
				
        }
        if (this->Keys[GLFW_KEY_D])
        {
			if (player->Position.x <= this->Width - player->Size.x)
			{
				player->Position.x += velocity;
				player->collider.Move(glm::vec2(velocity, 0.0f));

			}
                
				
				
			  
			  

        }
        if (this->Keys[GLFW_KEY_SPACE])
			
            ball->isStuck = false;
		
    }

}


void Game::Render()
{
    Renderer->DrawSprite(ResourceManager::GetTexture("absolutecinema"),
        glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	Renderer->DrawSprite(ResourceManager::GetTexture("background"),
		glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    
	if (this->State == GAME_ACTIVE)
	{
		this->levels[this->currentLevel].Draw(*Renderer);
		player->Draw(*Renderer);
		ball->Draw(*Renderer);
	
	}

}