#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <immintrin.h>
#include <ammintrin.h>

// const:

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

const __m128 MAX_RADIUS     = _mm_set1_ps(100.0);
const __m128 SCALE_CHANGE   = _mm_set1_ps(1.1);
const __m128 ZERO           = _mm_setzero_ps();
const int COORD_CHANGE      = 10;
const __m128 ONE            = _mm_set1_ps(1.0);
const __m128 _3210      = _mm_set_ps (3.f, 2.f, 1.f, 0.f);

//===============================================

union iter_num {
    __m128i reg;
    int array[4];
};

//===============================================

// functions:
void mandel_iter(__m128 x_0, __m128 y_0, union iter_num* array_of_iterations);
sf::Color get_color(int n);
void check_keyboard_events(sf::RenderWindow &window, int* x_center, int* y_center, __m128* scale);
void fill_pixel_array(sf::VertexArray &pixels, int x_center, int y_center, __m128 scale);
void calculate_fps(sf::Text &fps_text, sf::Clock &clock_fps);

// main:

int main()
{
    __m128 scale = _mm_set1_ps(100.0);
    int x_center = WINDOW_WIDTH/2, y_center = WINDOW_HEIGHT/2;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Fullscreen, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Default, false));

    sf::Font font;
    font.loadFromFile("./new_font.ttf");
    sf::Text fps_text(" ", font, 50);
    fps_text.setFillColor(sf::Color::Red);
    float fps = 0;
    sf::Clock clock_fps;
    float current_time = 0;

    sf::VertexArray pixels(sf::Points, WINDOW_HEIGHT * WINDOW_WIDTH);

    while (window.isOpen())
    {
        calculate_fps(fps_text, clock_fps);
        check_keyboard_events(window, &x_center, &y_center, &scale);
        fill_pixel_array(pixels, x_center, y_center, scale);

        window.clear(sf::Color::Black);
        window.draw(pixels);
        window.draw(fps_text);
        window.display();
    }

    return 0;
}

//===============================================

void mandel_iter(__m128 x_0, __m128 y_0, union iter_num* array_of_iterations)
{
    __m128 x, y, x2, y2, xy, r2, cmp;
    int it_exit = 0;
    x = x_0;
    y = y_0;
    int n = 0;

    for (n = 0; n <= 50; n++)
    {
        x2 = _mm_mul_ps(x, x);
        y2 = _mm_mul_ps(y, y);
        xy = _mm_mul_ps(x, y);

        r2 = _mm_add_ps(x2, y2);

        cmp = _mm_cmp_ps(r2, MAX_RADIUS, _CMP_LT_OS);
        it_exit = _mm_movemask_ps(cmp);

        if (!it_exit) break;

        array_of_iterations -> reg = _mm_add_epi32( array_of_iterations -> reg, _mm_cvtps_epi32( _mm_and_ps(cmp, ONE)));

        x = _mm_sub_ps(x2, y2);
        x = _mm_add_ps(x, x_0);
        // x = x2 - y2 + x_0;
        y = _mm_add_ps(xy, xy);
        y = _mm_add_ps(y, y_0);
        //y = xy + xy + y_0;
    }

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

void check_keyboard_events(sf::RenderWindow &window, int* x_center, int* y_center, __m128* scale)
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            *x_center += COORD_CHANGE;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            *y_center += COORD_CHANGE;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            *y_center -= COORD_CHANGE;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            *x_center -= COORD_CHANGE;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
        {
            *scale = _mm_mul_ps(*scale, SCALE_CHANGE);
            *x_center = (*x_center - WINDOW_WIDTH / 2) * 1.1 + WINDOW_WIDTH / 2;
            *y_center = (*y_center - WINDOW_HEIGHT / 2) * 1.1 + WINDOW_HEIGHT / 2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
        {
            *scale = _mm_div_ps(*scale, SCALE_CHANGE);
            *x_center = (*x_center - WINDOW_WIDTH / 2) / 1.1 + WINDOW_WIDTH / 2;
            *y_center = (*y_center - WINDOW_HEIGHT / 2) / 1.1 + WINDOW_HEIGHT / 2;
        }
        if (event.type == sf::Event::Closed)
            window.close();
        }
}

//=============================================================

void fill_pixel_array(sf::VertexArray &pixels, int x_center, int y_center, __m128 scale)
{
    union iter_num nums_of_iterations;
    __m128 y_0 = ZERO, x_0 = ZERO;

    for (int y_n = 0; y_n < WINDOW_HEIGHT; y_n++)
    {
        y_0 = _mm_div_ps(_mm_set1_ps(y_n - y_center), scale);
        for (int x_n = 0; x_n < WINDOW_WIDTH; x_n += 4)
        {
            x_0 = _mm_div_ps(_mm_add_ps(_mm_set1_ps(x_n - x_center), _3210), scale);

            mandel_iter(x_0, y_0, &nums_of_iterations);

            for (int i = 0; i < 4; i++)
            {
                pixels[y_n * WINDOW_WIDTH + x_n + i].color = get_color(nums_of_iterations.array[i]);
                pixels[y_n * WINDOW_WIDTH + x_n + i].position = sf::Vector2f((float)x_n + i, (float)y_n);
            }
            nums_of_iterations.reg = _mm_cvtps_epi32(ZERO);
        }
    }
}

//===================================================================

void calculate_fps(sf::Text &fps_text, sf::Clock &clock_fps)
{
    char fps_str[10];
    float current_time = clock_fps.restart().asSeconds();
    float fps =1.0f / (current_time);
    sprintf(fps_str, "FPS: %0.1f", fps);
    fps_text.setString(std::string(fps_str));
}

//=================================================================
