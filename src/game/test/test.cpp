/*
 * test.cpp
 *
 *  Created on: Sep 21, 2014
 *      Author: agustin
 *
 * compile: g++ -o test test.cpp -lsfml-graphics -lsfml-window -lsfml-system
 */

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include <game/AnimHandler.h>
#include <game/Animation.h>
#include <game/GameEntity.h>
#include <common/debug/DebugUtil.h>



// @brief show help
//
static void
showHelp(void)
{
    std::cout << "Para usar la app tenes que ejecutarla como sigue desde la "
              << "linea de comando: \n\n"
              << "\t./sfmltest <animFile> <textureFile>\n\n"
              << "Donde\n"
              << "\t<animFile> es el nombre del archivo de la animacion (.txt)\n"
              << "\t<textureFile> es el nombre de la textura asociada a la animacion\n";
}


// @brief Load the entity
//
static bool
loadGameEntity(const std::string& texture,
               const std::string& animFile,
               game::GameEntity& ge)
{
    std::vector<std::string> animFiles;
    animFiles.push_back(animFile);
    if (!ge.loadSpriteFromFile(texture) || !ge.loadAnimsFromFiles(animFiles)) {
        return false;
    }

    return true;
}


int
main(int argc, char** args)
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Anim Test");

    // check args
    if (argc < 3) {
        showHelp();
        return -1;
    }

    // get the files now to be used
    const std::string animFile = args[1];
    const std::string textFile = args[2];
    game::GameEntity gameEntity;

    if (!loadGameEntity(textFile, animFile, gameEntity)) {
        std::cerr << "Error loading the game entity, some file is wrong? " <<
                     "\n" << animFile << "\n" << textFile << "\n";
        return -1;
    }

    // get the animations to play
    const game::AnimHandler& animHandler = gameEntity.animHandler();
    std::vector<const game::Animation*> animations;
    int currentAnim = 0;
    animHandler.getAnimations(animations);
    if (animations.empty() || animations.front()->frames().empty()) {
        std::cerr << "No animations were loaded??\n";
        return false;
    }

    gameEntity.changeAnimation(animations[currentAnim]->name());

    // now we will show the sprite in the middle position and also show the
    // sprite with the animation name
    sf::Text animText;
    animText.setPosition(0,0);
    animText.setString(animations[currentAnim]->name());


    sf::Clock clock;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        const float timeFrame = clock.getElapsedTime().asMilliseconds();
        clock.restart();

        // check all the window's events that were triggered since the last
        // iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // check keys
        bool changeAnim = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            currentAnim -= 1;
            if (currentAnim < 0) {
                currentAnim = animations.size() - 1;
            }
            changeAnim = true;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            currentAnim += 1;
            if (currentAnim >= animations.size()) {
                currentAnim = 0;
            }
            changeAnim = true;
        }
        if (changeAnim) {
            const std::string& name = animations[currentAnim]->name();
            gameEntity.changeAnimation(name);
            animText.setString(name);
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.draw(gameEntity.sprite());
        window.draw(animText);

        // end the current frame
        window.display();
    }

    return 0;
}
