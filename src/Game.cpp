#include "game.h"
#include "SpriteRenderer.h"
#include "ResourceManger.h"
#include "GameLevel.h"

#define LEVEL 3

SpriteRenderer* Renderer;
GameObject* player; 
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
	
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));






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
                player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (player->Position.x <= this->Width - player->Size.x)
                player->Position.x += velocity;
        }
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
	}

}