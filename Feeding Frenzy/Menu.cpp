#include "Menu.h".

//Constructor
Menu::Menu(int numOfLines)
{
	font.loadFromFile("resources/fonts/scrubland_regular.ttf");
	
	this->numOfLines = numOfLines;
	lines = new Text[numOfLines];

	lines[0].setFont(font);
	lines[0].setFillColor(Color(157, 183, 255));
	lines[0].setOutlineColor(Color::Black);
	lines[0].setOutlineThickness(2.f);

	for (int i = 1; i < numOfLines; i++) 
	{
		lines[i].setFont(font);
		lines[i].setFillColor(Color(194, 232, 255));
		lines[i].setOutlineColor(Color::Black);
		lines[i].setOutlineThickness(1);
	}
}

//Getters
int Menu::getSelectedItem()
{ 
	return selectedItem;
}

//Methods
void Menu::moveUp()
{
	lines[selectedItem].setFillColor(Color(194, 232, 255));
	lines[selectedItem].setOutlineThickness(1);

	selectedItem--;
	if (selectedItem == -1)
		selectedItem = numOfLines - 1;

	lines[selectedItem].setFillColor(Color(157, 183, 255));
	lines[selectedItem].setOutlineThickness(2.f);
}
void Menu::moveDown()
{
	lines[selectedItem].setFillColor(Color(194, 232, 255));
	lines[selectedItem].setOutlineThickness(1);

	selectedItem++;
	if (selectedItem == numOfLines)
		selectedItem = 0;

	lines[selectedItem].setFillColor(Color(157, 183, 255));
	lines[selectedItem].setOutlineThickness(2.f);
}
void Menu::draw(RenderWindow& window)
{
	for (int i = 0; i < numOfLines; i++)
		window.draw(lines[i]);
}

//Deconstructora
Menu::~Menu(void) {
	delete[] lines;
	lines = nullptr;
}