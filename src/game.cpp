#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "text_renderer.h"

#include <iostream>
#include <sstream>
#include <fstream>

unsigned l1_objs = 5;
unsigned l2_objs = 10;
unsigned l3_objs = 15;
unsigned l4_objs = 20;

unsigned last_x[4];

// Game-related State data
SpriteRenderer *Renderer;
GameObject *Player;
GameObject *Menu;
TextRenderer *Text;

std::vector<GameObject> Coins;

float bg_position = 0.0f;

unsigned w_index = 0;
unsigned z_index = 0;
unsigned c_index = 0;

bool isfalling = false;
bool iswalking = true;

const char *walk_sprites[] = {
    "textures/walk1.png",
    "textures/walk2.png",
    "textures/walk3.png",
    "textures/walk4.png",
};

const char *zap_sprites[] = {
    "textures/newzap1.png",
    "textures/newzap2.png",
};

const char *coin_sprites[] = {
    "textures/coin1.png",
    "textures/coin2.png",
    "textures/coin1.png",
    "textures/coin3.png",
};

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
    delete Menu;
    delete Player;
    delete Text;
}

void Game::Init()
{
    // load shaders
    srand(time(0));
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/text.vs", "shaders/text.frag", nullptr, "text");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("text").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader myShader;

    myShader = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(myShader);
    // load textures
    ResourceManager::LoadTexture("textures/bg1.png", false, "background");
    ResourceManager::LoadTexture(walk_sprites[w_index], true, "player");
    ResourceManager::LoadTexture(zap_sprites[z_index], true, "zapper");
    ResourceManager::LoadTexture(coin_sprites[c_index], true, "coin");
    ResourceManager::LoadTexture("textures/menu1.png", true, "menu");
    // ResourceManager::LoadTexture("textures/gamend.png",true,"win");

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("poly/Poly-Regular.otf", 24);
    // ResourceManager::LoadTexture("textures/haha.png", true, "face");

    std::vector<glm::vec2> c_positions;

    //////////////////////// GENERATE LEVELS /////////////////////////////////

    std::ofstream f_one;

    f_one.open("levels/one.lvl", std::ios::out | std::ios::trunc);
    if (!f_one)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
    }

    unsigned x = 800, y = 40, rot = 0, h = 80, w = 220;
    unsigned l = 1;
    c_positions.push_back(glm::vec2(800, 300));


    f_one << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
    for (int i = 1; i < l1_objs; i++)
    {
        x = x + w + rand() % w + 200;
        y = 50 + (y + rand() % h) % (300); // Ensuring within screen
        h = 30 + rand() % 70;
        w = 100 + rand() % 100;
        rot = rand() % 360;
        last_x[0] = x;
        f_one << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
        if (y - 51 >= 80)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }

        if (y + h <= 415.0f && y + h + 80 <= 415.0f)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }
    }

    f_one.close();

    // level 2

    std::ofstream f_two;

    f_two.open("levels/two.lvl", std::ios::out | std::ios::trunc);
    if (!f_two)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
    }

    x = 800, y = 40, rot = 0, h = 80, w = 220;
    l = 2;
    c_positions.push_back(glm::vec2(800, 300));
    f_two << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
    for (int i = 1; i < l2_objs; i++)
    {
        x = x + w + rand() % w + 100;
        y = 50 + (y + rand() % h) % (300); // Ensuring within screen
        h = 30 + rand() % 70;
        w = 100 + rand() % 100;
        rot = rand() % 360;
        last_x[1] = x;
        f_two << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
        if (y - 51 >= 80)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }

        if (y + h <= 415.0f && y + h + 80 <= 415.0f)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }
    }

    f_two.close();

    // level 3

    std::ofstream f_three;

    f_three.open("levels/three.lvl", std::ios::out | std::ios::trunc);
    if (!f_three)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
    }

    x = 800, y = 40, rot = 0, h = 80, w = 220;
    l = 3;
    c_positions.push_back(glm::vec2(800, 300));
    f_three << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
    for (int i = 1; i < l3_objs; i++)
    {
        x = x + w + rand() % w + 50;
        y = 50 + (y + rand() % h) % (300); // Ensuring within screen
        h = 30 + rand() % 70;
        w = 100 + rand() % 100;
        rot = rand() % 360;
        last_x[2] = x;
        f_three << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
        if (y - 51 >= 80)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }

        if (y + h <= 415.0f && y + h + 80 <= 415.0f)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }
    }

    f_three.close();

    // level 4

    std::ofstream f_four;

    f_four.open("levels/four.lvl", std::ios::out | std::ios::trunc);
    if (!f_four)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
    }

    x = 800, y = 40, rot = 0, h = 80, w = 220;
    l = 4;
    c_positions.push_back(glm::vec2(800, 300));
    f_four << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
    for (int i = 1; i < l4_objs; i++)
    {
        x = x + w + rand() % w + 20;
        y = 50 + (y + rand() % h) % (300); // Ensuring within screen
        h = 30 + rand() % 70;
        w = 100 + rand() % 100;
        rot = rand() % 360;
        last_x[3] = x;
        f_four << x << " " << y << " " << rot << " " << h << " " << w << " " << l << std::endl;
        if (y - 51 >= 80)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
            c_positions.push_back(glm::vec2(x + 40, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
            c_positions.push_back(glm::vec2(x + 70, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
            c_positions.push_back(glm::vec2(x + 100, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }

        if (y + h <= 415.0f && y + h + 80 <= 415.0f)
        {
            c_positions.push_back(glm::vec2(x, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
            c_positions.push_back(glm::vec2(x + 40, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
            c_positions.push_back(glm::vec2(x + 70, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
            c_positions.push_back(glm::vec2(x + 100, y - 80));
            std::cout << c_positions.back().x << " " << c_positions.back().y << std::endl;
        }
    }

    f_four.close();

    for (int i = 0; i < 4; i++)
    {
        last_x[i] += 120;
    }
    /////////////////////// load levels  ////////////////////////////////////

    GameLevel one;
    one.Load("levels/one.lvl", this->Width, this->Height);
    GameLevel two;
    two.Load("levels/two.lvl", this->Width, this->Height);
    GameLevel three;
    three.Load("levels/three.lvl", this->Width, this->Height);
    GameLevel four;
    four.Load("levels/four.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;

    this->distance = 0;

    this->s_dis = 0;

    // configure game objects
    //  glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    glm::vec2 playerPos = glm::vec2(150.0f, 410.0f);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));
    Player->Dead = false;

    for (int i = 0; i < c_positions.size(); i++)
    {
        Coins.push_back(GameObject(c_positions[i], glm::vec2(40.0f, 40.0f), ResourceManager::GetTexture("coin"), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f));
        Coins[i].Destroyed = false;
    }
    Menu = new GameObject(glm::vec2(200.0f, 100.0f), glm::vec2(400.0f, 300.0f), ResourceManager::GetTexture("menu"));
}

void Game::Update(float dt)
{
    static float elapsed = 0;

    elapsed += dt;

    // std::cout << "iswalking : " << iswalking << std::endl;
    // std::cout << "isfalling : " << isfalling << std::endl;
    DoCollisions(0);
    DoCollisions(1);

    if (Player->Dead)
    {
        this->State = GAME_LOSE;
        ResourceManager::LoadTexture("textures/gameover.png", true, "menu");
        Menu->Sprite = ResourceManager::GetTexture("menu");
    }
    else
    {
        if (!iswalking && !isfalling) // space is down
        {
            ResourceManager::LoadTexture("textures/thrust.png", true, "player");
        }
        else if (!iswalking && isfalling)
        {
            ResourceManager::LoadTexture("textures/fall.png", true, "player");
        }

        if (elapsed >= 0.1)
        {
            elapsed = 0.0f;
            z_index = z_index + 1;
            z_index %= 2;
            ResourceManager::LoadTexture(zap_sprites[z_index], true, "zapper");
            if (iswalking && !isfalling)
            {
                w_index = w_index + 1;
                w_index %= 4;
                ResourceManager::LoadTexture(walk_sprites[w_index], true, "player");
            }
            c_index = c_index + 1;
            c_index %= 4;
            ResourceManager::LoadTexture(coin_sprites[c_index], true, "coin");
        }

        // std::cout << "c_index : "
        //           << " " << c_index << std::endl;

        Player->Sprite = ResourceManager::GetTexture("player");
        // this->Levels[Level].Zappers.front().else if (this->State == GAME_WIN)
        // {
        //     if (this->Keys[GLFW_KEY_X])
        //     {
        //         exit(0);
        //     }
        // }Sprite = ResourceManager::GetTexture("zapper");
        for (int i = 0; i < this->Levels[this->Level].Zappers.size(); i++)
        {
            this->Levels[this->Level].Zappers[i].Sprite = ResourceManager::GetTexture("zapper");
        }

        for (int i = 0; i < Coins.size(); i++)
        {
            Coins[i].Sprite = ResourceManager::GetTexture("coin");
        }

        if (bg_position <= 800.0f && bg_position + 1.0f <= 800.0f)
        {
            bg_position = bg_position + 1;
        }
        else
        {
            bg_position = 0.0f;
        }

        // std::cout << bg_position << std::endl;
    }

    if (this->distance >= last_x[this->Level])
    {
        if (this->Level != 3)
        {
            this->distance = 0;
            std::cout << this->Level << std::endl;
            this->Level++;
        }
        else
        {
            this->State = GAME_WIN;
            std::cout << "WIN!!!!!!!" << std::endl;
        }
    }

    // Objects that need to come forward
    // Move objects froward level by level
    if (this->State == GAME_ACTIVE)
    {
        for (int i = 0; i < this->Levels[Level].Zappers.size(); i++)
        {
            this->Levels[Level].Zappers[i].Position.x = this->Levels[Level].Zappers[i].Position.x - 1;
            Coins[i].Position.x = Coins[i].Position.x - 1;
        }

        this->distance++;
        this->s_dis++;
    }

    if (this->State == GAME_WIN)
    {
        // ResourceManager::LoadTexture("textures/haha.png", true, "player");

        // Player->Sprite = ResourceManager::GetTexture("player");

        ResourceManager::LoadTexture("textures/gamend.png", true, "menu");
        Menu->Sprite = ResourceManager::GetTexture("menu");
    }

    // if (this->distance + 800 >= last_x[this->Level])
    // {
    //     this->State == GAME_WIN;
    // }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {

        // std::cout << Player->Position.x << " " << Player->Position.y << std::endl;
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_SPACE])
        {
            iswalking = false;
            isfalling = false;
            // if (Player->Position.x >= 0.0f)
            //     Player->Position.x -= velocity;
            if (Player->Position.y >= 50.0f && Player->Position.y - velocity >= 50.0f)
            {
                Player->Position.y -= velocity;
            }
        }
        else
        {
            isfalling = true;
            if (Player->Position.y <= 410.0f && Player->Position.y + velocity <= 410.0f)
            {
                Player->Position.y += velocity;
            }
        }

        if (Player->Position.y >= 410.0f - velocity) // on ground
        {
            iswalking = true;
            isfalling = false;
        }
    }
    else if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
        }
    }
    // else if (this->State == GAME_WIN)
    // {
    //     if (this->Keys[GLFW_KEY_X])
    //     {
    //         exit(0);
    //     }
    // }
}

void Game::Render()
{
    Texture2D myText;

    Texture2D myT1;

    myText = ResourceManager::GetTexture("background");
    Renderer->DrawSprite(myText, glm::vec2(0.0f - bg_position, 0.0f), glm::vec2(800.0f, 600.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer->DrawSprite(myText, glm::vec2(800.0f - bg_position, 0.0f), glm::vec2(800.0f, 600.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    if (this->State == GAME_MENU)
    {
        Menu->Draw(*Renderer);
    }

    Player->Draw(*Renderer);

    unsigned suffix[4];
    suffix[0] = last_x[0];
    for(int i = 1;i < 4;i++)
    {
        suffix[i] = suffix[i-1] + last_x[i] ;
    }

    std::stringstream ss1;
    ss1 << this->s_dis;
    std::stringstream aa;
    aa << suffix[this->Level];
    Text->RenderText("Distance:" + ss1.str() + "/" + aa.str(), 5.0f, 5.0f, 1.0f);
    std::stringstream ss2;
    ss2 << this->coins;
    Text->RenderText("Coins:" + ss2.str(), 220.0f, 5.0f, 1.0f);
    std::stringstream ss3;
    ss3 << this->Level + 1;
    Text->RenderText("Level:" + ss3.str(), 440.0f, 5.0f, 1.0f);

    // draw level
    // if (this->Level == 0)
    // {
    //     this->Levels[0].Draw(*Renderer);
    // }
    // else if(this->Level == 1)
    // {
    //     this->Levels[1].Draw(*Renderer);
    // }
    // else if(this->Level == 2)
    // {
    //     this->Levels[2].Draw(*Renderer);
    // }
    // else if(this->Level == 3)
    // {
    //     this->Levels[3].Draw(*Renderer);
    // }

    this->Levels[this->Level].Draw(*Renderer);

    myT1 = ResourceManager::GetTexture("coin");
    for (int i = 0; i < Coins.size(); i++)
    {
        Coins[i].Draw(*Renderer);
    }
    if (this->State == GAME_LOSE)
    {
        Menu->Draw(*Renderer);
    }

    if (this->State == GAME_WIN)
    {
        Menu->Draw(*Renderer);
    }
}

bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x + 20 &&
                      two.Position.x + two.Size.x >= one.Position.x + 20;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y + 20 &&
                      two.Position.y + two.Size.y >= one.Position.y + 20;
    // collision only if on both axes
    return collisionX && collisionY;
}
void Game::DoCollisions(unsigned flag)
{
    if (!flag)
    {
        for (GameObject &box : this->Levels[this->Level].Zappers)
        {
            if (CheckCollision(*Player, box))
            {
                Player->Dead = true;
            }
        }
    }
    else
    {
        for (GameObject &box : Coins)
        {
            if (CheckCollision(*Player, box))
            {
                if (!box.Destroyed)
                {
                    this->coins++;
                }
                box.Destroyed = true;
            }
        }
    }
}
