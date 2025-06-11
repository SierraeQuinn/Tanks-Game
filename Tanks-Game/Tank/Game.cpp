#include "Game.h"

Game::Game()
    : window(sf::VideoMode({1920,1080}), "Tank Game", sf::State::Windowed)
    , level(nullptr)
    , gameTime()
{

    level = new LevelScreen({ (float)sf::VideoMode::getDesktopMode().size.x,
        (float)sf::VideoMode::getDesktopMode().size.y });
}

Game::~Game()
{
    delete level;
}

void Game::Run()
{
    while (window.isOpen())
    {
        // Window events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        // Update
        float frameTime = gameTime.restart().asSeconds();
        level->Update(frameTime);


        // Drawing
        window.clear();


        // Draw all the things
        level->DrawTo(window);


        window.display();
    }
}