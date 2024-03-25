#include <SFML/Graphics.hpp>



#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Default, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Default, false));

   // window.setPosition(sf::Vector2i(400, 200));
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    size_t x_coord = 200, y_coord = 300;
    shape.move(x_coord, y_coord);

    // приложение будет работать, пока окно открыто
    while (window.isOpen())
    {
        // проверяем все события окна, которые были запущены с предыдущей итерации цикла
        sf::Event event;
        while (window.pollEvent(event))
        {
            // сработало событие закрытия окна - закроем его
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                x_coord -= 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                y_coord -= 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                y_coord += 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                x_coord += 10;
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

        for (int i = 0; i < (WINDOW_HEIGHT * WINDOW_WIDTH); i++)
        {
            lines[i].position = sf::Vector2f(i, i);
        }
        window.draw(lines);
        /*shape.setFillColor(sf::Color::Green);
        sf::CircleShape shape(100.f);
        shape.move(x_coord, y_coord);
        window.draw(shape);*/
        // конец текущего кадра - отображаем
        window.display();
    }

    return 0;
}
