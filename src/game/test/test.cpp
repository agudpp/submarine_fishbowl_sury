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
              << "\t./sfmltest <textureFile> <textureFile1> [<textureFile2>] [<textureFile3>] ...\n\n"
              << "Donde\n"
              << "\t<textureFile> es el nombre de la textura asociada a la animacion\n"
              << "\t<animFile1> es el nombre del archivo de la animacion 1 (.txt)\n"
              << "\t<animFile2> es el nombre del archivo de la animacion 2 si existe (.txt)\n"
              << "\t\t y asi sucesivamente para cargar todas las animaciones que querramos";
}


// @brief Load the entity
//
static bool
loadGameEntity(const std::string& texture,
               const std::vector<std::string>& animFiles,
               game::GameEntity& ge)
{
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
    std::vector<std::string> animFiles;
    const std::string textFile = args[1];
    for (int i = 2; i < argc; ++i) {
        animFiles.push_back(args[i]);
    }

    game::GameEntity gameEntity;
    if (!loadGameEntity(textFile, animFiles, gameEntity)) {
        std::cerr << "Error loading the game entity, some file is wrong? " <<
                     "\n" << textFile << "\n";
        return -1;
    }

    // get the animations to play
    const game::AnimHandler& animHandler = gameEntity.animHandler();
    std::vector<game::Animation> animations;
    int currentAnim = 0;
    animHandler.getAnimations(animations);
    debug("We have %d animations: \n", animations.size());
    for (unsigned int i = 0; i < animations.size(); ++i) {
        debug("Anim[%d]: %s\n", i, animations[i].name().c_str());
    }
    if (animations.empty() || animations.front().frames().empty()) {
        std::cerr << "No animations were loaded??\n";
        return false;
    }

    gameEntity.changeAnimation(animations[currentAnim].name());
    gameEntity.setAnimLoop(true);
    gameEntity.sprite().setPosition(100, 100);

    // now we will show the sprite in the middle position and also show the
    // sprite with the animation name
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        debugERROR("The file font \"font.ttf\" is not in the same folder where the"
                   " app is... No text will be shown\n");
    }
    sf::Text animText;
    animText.setFont(font);
    animText.setColor(sf::Color::White);
    animText.setPosition(0,0);
    animText.setCharacterSize(30);
    animText.setString(animations[currentAnim].name());


    sf::Clock clock;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        const float timeFrame = clock.restart().asSeconds();

        // check all the window's events that were triggered since the last
        // iteration of the loop
        sf::Event event;
        bool changeAnim = false;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    currentAnim -= 1;
                    if (currentAnim < 0) {
                        currentAnim = animations.size() - 1;
                    }
                    changeAnim = true;
                } else if (event.key.code == sf::Keyboard::Right) {
                    currentAnim += 1;
                    if (currentAnim >= animations.size()) {
                        currentAnim = 0;
                    }
                    changeAnim = true;
                }
            }
        }



        if (changeAnim) {
            const std::string& name = animations[currentAnim].name();
            if (!gameEntity.changeAnimation(name)) {
                debugERROR("Error trying to change the animation to %s\n", name.c_str());
            } else {
                gameEntity.setAnimLoop(true);
                animText.setString(name);
            }
        }

        // update all the entities
        gameEntity.update(timeFrame);

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
