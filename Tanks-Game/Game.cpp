#include "Game.h"

Game::Game()
    : window(sf::VideoMode(sf::VideoMode::getDesktopMode()), "Tank Game", sf::State::Windowed)
    , level(nullptr)
    , gameTime()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
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
        window.clear(sf::Color(135, 206, 235)); // Light sky blue background

        // Draw all the things
        level->DrawTo(window);

        window.display();
    }
}