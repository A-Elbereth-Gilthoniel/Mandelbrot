#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <time.h>


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

int main()
{
    float scale = 100;
    float dx = 0, dy = 0;
    float circle_radius = 1000;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Fullscreen, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Fullscreen, false));

    int x_center = WINDOW_WIDTH/2 - 200, y_center = WINDOW_HEIGHT/2;

    sf::Font font;
    font.loadFromFile("./new_font.ttf");
    char fps_str[10];
    sf::Text fps_text(" ", font, 50);
    fps_text.setFillColor(sf::Color::Red);
    float fps = 0;
    sf::Clock clock_fps;

    while (window.isOpen())
    {
        float current_time = clock_fps.restart().asSeconds();
        fps =1.0f / (current_time);
        int counter = 0;
        sprintf(fps_str, "%0.1f", fps);
        fps_text.setString(std::string(fps_str));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                x_center -= 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                y_center -= 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                y_center += 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                x_center += 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
            {
                scale += 50;

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
            {
                scale -= 50;
                if (scale < 0)
                    scale = 0;
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::VertexArray pixels(sf::Points, WINDOW_HEIGHT * WINDOW_WIDTH);

        float y_0, x_0, x, y, x2, y2, xy, r2;

        clock_t begin = clock();

        for (int y_n = 0; y_n < WINDOW_HEIGHT; y_n++)
        {
            y_0 = (float)(y_n - y_center)/(scale+0.01);

            for (int x_n = 0; x_n < WINDOW_WIDTH; x_n++)
            {
                x_0 = (float)(x_n - x_center)/(scale+0.01);
                x = x_0;
                y = y_0;
                int n = 0;
                for (n = 0; n < 256; n++)
                {
                    x2 = x * x, y2 = y * y, xy = x*y;
                    r2 = x2+y2;

                    if (r2 >= circle_radius) break;

                    x = x2 - y2 + x_0;
                    y = xy + xy + y_0;
                }
                if (n >= 255)
                    pixels[counter].color = sf::Color::Black;
                else
                {
                    pixels[counter].color = sf::Color(255-n, 255-n, (100+n)%256);
                }
                pixels[counter].position = sf::Vector2f((float)x_n, (float)y_n);

                counter++;
            }
        }

        clock_t end = clock();
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                printf("The elapsed time is %lf seconds\n", time_spent);


        window.clear(sf::Color::Black);

        window.draw(pixels);

        window.draw(fps_text);
        window.display();
    }

    return 0;
}
