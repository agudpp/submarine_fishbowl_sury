/*
 * test.cpp
 *
 *  Created on: Sep 21, 2014
 *      Author: agustin
 *
 * compile: g++ -o test test.cpp -lsfml-graphics -lsfml-window -lsfml-system
 */


#include <SFML/Graphics.hpp>



int
main(void)
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }

    return 0;
}
