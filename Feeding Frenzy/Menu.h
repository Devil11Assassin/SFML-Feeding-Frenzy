#pragma once
#include "sfml/Graphics.hpp"
using namespace sf;

class Menu
{
protected:
	int selectedItem = 0;

public:
	Font font;
	int numOfLines;
	Text* lines;

	//Constructor
	Menu(int numOfLines);

	//Getters
	int getSelectedItem();
	
	//Methods
	void moveUp();
	void moveDown();
	void draw(RenderWindow& window);

	//Deconstructor
	~Menu(void);
};