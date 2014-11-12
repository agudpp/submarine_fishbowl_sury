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
#include <game/player/Submarine.h>
#include <game/enemies/FishEnemyUnit.h>


int
main(int argc, char** args)
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Anim Test");
    game::SceneManager sceneMngr;
    game::Submarine submarine;

    game::SceneManager::InitData initdata;
    initdata.renderTarge = &window;
    const sf::Vector2u& rtsize = window.getSize();
    initdata.screenSize = sf::IntRect(0,0,rtsize.x, rtsize.y);
    if (!sceneMngr.init(initdata)) {
        debugERROR("Error initializing the scene manager\n");
        return -1;
    }

    // add the player
    submarine.showDebugBB(true);
    sceneMngr.addExternalSceneObject(&submarine);
    sceneMngr.setPlayer(&submarine);

    game::FishEnemyUnit* fish = 0;
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
                    /*const float xrnd = static_cast<float>(std::rand() % 999) / 999.f;
                    const float yrnd = static_cast<float>(std::rand() % 999) / 999.f;

                    const game::SceneObject* explosion = sceneMngr.playEffect(game::Effect::EffectType::ET_EXPLOSION,
                                         sf::Vector2f(xrnd, yrnd));
                    sceneMngr.playEffect(game::Effect::EffectType::ET_3SEC_COUNTER,
                                         sf::Vector2f(0, 0),
                                         explosion);
                                         */
                } else if (event.key.code == sf::Keyboard::Num3) {
                    const float xrnd = static_cast<float>(std::rand() % 999)
                        / 999.f;
                    const float yrnd = static_cast<float>(std::rand() % 999)
                        / 999.f;
                    sceneMngr.playEffect(game::Effect::EffectType::ET_3SEC_COUNTER,
                                         sf::Vector2f(xrnd, yrnd),
                                         0);
                    debug("Creating effect\n");
                } else if (event.key.code == sf::Keyboard::Num1) {
                    fish = (game::FishEnemyUnit*) sceneMngr.createEnemy(game::EnemyUnit::EnemyType::FISH);
                } else if (event.key.code == sf::Keyboard::D) {
                    if (fish != 0) {
                        fish->setLife(-1);
                    }
                } else if (event.key.code == sf::Keyboard::Num2) {
                    const float xrnd = static_cast<float>(std::rand() % 999) / 999.f;
                    const float yrnd = static_cast<float>(std::rand() % 999) / 999.f;
                    const sf::Vector2f pos(xrnd, yrnd);
                    sceneMngr.createElement(game::ElementObject::Type::EOT_MINE, pos);
                }

            }

            if (event.type == sf::Event::KeyPressed) {
                static const float velFactor = 10.1f;
                sf::Vector2f resultVec(0,0);
                switch (event.key.code) {
                case sf::Keyboard::Left: resultVec.x -= velFactor * timeFrame; break;
                case sf::Keyboard::Right: resultVec.x += velFactor * timeFrame; break;
                case sf::Keyboard::Up: resultVec.y -= velFactor * timeFrame; break;
                case sf::Keyboard::Down: resultVec.y += velFactor * timeFrame; break;
                }
                if (resultVec.x != 0.f || resultVec.y != 0.f) {
                    submarine.setPosition(submarine.position() + resultVec);
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
