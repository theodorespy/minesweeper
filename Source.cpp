#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <windows.h>

using namespace std;

class tile {
public:
	sf::Sprite sprite;
	int numberNear = 0; // -1 indicates this is a bomb tile
	bool flagged = false;
	bool revealed = false;
};

const int rows = 20;
const int cols = 20;
tile grid[rows][cols];
sf::Texture s_square, s_bomb, s_hit, s_wrong, s_flag, s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7, s_8;

vector<int> getMouseGridPos(int mouseX, int mouseY, int tileSize, int widthOffset, int heightOffset) {
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

bool validCoord(int x, int y)
{
	return (0 <= x && x < rows && 0 <= y && y < cols);
}

void revealTiles(int x, int y, bool bombs) {
	
	if (grid[x][y].flagged) 
	{
		if (bombs && grid[x][y].numberNear != -1)
		{
			grid[x][y].sprite.setTexture(s_wrong);
		}
		return;
	}

	switch (grid[x][y].numberNear) {
	case -1: if (bombs) { grid[x][y].sprite.setTexture(s_bomb); }
		break;
	case 0: grid[x][y].sprite.setTexture(s_0);
		break;
	case 1: grid[x][y].sprite.setTexture(s_1);
		break;
	case 2: grid[x][y].sprite.setTexture(s_2);
		break;
	case 3: grid[x][y].sprite.setTexture(s_3);
		break;
	case 4: grid[x][y].sprite.setTexture(s_4);
		break;
	case 5: grid[x][y].sprite.setTexture(s_5);
		break;
	case 6: grid[x][y].sprite.setTexture(s_6);
		break;
	case 7: grid[x][y].sprite.setTexture(s_7);
		break;
	case 8: grid[x][y].sprite.setTexture(s_8);
		break;
	}

	if (grid[x][y].numberNear == 0 && !grid[x][y].revealed)
	{
		grid[x][y].revealed = true;
		vector<vector<int>> coords = { {x - 1, y - 1}, {x, y - 1}, {x + 1, y - 1}, {x - 1, y}, {x + 1, y}, {x - 1, y + 1}, {x, y + 1}, {x + 1, y + 1} };

		for (auto coord : coords)
		{
			if (validCoord(coord[0], coord[1]))
			{
				revealTiles(coord[0], coord[1], false);
			}
		}
	}
	
	grid[x][y].revealed = true;
}

int main()
{
	// Initialisation
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");

	sf::Font font;
	if (!font.loadFromFile("fonts/arial.ttf"))
	{
		cout << "Font loading error." << endl;
	}

	srand(time(NULL));

	int mouseX = 0;
	int mouseY = 0;

	int tileSize = 32;
	int numBombs = 50;
	int flagsRemaining = numBombs;
	int totalWidth = rows * tileSize;
	int totalHeight = cols * tileSize;
	int widthOffset = (WIDTH - totalWidth) / 2;
	int heightOffset = (HEIGHT - totalHeight) / 2;

	bool gameOver = false;
	bool gridShown = false;

	if (!s_square.loadFromFile("sprites/square.png") || !s_bomb.loadFromFile("sprites/bomb.png") || !s_hit.loadFromFile("sprites/hit.png") 
		|| !s_flag.loadFromFile("sprites/flag.png") || !s_wrong.loadFromFile("sprites/wrong.png") || !s_0.loadFromFile("sprites/0.png") 
		|| !s_1.loadFromFile("sprites/1.png") || !s_2.loadFromFile("sprites/2.png") || !s_3.loadFromFile("sprites/3.png")
		|| !s_4.loadFromFile("sprites/4.png") || !s_5.loadFromFile("sprites/5.png") || !s_6.loadFromFile("sprites/6.png") 
		|| !s_7.loadFromFile("sprites/7.png") || !s_8.loadFromFile("sprites/8.png"))
	{
		cout << "Failed to load texture(s)." << endl;
		MessageBoxA(NULL, "Failed to load texture(s).", "Error!", MB_ICONERROR | MB_OK);
		exit(0);
	}

	// Generate grid structure
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			sf::Sprite sprite;
			sprite.setPosition(sf::Vector2f(i * tileSize + widthOffset, j * tileSize + heightOffset));
			sprite.setTexture(s_square);
			grid[i][j].sprite = sprite;
		}
	}

	// Generate Bombs
	for (int i = 0; i < numBombs; i++)
	{
		while (true)
		{
			int rRow = rand() % rows;
			int rCols = rand() % cols;
			if (grid[rRow][rCols].numberNear != -1)
			{
				grid[rRow][rCols].numberNear = -1;
				break;
			}
		}
	}

	// Calculate each tile's nearby bombs
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j].numberNear == -1) {
				continue;
			}
			int count = 0;
			vector<vector<int>> coords = { {i - 1, j - 1}, {i, j - 1}, {i + 1, j - 1}, {i - 1, j}, {i + 1, j}, {i - 1, j + 1}, {i, j + 1}, {i + 1, j + 1} };

			for (auto coord : coords)
			{
				if (validCoord(coord[0], coord[1]))
				{
					count += (grid[coord[0]][coord[1]].numberNear == -1) ? 1 : 0;
				}
			}
			grid[i][j].numberNear = count;

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
					vector<int> pos = getMouseGridPos(mouseX, mouseY, tileSize, widthOffset, heightOffset);
					if (pos != vector<int>{-1, -1} && !gameOver)
					{
						if (!grid[pos[0]][pos[1]].flagged)
						{
							if (grid[pos[0]][pos[1]].numberNear == -1)
							{
								grid[pos[0]][pos[1]].sprite.setTexture(s_hit);
								window.draw(grid[pos[0]][pos[1]].sprite);
								gameOver = true;
								grid[pos[0]][pos[1]].numberNear = -2; // avoid changing sprites
							}

							if (!grid[pos[0]][pos[1]].revealed)
							{
								revealTiles(pos[0], pos[1], false);
								for (int i = 0; i < rows; i++)
								{
									for (int j = 0; j < cols; j++)
									{
										window.draw(grid[i][j].sprite);
									}
								}
							}
						}
					}
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					vector<int> pos = getMouseGridPos(mouseX, mouseY, tileSize, widthOffset, heightOffset);
					if (pos != vector<int>{-1, -1} && !gameOver)
					{
						if (!grid[pos[0]][pos[1]].revealed)
						{
							if (!grid[pos[0]][pos[1]].flagged && flagsRemaining > 0)
							{
								grid[pos[0]][pos[1]].sprite.setTexture(s_flag);
								grid[pos[0]][pos[1]].flagged = true;
								flagsRemaining--;
							}
							else if (grid[pos[0]][pos[1]].flagged) {
								grid[pos[0]][pos[1]].sprite.setTexture(s_square);
								grid[pos[0]][pos[1]].flagged = false;
								flagsRemaining++;
							}
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
		text.setCharacterSize(16);
		text.setFillColor(sf::Color::White);

		text.setString("Number of bombs: " + to_string(numBombs));
		text.setPosition(sf::Vector2f(0, 0));
		window.draw(text);

		text.setString("Flags remaining: " + to_string(flagsRemaining));
		text.setPosition(sf::Vector2f(0, 20));
		window.draw(text);

		vector<int> pos = getMouseGridPos(mouseX, mouseY, tileSize, widthOffset, heightOffset);

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

		// Win check
		if (flagsRemaining == 0 && !gameOver) {
			int correct = 0;
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					if (!grid[i][j].revealed && !grid[i][j].flagged) {
						correct = -999;
						break;
					}
					if (grid[i][j].flagged && grid[i][j].numberNear == -1)
					{
						correct++;
					}
				}
			}
			if (correct == numBombs) {
				MessageBoxA(NULL, "You won!", "Congratulations!", MB_ICONEXCLAMATION | MB_OK);
				gameOver = true;
				gridShown = true;
			}
		}

		if (gameOver && !gridShown)
		{
			gridShown = true;
			MessageBoxA(NULL, "You hit a bomb!", "Game over!", MB_ICONEXCLAMATION | MB_OK);
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < cols; j++)
				{
					grid[i][j].revealed = true;
					revealTiles(i, j, true);
				}
			}
		}
	}

	return 0;
}