#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <cstring>

// const:

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

const int COORD_CHANGE      = 10;
const float SCALE_CHANGE    = 1.1;
const float circle_radius   = 1000;

//===============================================

struct iter_num {
    int array[4];
};

//===============================================

// functions:

sf::Color get_color(int n);
void mandel_iter(float* x_0_arr, float* y_0_arr, struct iter_num* iterations);
void calculate_fps(sf::Text &fps_text, sf::Clock &clock_fps);
void check_keyboard_events(sf::RenderWindow &window, int* x_center, int* y_center, float* scale);
void fill_pixel_array(sf::VertexArray &pixels, int x_center, int y_center, float scale);


//=============================================

int main()
{
    float scale = 100;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mandelbrot", sf::Style::Fullscreen, \
                                            sf::ContextSettings(0, 0, 0, 1, 1, sf::Style::Default, false));

    int x_center = WINDOW_WIDTH/2 - 200, y_center = WINDOW_HEIGHT/2;


    sf::Font font;
    font.loadFromFile("./new_font.ttf");
    char fps_str[10];
    float current_time = 0;
    sf::Text fps_text(" ", font, 50);
    fps_text.setFillColor(sf::Color::Red);
    float fps = 0;
    sf::Clock clock_fps;

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


//====================================================

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

void mandel_iter(float* x_0_arr, float* y_0_arr, struct iter_num* iterations)
{
    int cmp[4] = {0,0,0,0};
    float x2[4] = {0,0,0,0};
    float y2[4] = {0,0,0,0};
    float xy[4] = {0,0,0,0};
    float x_arr[4] = {x_0_arr[0],x_0_arr[1],x_0_arr[2],x_0_arr[3]}, y_arr[4] = {y_0_arr[0],y_0_arr[0],y_0_arr[0],y_0_arr[0]};
    int n = 0;
    for (n = 0; n <= 50; n++)
    {
        for (int i = 0; i < 4; i++)
        {
            x2[i] = x_arr[i] * x_arr[i];
            y2[i] = y_arr[i] * y_arr[i];
            xy[i] = x_arr[i] * y_arr[i];
         }

        for (int i = 0; i < 4; i++)  cmp[i] = (x2[i] + y2[i] < circle_radius) ? 1: 0;

        int mask = 0;
        for (int i = 0; i < 4; i++) mask |= (cmp[i] << i);
        if (!mask) break;

        for (int i = 0; i < 4; i++)
        { iterations->array[i] += cmp[i];}

        for (int i = 0; i < 4; i++)
        {
            x_arr[i] = x2[i] - y2[i] + x_0_arr[i];
            y_arr[i] = xy[i] + xy[i] + y_0_arr[i];
        }
    }
}

//===================================================

void calculate_fps(sf::Text &fps_text, sf::Clock &clock_fps)
{
    char fps_str[10];
    float current_time = clock_fps.restart().asSeconds();
    float fps =1.0f / (current_time);
    sprintf(fps_str, "FPS: %0.1f", fps);
    fps_text.setString(std::string(fps_str));
}

//===================================================

void check_keyboard_events(sf::RenderWindow &window, int* x_center, int* y_center, float* scale)
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
            *scale *= SCALE_CHANGE;
            *x_center = (*x_center - WINDOW_WIDTH / 2) * SCALE_CHANGE + WINDOW_WIDTH / 2;
            *y_center = (*y_center - WINDOW_HEIGHT / 2) * SCALE_CHANGE + WINDOW_HEIGHT / 2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
        {
            *scale /= SCALE_CHANGE;
            *x_center = (*x_center - WINDOW_WIDTH / 2) / SCALE_CHANGE + WINDOW_WIDTH / 2;
            *y_center = (*y_center - WINDOW_HEIGHT / 2) / SCALE_CHANGE + WINDOW_HEIGHT / 2;
        }
        if (event.type == sf::Event::Closed)
            window.close();
        }
}

//=============================================================

void fill_pixel_array(sf::VertexArray &pixels, int x_center, int y_center, float scale)
{
    struct iter_num iterations;
    for (int y_n = 0; y_n < WINDOW_HEIGHT; y_n++)
    {

        for (int x_n = 0; x_n < WINDOW_WIDTH; x_n += 4)
        {
            memset(iterations.array, 0, 4 * sizeof(int));
            float y_0_arr[4] = {(float)(y_n - y_center)/scale, (float)(y_n - y_center)/scale, \
                        (float)(y_n - y_center)/scale, (float)(y_n - y_center)/scale};
            float x_0_arr[4] = {(float)(x_n - x_center)/scale, (float)(x_n+1 - x_center)/scale, \
                        (float)(x_n+2 - x_center)/scale, (float)(x_n+3 - x_center)/scale};

            mandel_iter(x_0_arr, y_0_arr, &iterations);

            for (int i = 0; i < 4; i++)
            {
                pixels[y_n * WINDOW_WIDTH + x_n + i].color = get_color(iterations.array[i]);
                pixels[y_n * WINDOW_WIDTH + x_n + i].position = sf::Vector2f((float)x_n + i, (float)y_n);
            }
        }
    }
}

//===============================================================
