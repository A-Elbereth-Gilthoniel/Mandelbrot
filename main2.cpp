#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <immintrin.h>
#include <ammintrin.h>

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

const __m256 circle_radius = _mm256_set1_ps(1000.0);
const __m256 scale_change = _mm256_set1_ps(50.0);
const __m256 zero = _mm256_setzero_ps();

int mandel_iter(__m256 x_0, __m256 y_0);

int main()
{
    __m256 scale = _mm256_set1_ps(100.0);
    float dx = 0, dy = 0;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Default, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Default, false));

    int x_center = WINDOW_WIDTH/2 - 200, y_center = WINDOW_HEIGHT/2;

    sf::Font font;
    font.loadFromFile("./new_font.ttf");
    char fps_str[10];
    sf::Text fps_text(" ", font, 50);
    fps_text.setFillColor(sf::Color::Red);
    float fps = 0;
    sf::Clock clock_fps;
    float current_time = 0;

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
                x_center += 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                y_center += 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                y_center -= 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                x_center -= 10;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
                scale = _mm256_add_ps(scale, scale_change);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
            {
                scale = _mm256_sub_ps(scale, scale_change);
                __m256 cmp = _mm256_cmp_ps(scale, zero, _CMP_LT_OS);
                if (_mm256_movemask_ps(cmp))
                    scale = _mm256_setzero_ps();
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::VertexArray pixels(sf::Points, WINDOW_HEIGHT * WINDOW_WIDTH);

        __m256 y_0, x_0;

        for (int y_n = 0; y_n < WINDOW_HEIGHT; y_n++)
        {
            y_0 = _mm256_set1_ps((y_n - y_center));
            y_0 = _mm256_div_ps(y_0, scale);

            for (int x_n = 0; x_n < WINDOW_WIDTH; x_n++)
            {
                x_0 = _mm256_set1_ps((x_n - x_center));
                x_0 = _mm256_div_ps(x_0, scale);

                int n = mandel_iter(x_0, y_0);

                if (n >= 255)
                    pixels[counter].color = sf::Color::Black;
                else
                {
                    pixels[counter].color = sf::Color(255-n, 255-n, (100+n)%256);
                }
                pixels[counter].position = sf::Vector2f((float)x_n, (float)y_n);

                counter++;
            }
         //   y_n = _mm256_add_ps(y_n)
        }

        window.clear(sf::Color::Black);

        window.draw(pixels);

        window.draw(fps_text);
        window.display();

    }

    return 0;
}

//============================================

int mandel_iter(__m256 x_0, __m256 y_0)
{
    __m256 x, y, x2, y2, xy, r2, cmp;
    int it_exit = 0;
    x = x_0;
    y = y_0;
    int n = 0;
    for (n = 0; n <= 255; n++)
    {
        x2 = _mm256_mul_ps(x, x);
        y2 = _mm256_mul_ps(y, y);
        xy = _mm256_mul_ps(x, y);

        r2 = _mm256_add_ps(x2, y2);

        cmp = _mm256_cmp_ps(r2, circle_radius, _CMP_GE_OS);
        it_exit = _mm256_movemask_ps(cmp);

        if (it_exit) return n;

        x = _mm256_sub_ps(x2, y2);
        x = _mm256_add_ps(x, x_0);
        // x = x2 - y2 + x_0;
        y = _mm256_add_ps(xy, xy);
        y = _mm256_add_ps(y, y_0);
        //y = xy + xy + y_0;
    }
    return n;
}

//=================================================
