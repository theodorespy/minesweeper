#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;

class tile {
public:
    int x;
    int y;
    sf::Sprite sprite;
};

int main()
{
    const int WIDTH = 1280;
    const int HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        cout << "Font loading error." << endl;
    }

    int mouseX = 0;
    int mouseY = 0;

    int tileSize = 32;
    const int rows = 20;
    const int cols = 20;
    int totalWidth = rows * tileSize;
    int totalHeight = cols * tileSize;
    int widthOffset = (WIDTH - totalWidth) / 2;
    int heightOffset = (HEIGHT - totalHeight) / 2;
    tile grid[rows][cols];
    sf::Texture square;
    if (!square.loadFromFile("sprites/square.png"))
    {
        cout << "Failed to load texture." << endl;
    }

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            grid[i][j].x = i;
            grid[i][j].y = j;
            sf::Sprite sprite;
            sprite.setTexture(square);
            sprite.setPosition(sf::Vector2f(i * tileSize + widthOffset, j  * tileSize + heightOffset));
            grid[i][j].sprite = sprite;
            
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    cout << "the left button was pressed" << endl;
                    cout << "mouse x: " << event.mouseButton.x << endl;
                    cout << "mouse y: " << event.mouseButton.y << endl;
                }
            }
            if (event.type == sf::Event::MouseMoved)
            {
                mouseX = event.mouseMove.x;
                mouseY = event.mouseMove.y;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
            
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        sf::Text text;

        text.setFont(font);
        text.setString("Mouse X: " + to_string(mouseX) + " Y: " + to_string(mouseY));
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        window.draw(text);

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                if (mouseX > i * tileSize + widthOffset && mouseX < (i + 1) * tileSize + widthOffset
                    && mouseY > j * tileSize + heightOffset && mouseY < (j + 1) * tileSize + heightOffset) {
                    grid[i][j].sprite.setColor(sf::Color(0, 255, 0));
                }
                else {
                    grid[i][j].sprite.setColor(sf::Color(255, 255, 255));
                }
                window.draw(grid[i][j].sprite);
            }
        }

        window.display();
    }

    return 0;
}