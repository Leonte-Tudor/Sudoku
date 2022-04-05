#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include "Sudoku.h"
#include "SudokuSolver.h"

const float VIEW_HEIGHT = 600;
const float SUDOKU_HEIGHT = 500;
const float SMALL_LINE_WIDTH = 2;
const float WIDE_LINE_WIDTH = 4;

void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
    view.setCenter(VIEW_HEIGHT/2, VIEW_HEIGHT / 2);
}

void DisplaySudoku(sf::RenderWindow& window, sf::View view, Sudoku sudoku)
{
    int sqs = sudoku.getsqs();
    int size = sqs * sqs;

    window.clear(sf::Color::White);

    sf::RectangleShape field;
    field.setFillColor(sf::Color::Black);
    field.setSize(sf::Vector2f(SUDOKU_HEIGHT, SUDOKU_HEIGHT));
    sf::Vector2f topleftpos = view.getCenter() - field.getSize() / 2.f;
    field.setPosition(topleftpos);

    window.draw(field);

    int nbsmall = (sqs - 1) * sqs;
    int nbwide = sqs + 1;
    float SQUARE_WIDTH = (SUDOKU_HEIGHT - nbsmall * SMALL_LINE_WIDTH - nbwide * WIDE_LINE_WIDTH)/(sqs*sqs);

    sf::RectangleShape square;
    square.setFillColor(sf::Color::White);
    square.setSize(sf::Vector2f(SQUARE_WIDTH, SQUARE_WIDTH));
    float posijx, posijy;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    for (int ci = 0; ci < sqs; ++ci)
    {
        for (int cj = 0; cj < sqs; ++cj)
        {
            for (int i = 0; i < sqs; ++i)
            {
                for (int j = 0; j < sqs; ++j)
                {
                    posijx = ci * (sqs * SQUARE_WIDTH + (sqs - 1) * SMALL_LINE_WIDTH + WIDE_LINE_WIDTH) + i * (SQUARE_WIDTH + SMALL_LINE_WIDTH);
                    posijy = cj * (sqs * SQUARE_WIDTH + (sqs - 1) * SMALL_LINE_WIDTH + WIDE_LINE_WIDTH) + j * (SQUARE_WIDTH + SMALL_LINE_WIDTH);
                    square.setPosition(topleftpos.x + WIDE_LINE_WIDTH + posijx, topleftpos.y + WIDE_LINE_WIDTH + posijy);

                    window.draw(square);

                    if (sudoku.getgrid()[ci * sqs + i][cj * sqs + j] != 0)
                    {

                        sf::Text number;

                        number.setString(std::to_string(sudoku.getgrid()[ci * sqs + i][cj * sqs + j]));
                        number.setFont(font);

                        number.setCharacterSize(40);

                        sf::FloatRect numberRect = number.getLocalBounds();
                        number.setOrigin(numberRect.left + numberRect.width / 2.0f,
                            numberRect.top + numberRect.height / 2.0f);
                        number.setPosition(square.getPosition() + square.getSize() / 2.f);

                        number.setFillColor(sf::Color::Black);

                        window.draw(number);
                    }
                }
            }
        }
    }

    window.setView(view);
    window.display();
}

int main()
{
    int sqs = 3;
    int size = sqs * sqs;

    int dif;
    std::cout << "Select difficulty: " << std::endl
        << "1 - Easy" << std::endl
        << "2 - Medium" << std::endl
        << "3 - Expert" << std::endl;
    std::cin >> dif;
    Sudoku S(sqs); // Here you can choose the size of the game, which will be the square of the input you give
    S.shuffle();
    std::cout << "Shuffled:" << std::endl;
    S.disp();
    S.delnums(dif);
    std::cout << "Sudoku to solve:" << std::endl;
    S.disp();

    sf::RenderWindow window(sf::VideoMode(1900, 900), "Sudoku");
    sf::View view(sf::Vector2f(200.f, VIEW_HEIGHT / 2), sf::Vector2f(300.f, VIEW_HEIGHT));
    ResizeView(window, view);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                ResizeView(window, view);
                break;
            }
        }

        DisplaySudoku(window, view, S);
    }

    return 0;


    bool onesolution = true;       // If true, displays only one solution. If false, displays every solutions (do not do that if grid has less than ~15 no-zeros elements, or it might take a while!)
    bool dispsteps = false;         // Display all the steps of the calculations. If you are in multi-solutions mode, just use this function if you have a very small number of solutions, otherwise it could be extremely long!!
    bool dispallsol = false;        // Display every solutions, only for multi-solutions mode. Can be extremely long if your sudoku grid has many zeros!!

    SudokuSolver sudokusolver(S.getgrid(), dispsteps, S.getsqs());
    const clock_t begin_time = clock();
    sudokusolver.CalculateSolution(-1, -1, 0);
    std::cout << "Time : " << float(clock() - begin_time) / CLOCKS_PER_SEC << " s";
    std::cout << std::endl << std::endl;
    std::cout << "Solution : " << std::endl << std::endl;
    sudokusolver.DisplayGrid();
    std::cout << std::endl << std::endl << std::endl;

    return 0;
}