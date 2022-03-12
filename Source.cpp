#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

class tile {
public:
	int x;
	int y;
	sf::Sprite sprite;
	int numberNear;
	bool flagged = false;
};

vector<int> getMouseGridPos(int mouseX, int mouseY, int rows, int cols, int tileSize, int widthOffset, int heightOffset) {
	vector<int> pos = { -1, -1 };
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (mouseX > i * tileSize + widthOffset && mouseX < (i + 1) * tileSize + widthOffset
				&& mouseY > j * tileSize + heightOffset && mouseY < (j + 1) * tileSize + heightOffset) {
				pos = { i, j };
				break;
			}
		}
	}
	return pos;
}

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

	sf::Texture s_square;
	if (!s_square.loadFromFile("sprites/square.png"))
	{
		cout << "Failed to load texture 'square'." << endl;
	}
	sf::Texture s_bomb;
	if (!s_bomb.loadFromFile("sprites/bomb.png"))
	{
		cout << "Failed to load texture 'bomb'." << endl;
	}
	sf::Texture s_flag;
	if (!s_flag.loadFromFile("sprites/flag.png"))
	{
		cout << "Failed to load texture 'flag'." << endl;
	}
	sf::Texture s_1;
	if (!s_1.loadFromFile("sprites/1.png"))
	{
		cout << "Failed to load texture '1'." << endl;
	}
	sf::Texture s_2;
	if (!s_2.loadFromFile("sprites/2.png"))
	{
		cout << "Failed to load texture '2'." << endl;
	}
	sf::Texture s_3;
	if (!s_3.loadFromFile("sprites/3.png"))
	{
		cout << "Failed to load texture '3'." << endl;
	}
	sf::Texture s_4;
	if (!s_4.loadFromFile("sprites/4.png"))
	{
		cout << "Failed to load texture '4'." << endl;
	}
	sf::Texture s_5;
	if (!s_5.loadFromFile("sprites/5.png"))
	{
		cout << "Failed to load texture '5'." << endl;
	}
	sf::Texture s_6;
	if (!s_6.loadFromFile("sprites/6.png"))
	{
		cout << "Failed to load texture '6'." << endl;
	}
	sf::Texture s_7;
	if (!s_7.loadFromFile("sprites/7.png"))
	{
		cout << "Failed to load texture '7'." << endl;
	}
	sf::Texture s_8;
	if (!s_8.loadFromFile("sprites/8.png"))
	{
		cout << "Failed to load texture '8'." << endl;
	}


	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			grid[i][j].x = i;
			grid[i][j].y = j;

			sf::Sprite sprite;
			sprite.setPosition(sf::Vector2f(i * tileSize + widthOffset, j * tileSize + heightOffset));
			switch ((rand() % 10))
			{
			case 1:
				sprite.setTexture(s_1);
				break;
			case 2:
				sprite.setTexture(s_2);
				break;
			case 3:
				sprite.setTexture(s_3);
				break;
			case 4:
				sprite.setTexture(s_4);
				break;
			case 5:
				sprite.setTexture(s_5);
				break;
			case 6:
				sprite.setTexture(s_6);
				break;
			case 7:
				sprite.setTexture(s_7);
				break;
			case 8:
				sprite.setTexture(s_8);
				break;
			case 9:
				sprite.setTexture(s_bomb);
				break;
			default:
				sprite.setTexture(s_square);
				break;
			}
			grid[i][j].sprite = sprite;

			grid[i][j].numberNear = 1;
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
					/*cout << "the left button was pressed" << endl;
					cout << "mouse x: " << event.mouseButton.x << endl;
					cout << "mouse y: " << event.mouseButton.y << endl;*/
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					vector<int> pos = getMouseGridPos(mouseX, mouseY, rows, cols, tileSize, widthOffset, heightOffset);
					if (pos != vector<int>{-1, -1})
					{
						if (!grid[pos[0]][pos[1]].flagged)
						{
							// !!! can't flag a revealed square
							grid[pos[0]][pos[1]].sprite.setTexture(s_flag);
							grid[pos[0]][pos[1]].flagged = true;
						}
						else {
							// !!! reset to numberNear sprite
							grid[pos[0]][pos[1]].sprite.setTexture(s_square);
							grid[pos[0]][pos[1]].flagged = false;
						}
					}
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

		vector<int> pos = getMouseGridPos(mouseX, mouseY, rows, cols, tileSize, widthOffset, heightOffset);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				grid[i][j].sprite.setColor(sf::Color(255, 255, 255));
				if (pos != vector<int>{-1, -1})
				{
					grid[pos[0]][pos[1]].sprite.setColor(sf::Color(222, 222, 222));
				}
				window.draw(grid[i][j].sprite);
			}
		}

		window.display();
	}

	return 0;
}