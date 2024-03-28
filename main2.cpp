#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <immintrin.h>
#include <ammintrin.h>

// const:

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

const __m256 circle_radius = _mm256_set1_ps(100.0);
const __m256 scale_change = _mm256_set1_ps(50.0);
const __m256 zero = _mm256_setzero_ps();
const __m256 coord_change = _mm256_set1_ps(10.0);
const __m256 one = _mm256_set1_ps(1.0);

// functions:

int mandel_iter(__m256 x_0, __m256 y_0);
sf::Color get_color(int n);

// main:

int main()
{
    __m256 scale = _mm256_set1_ps(100.0);
    __m256 x_center = _mm256_set1_ps(WINDOW_WIDTH/2), y_center = _mm256_set1_ps(WINDOW_HEIGHT/2);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Fullscreen, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Default, false));

    sf::Font font;
    font.loadFromFile("./new_font.ttf");
    char fps_str[10];
    sf::Text fps_text(" ", font, 50);
    fps_text.setFillColor(sf::Color::Red);
    float fps = 0;
    sf::Clock clock_fps;
    float current_time = 0;

    sf::Event event;

    __m256 y_0 = zero, x_0 = zero, cmp_scale = zero;
    __m256 cmp_x_n = zero, cmp_y_n = zero;
    __m256 y_n = zero, x_n = zero;

    sf::VertexArray pixels(sf::Points, WINDOW_HEIGHT * WINDOW_WIDTH);


    while (window.isOpen())
    {
        current_time = clock_fps.restart().asSeconds();
        fps =1.0f / (current_time);
        int counter = 0;
        sprintf(fps_str, "FPS: %0.1f", fps);
        fps_text.setString(std::string(fps_str));

        while (window.pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                x_center = _mm256_add_ps(x_center, coord_change);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                y_center = _mm256_add_ps(y_center, coord_change);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                y_center = _mm256_sub_ps(y_center, coord_change);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                x_center = _mm256_sub_ps(x_center, coord_change);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
                scale = _mm256_add_ps(scale, scale_change);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
            {
                scale = _mm256_sub_ps(scale, scale_change);
                cmp_scale = _mm256_cmp_ps(scale, zero, _CMP_LT_OS);
                if (_mm256_movemask_ps(cmp_scale))
                    scale = _mm256_setzero_ps();
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }

        y_n = zero, x_n = zero;
        cmp_y_n = _mm256_cmp_ps(y_n, _mm256_set1_ps(WINDOW_HEIGHT), _CMP_LT_OS);
        while (_mm256_movemask_ps(cmp_y_n))
        {
            y_0 = _mm256_sub_ps(y_n, y_center);
            y_0 = _mm256_div_ps(y_0, scale);

            cmp_x_n = _mm256_cmp_ps(x_n, _mm256_set1_ps(WINDOW_WIDTH), _CMP_LT_OS);
            while (_mm256_movemask_ps(cmp_x_n))
            {
                x_0 = _mm256_sub_ps(x_n, x_center);
                x_0 = _mm256_div_ps(x_0, scale);

                int n = mandel_iter(x_0, y_0);

                pixels[counter].color = get_color(n);
                pixels[counter].position = sf::Vector2f(_mm256_cvtss_f32(x_n), _mm256_cvtss_f32(y_n));

                counter++;
                x_n = _mm256_add_ps(x_n, one);
                cmp_x_n = _mm256_cmp_ps(x_n, _mm256_set1_ps(WINDOW_WIDTH), _CMP_LT_OS);
            }
            x_n = zero;
            y_n = _mm256_add_ps(y_n, one);
            cmp_y_n = _mm256_cmp_ps(y_n, _mm256_set1_ps(WINDOW_HEIGHT), _CMP_LT_OS);
        }

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.draw(fps_text);
        window.display();
    }

    return 0;
}

//===============================================

int mandel_iter(__m256 x_0, __m256 y_0)
{
    __m256 x, y, x2, y2, xy, r2, cmp;
    int it_exit = 0;
    x = x_0;
    y = y_0;
    int n = 0;
    for (n = 0; n <= 50; n++)
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

sf::Color get_color(int n)
{
    int colwidth = 666, coloffset = 3200;
    float red = 0, blue = 0, green = 0;
    if (n >= 50)
        return sf::Color::Black;
    n *= 25869;
    n=(n+coloffset)%(3*colwidth);
    if (n/colwidth==0)
    {
        blue =116+139*n/colwidth;
        green =11+244*n/colwidth;
        red =237*n/colwidth;
    }
    else if (n/colwidth==1)
    {
        n-=colwidth;
        blue=255-253*n/colwidth;
        green=255-123*n/colwidth;
        red =238;
    }
    else
    {
        n-=2*colwidth;
        blue =2+114*n/colwidth;
        green=132-121*n/colwidth;
        red =239-239*n/colwidth;
    }
    return sf::Color(red, green, blue);

}

//====================================================
