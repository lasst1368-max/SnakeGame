#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>

const int CELL_SIZE = 20;
const int WIDTH = 800;
const int HEIGHT = 600;

int grow = 0;

enum direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
};

sf::VertexArray grid(sf::Lines);

int main() {
    bool gameOver = false;
        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake Game");

        sf::SoundBuffer eatBuffer;
        if (!eatBuffer.loadFromFile("eat-323883.ogg")) { /* handle error */ }

        sf::Sound eatSound;
        eatSound.setBuffer(eatBuffer);

        sf::Music bgm;
        if (!bgm.openFromFile("gaming-game-minecraft-background-music-372242.ogg")) { /* handle error */ }
        bgm.setLoop(true);
        bgm.play();

        std::vector<SnakeSegment> snake = {{10, 10}};
        direction dir = RIGHT;
        SnakeSegment fruit= {15, 10};

        sf::Clock clock;
        float moveDelay = 0.10f - snake.size() * 0.005f;
        if (moveDelay < 0.05f) moveDelay = 0.05f;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir != DOWN ) dir = UP;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir != UP ) dir = DOWN;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir != RIGHT ) dir = LEFT;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir != RIGHT ) dir = RIGHT;
            if (clock.getElapsedTime().asSeconds() >= moveDelay) {
                clock.restart();

                for (int i = snake.size() - 1; i > 0; --i)
                    snake[i] = snake[i - 1];

                switch (dir) {
                    case UP: snake[0].y -= 1; break;
                    case DOWN: snake[0].y += 1; break;
                    case LEFT: snake[0].x -= 1; break;
                    case RIGHT: snake[0].x += 1; break;
                }
                SnakeSegment oldTail = snake.back();

                if (snake[0].x < 0 || snake[0].x >= WIDTH / CELL_SIZE ||
        snake[0].y < 0 || snake[0].y >= HEIGHT / CELL_SIZE) {
                    gameOver = true;
        }


                for (int i = 1; i < snake.size(); ++i) {
                    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
                        gameOver = true;
                }

                if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                    grow += 1;
                    fruit.x = rand() % (WIDTH / CELL_SIZE);
                    fruit.y = rand() % (HEIGHT / CELL_SIZE);

                    eatSound.play();
                }

                if (grow > 0) {
                    snake.push_back(oldTail);
                    grow--;
                }
            }

            if (!gameOver) {
                window.clear(sf::Color::Black);


                    for (int x = 0; x <= WIDTH; x += CELL_SIZE) {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(x, 0), sf::Color::Blue),
                            sf::Vertex(sf::Vector2f(x, HEIGHT), sf::Color::Blue)};
                        window.draw(line, 2, sf::Lines);

                    }

                    for (int y = 0; y <= HEIGHT; y += CELL_SIZE) {
                        sf::Vertex line[] = {
                            sf::Vertex(sf::Vector2f(0, y), sf::Color::Blue),
                                sf::Vertex(sf::Vector2f(WIDTH, y), sf::Color::Blue)};
                        window.draw(line, 2, sf::Lines);
                    }

                    sf::RectangleShape block(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                    block.setFillColor(sf::Color::Green);
                    for (auto &seg : snake) {
                        block.setPosition(seg.x * CELL_SIZE, seg.y * CELL_SIZE);
                        window.draw(block);
                    }
                    block.setFillColor(sf::Color::Red);
                    block.setPosition(fruit.x * CELL_SIZE, fruit.y * CELL_SIZE);
                    window.draw(block);
                }
            else {
                    sf::RectangleShape overlay(sf::Vector2f(WIDTH, HEIGHT));
                    overlay.setFillColor(sf::Color(0, 0, 0, 150));
                    sf::Font font;
                    if (!font.loadFromFile("ARIAL.TTF")) {
                        std::cerr << "Failed to load font!" << std::endl;
                        return -1;
                    }
                    sf::Text gameOverText;
                    gameOverText.setFont(font);
                    gameOverText.setString("GAME OVER");
                    gameOverText.setCharacterSize(50);
                    gameOverText.setFillColor(sf::Color::Red);

                    sf::Text restartText;
                    restartText.setFont(font);
                    restartText.setString("Press R to Restart");
                    restartText.setCharacterSize(40);
                    restartText.setFillColor(sf::Color::Green);




                    sf::FloatRect textRect = gameOverText.getLocalBounds();
                    gameOverText.setOrigin(textRect.left + textRect.width/2.0f,
                                           textRect.top  + textRect.height/2.0f);
                    gameOverText.setPosition(WIDTH/2.0f, HEIGHT/2.0f);

                    sf::FloatRect restartRect = restartText.getLocalBounds();
                    restartText.setOrigin(restartRect.left + restartRect.width/2.0f,
                                       restartRect.top  + restartRect.height/1.5f);
                    restartText.setPosition(WIDTH/2.0f, HEIGHT/1.5f);

                    window.draw(overlay);
                    window.draw(gameOverText);
                    window.draw(restartText);
                }
            if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                snake.clear();
                snake.push_back({10, 10});
                dir = RIGHT;
                fruit = {15, 10};
                grow = 0;
                clock.restart();
                gameOver = false;
            }
            window.display();
        }
        return 0;

    }


