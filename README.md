# Feeding Frenzy

Feeding Frenzy is a single-player mode and arcade-style aquatic video game written by Sprout Games, and published by PopCap Games on February 11, 2004. In Feeding Frenzy, players control a hungry marine predator intent on munching as many other fish as possible. The player takes on the role of different aquatic species each trying to move up the food chain.

In Q2 of 2023, The goal of this project was to apply the concepts of Structured Programming by cloning the game using the [SFML Library](https://github.com/SFML/SFML).

In Q3 of 2024, the goal was expanded to include the concepts of Object-Oriented Programming and Data Structures.

## Features

- **Gameplay:**
  - Framerate independent movement & animation
  - Entities:
    - Player & enemy collision detection
    - Screen border collision detection
    - Non-linear movement (every entity) in any direction (enemies only)
    - Smart Enemy AI (avoids player if eatable)
    - Upto 5 different entities (excluding player) in a level
  - Tracking of current & highest scores individually for every level
  - Storing highest scores after exiting
  - Pausing during a level
  - Levels variety
    - 4 Different levels
    - 3 Different visuals 
    - 2 Different gamemodes
	
- **Menus:**
  - Main menu:
    - Proceeds to Level Selection
    - Display Gameplay Instructions
    - Display Credits
    - Exit
  - Level selection:
    - Dynamic background and entities (depends on the selected level)
    - Proceeds player to the selected level (any of the 4 levels) or returns to main menu
  - Level endscreen:
    - Displays current and highest score for the finished level
    - Proceeds player to the next level or returns to the main menu
  - Game endscreen:
    - Displays current and highest scores for all levels
    - Returns player to the main menu
  - Death screen:
    - Returns player to the main menu

## Installation (for developers)

1. Clone the repository
2. Download [`SFML 2.5.1`](https://github.com/SFML/SFML/releases/tag/2.5.1) & install it in the repository's directory
3. Open `Feeding Frenzy.sln` with Visual Studio IDE

## Installation (for players)

1. Download & extract `Feeding.Frenzy.rar` from the [`Latest Release`](https://github.com/Devil11Assassin/SFML-Feeding-Frenzy/releases/tag/v1.0.0).
2. Double-click `Feeding Frenzy.exe` & enjoy playing!

## Development

- Developed by @Devil11Assassin, ___ and ___
- Built using [`SFML 2.5.1`](https://github.com/SFML/SFML/releases/tag/2.5.1)
- Structured Programming Project (2nd Semester - college)
- Updated with OOP & Data Structures (4th Semester - independent)
- Faculty of Computer & Information Science | Ain Shams University
