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

#include <common/debug/DebugUtil.h>
#include <game/scene/SceneManager.h>
#include <game/effects/Effect.h>


int
main(int argc, char** args)
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Anim Test");
    game::SceneManager sceneMngr;

    game::SceneManager::InitData initdata;
    initdata.renderTarge = &window;
    const sf::Vector2u& rtsize = window.getSize();
    initdata.screenSize = sf::IntRect(0,0,rtsize.x, rtsize.y);
    if (!sceneMngr.init(initdata)) {
        debugERROR("Error initializing the scene manager\n");
        return -1;
    }


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
                    // create an effect
                    const float xrnd = static_cast<float>(std::rand() % 999) / 999.f;
                    const float yrnd = static_cast<float>(std::rand() % 999) / 999.f;

                    sceneMngr.playEffect(game::Effect::EffectType::ET_EXPLOSION,
                                         sf::Vector2f(xrnd, yrnd));
                } else if (event.key.code == sf::Keyboard::Right) {

                }

            }
        }

        // update all the entities
        sceneMngr.update(timeFrame);

        // end the current frame
        window.display();
    }

    return 0;
}
