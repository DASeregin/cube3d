#include <SFML/Graphics.hpp>
#include <iostream>
#include "Figure.h"

using namespace std;
float w = 1080, h = 720;

int main()
{
    sf::RenderWindow window(sf::VideoMode(w, h), "SFML Application");
    Figure f("/Users/denis_seregin/CLionProjects/lab_2/cube", w, h);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        f.Draw(window);
        f.Rotate(1.0/10000);
        window.display();
    }
}