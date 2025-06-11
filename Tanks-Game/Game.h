#pragma once

// Includes
#include <SFML/Graphics.hpp>
#include "LevelScreen.h"
class Game
{
public:

	// Constructors
	Game();
	~Game();

	// Public functions
	void Run();


	// Data
private:
	sf::RenderWindow window;
	LevelScreen* level;
	sf::Clock gameTime;
};

