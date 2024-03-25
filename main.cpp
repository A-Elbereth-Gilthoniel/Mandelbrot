#include <SFML/Graphics.hpp>
#include <stdio.h>


#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

int main()
{
    float circle_radius = 200;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Default, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Default, false));

   // window.setPosition(sf::Vector2i(400, 200));

    size_t x_center = WINDOW_WIDTH/2, y_center = WINDOW_HEIGHT/2;

    sf::Font font;
    font.loadFromFile("./new_font.ttf");
    char fps_str[10];
    sf::Text fps_text(" ", font, 50);
    fps_text.setColor(sf::Color::Red);
    float fps = 0;
    sf::Clock clock;
    int colwidth=666;
  int coloffset=3200;
    // приложение будет работать, пока окно открыто
    while (window.isOpen())
    {
        float current_time = clock.restart().asSeconds();
        fps =1.0f / (current_time);

        sprintf(fps_str, "%0.1f", fps);
        fps_text.setString(std::string(fps_str));

        // проверяем все события окна, которые были запущены с предыдущей итерации цикла
        sf::Event event;
        while (window.pollEvent(event))
        {
            // сработало событие закрытия окна - закроем его
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
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // очищаем окно и заливаем черным цветом
        window.clear(sf::Color::Black);

        // рисуем что-нибудь здесь
        // window.draw(...);
        sf::VertexArray pixels(sf::Points, WINDOW_HEIGHT * WINDOW_WIDTH);


        for (int y_n = 0; y_n < WINDOW_HEIGHT; y_n++)
        {
            float y_0 = (y_n - y_center);
            float x_0 = -x_center;

            for (int x_n = 0; x_n < WINDOW_WIDTH; x_n++)
            {
                float x = x_0;
                float y = y_0;
                int n = 0;
                for (n = 0; n < 256; n++)
                {
                    float x2 = x * x, y2 = y * y, xy = x*y;
                    float r2 = x2+y2;

                    if (r2 >= circle_radius) break;

                    x = x2 - y2 + x_0;
                    y = xy * xy + y_0;
                }
          n=(n+coloffset)%(3*colwidth);
          int r, g, b;
          if (n/colwidth==0)
          { // Голубой 116.11.0
            b = 116+139*n/colwidth;//B
            g = 11+244*n/colwidth;//G
            r = 237*n/colwidth;//R
          }
          else if (n/colwidth==1)
          { // Белый 255.255.237
            n-=colwidth;
            b =255-253*n/colwidth;//B
            g =255-123*n/colwidth;//G
            r =238;//R
          }
          else
          { // Рыжий 2.132.239
            n-=2*colwidth;
            b =2+114*n/colwidth;//B
            g =132-121*n/colwidth;//G
            r =239-239*n/colwidth;//R
          }
            pixels[y_n * WINDOW_HEIGHT + x_n].color = {r, g, b};
            pixels[y_n * WINDOW_HEIGHT + x_n].position = sf::Vector2f((float)(y_n * WINDOW_HEIGHT), (float)x_n);
            }
        }

        for (int i = 0; i < (WINDOW_HEIGHT * WINDOW_WIDTH); i++)
        {
            float x_n = (i) % WINDOW_WIDTH;
            float y_n = i / WINDOW_WIDTH;
            float med_x = x_n;
            float med_y = y_n;
            while (((med_x - x_center)*(med_x - x_center) + (med_y - y_center)*(med_y - y_center)) < circle_radius * circle_radius)
            {
                med_x, med_y = med_x * med_x - med_y * med_y , 2 * med_x * med_y ;
            }
            if ((int)(med_x+med_y)% 2 == 0)
                pixels[i].color = {255, 255, 255};
            else
                pixels[i].color = {0, 0, 0};
            pixels[i].position = sf::Vector2f( x_n, y_n);
        }




        window.draw(pixels);

        window.draw(fps_text);
        window.display();
    }

    return 0;
}
