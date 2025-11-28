#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "Map.h"
#include "AStar.h"
#include "GameCharacter.h"
#include "Dijkstra.h"
#include "Utility/Button.h"

void PrintMapConsole(const Map &map);

sf::Font font;

int main() {
    int width = 800;
    int height = 800;
    int menuHeight = 120;
    sf::RenderWindow window(sf::VideoMode(width, height + menuHeight), "A* Pathfinder",sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);


    if (!font.loadFromFile("../Font/OpenSans-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Error loading font\n";
    }

    int mapWidth = 49;
    int mapHeight = 49;
    Map map(mapWidth, mapHeight, width / mapWidth, font);


    int numObstacles;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, mapWidth * mapHeight / 1.5);
    numObstacles = dis(gen);
    //map.generateRandomObstacles(numObstacles);
    float threshold = 0.55f;
    float scale = 0.2f;
    //map.generateObstaclesPerlin(threshold, scale, gen());
    map.generateMaze();

    GameCharacter player(map.getStart(), map.getCellSize());
    sf::Clock clock;
    float moveDelay = 0.1f;

    bool editMode = true;

    int pathLength = 0;
    int numVisitedCells = 0;

    // Variabili per la gestione dei pulsanti. Per evitare che il tasto venga premuto più volte in un frame.
    bool dWasPressed = false;
    bool rWasPressed = false;
    bool eWasPressed = false;
    bool pWasPressed = false;
    bool mWasPressed = false;

    // Inizializzo a true per forzare il calcolo del percorso all'inizio
    bool stateHasChanged = true;

    bool startSetMode = false;
    bool goalSetMode = false;

    bool drawBorder = false;
    bool drawTexture = true;

    bool useAStar = true;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);

    // Pulsanti
    sf::Vector2f buttonSize(15, 15);
    sf::Vector2f startModeButtonPosition(775, height + 4);
    sf::Vector2f goalModeButtonPosition(775, height + 27);
    sf::Vector2f borderButtonPosition(775, height + 50);
    sf::Vector2f showTextureButtonPosition(775, height + 73);
    sf::Vector2f algorithmButtonPosition(775, height + 96);


    // Hover shapes per le celle e i pulsanti
    sf::RectangleShape hoverShapeCell(sf::Vector2f((float)map.getCellSize(), (float)map.getCellSize()));
    hoverShapeCell.setFillColor(sf::Color(0, 0, 0, 50));
    hoverShapeCell.setOutlineColor(sf::Color::Black);
    hoverShapeCell.setOutlineThickness(2);

    sf::RectangleShape hoverShapeButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    hoverShapeButton.setFillColor(sf::Color(0, 0, 0, 50));
    hoverShapeButton.setOutlineColor(sf::Color::Black);
    hoverShapeButton.setOutlineThickness(2);

    std::vector<sf::Vector2i> path;

    std::vector<Button> buttons;
    buttons.emplace_back("start", startModeButtonPosition, buttonSize, startSetMode, sf::Keyboard::Key::S, true);
    buttons.emplace_back("goal", goalModeButtonPosition, buttonSize, goalSetMode, sf::Keyboard::Key::G, true);
    buttons.emplace_back("border", borderButtonPosition, buttonSize, drawBorder, sf::Keyboard::Key::B, true);
    buttons.emplace_back("texture", showTextureButtonPosition, buttonSize, drawTexture, sf::Keyboard::Key::T, true);
    buttons.emplace_back("algorithm", algorithmButtonPosition, buttonSize, useAStar, sf::Keyboard::Key::A, true);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mousePos.x /= (int)map.getCellSize();
        mousePos.y /= (int)map.getCellSize();

        auto mousePosAbsolute = sf::Vector2f(sf::Mouse::getPosition(window));

        if(editMode){
            window.clear(sf::Color::White);

            // Gestione posizionamento delle celle
            // Si ottiene la posizione del mouse e si verifica se è all'interno della mappa
            // Se il tasto destro è premuto si piazza una cella walkable, altrimenti si piazza un ostacolo
            if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    if(map.canPlaceWalkable(mousePos.x, mousePos.y)){
                        map.setCellState(mousePos.x, mousePos.y, CellState::Walkable);
                        stateHasChanged = true;
                        numObstacles--;
                    }
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    // Si posiziona start/goal con il tasto sinistro. Si verifica se si è in modalità, altrimenti si piazza un ostacolo
                    if(startSetMode){
                        if(map.getCellState(mousePos.x, mousePos.y) == CellState::Obstacle){
                            numObstacles--;
                        }
                        map.setCellState(mousePos.x, mousePos.y, CellState::Start);
                        map.setStart(mousePos);
                        stateHasChanged = true;
                    }
                    else if(goalSetMode){
                        if(map.getCellState(mousePos.x, mousePos.y) == CellState::Obstacle){
                            numObstacles--;
                        }
                        map.setCellState(mousePos.x, mousePos.y, CellState::Goal);
                        map.setGoal(mousePos);
                        stateHasChanged = true;
                    }
                    else if(map.canPlaceObstacle(mousePos.x, mousePos.y)){
                        map.setCellState(mousePos.x, mousePos.y, CellState::Obstacle);
                        stateHasChanged = true;
                        numObstacles++;
                    }
                }
            }

            // Gestione dei pulsanti start e goal
            for (auto& button : buttons) {
                if (button.getLabel() == "start") {
                    if (button.checkClick(mousePosAbsolute, sf::Mouse::isButtonPressed(sf::Mouse::Left),
                        sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? sf::Keyboard::S : sf::Keyboard::Unknown)) {
                        std::cout<<"Button " << button.getLabel() << " state: " << (button.getState() ? "true" : "false") << "\n";
                        startSetMode = button.getState();
                        if (startSetMode) {
                            for (auto& otherButton : buttons) {
                                if (otherButton.getLabel() == "goal") {
                                    otherButton.setState(false);
                                    goalSetMode = false;
                                }
                            }
                        }
                    }
                } else if (button.getLabel() == "goal") {
                    if (button.checkClick(mousePosAbsolute, sf::Mouse::isButtonPressed(sf::Mouse::Left),
                        sf::Keyboard::isKeyPressed(sf::Keyboard::G) ? sf::Keyboard::G : sf::Keyboard::Unknown)) {
                        std::cout<<"Button " << button.getLabel() << " state: " << (button.getState() ? "true" : "false") << "\n";
                        goalSetMode = button.getState();
                        if (goalSetMode) {
                            for (auto& otherButton : buttons) {
                                if (otherButton.getLabel() == "start") {
                                    otherButton.setState(false);
                                    startSetMode = false;
                                }
                            }
                        }
                    }
                }
            }

        } else{
            window.clear(sf::Color(150, 150, 150));

            // Gestione movimento del personaggio ogni moveDelay secondi
            if (clock.getElapsedTime().asSeconds() >= moveDelay) {
                if (!path.empty()) {
                    player.update(path.front());
                    if(player.getPosition() != map.getStart()){
                        map.setStart(player.getPosition());
                        stateHasChanged = true;
                    }
                }
                clock.restart();
            }

            // Gestione posizionamento del goal
            if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    if(map.canPlaceObstacle(mousePos.x, mousePos.y)){
                        map.setCellState(mousePos.x, mousePos.y, CellState::Goal);
                        map.setGoal(mousePos);
                        stateHasChanged = true;
                    }
                }
            }
        }


        // Gestione del testo
        text.setString(editMode ? "Press 'Left Click' to set an obstacle\nPress 'Right Click' to set a walkable cell\nPress 'E' to exit edit mode" : "Press 'Left Click' to set a goal point\nPress 'E' to enter edit mode");
        text.setPosition(10, (float)height);
        window.draw(text);

        text.setString("Press 'S' to set start point\nPress 'G' to set goal point\nPress 'B' to toggle border\nPress 'T' to change texture\nPress 'A' to toggle A*/Dijkstra");
        text.setPosition(560, (float)height);
        window.draw(text);

        text.setString("Press 'R' to reset the map\nwith random obstacles\nPress 'P' to use Perlin\nNumber of obstacles: " + std::to_string(numObstacles));
        text.setPosition(330, (float)height);
        window.draw(text);

        text.setStyle(sf::Text::Bold);
        text.setString("Path length: " + std::to_string(pathLength));
        text.setPosition(10, (float)height + 65);
        window.draw(text);
        text.setString("Visited cells: " + std::to_string(numVisitedCells));
        text.setPosition(160, (float)height + 65);
        window.draw(text);
        text.setStyle(sf::Text::Regular);

        // Calcolo del percorso se lo stato della mappa è cambiato
        if(stateHasChanged && map.getGoal() != map.getStart()){
            map.resetForRecalculation();

            auto result = useAStar ?
                AStar::findPath(map, map.getStart(), map.getGoal()) :
                Dijkstra::findPath(map, map.getStart(), map.getGoal());
            //auto result = Dijkstra::findPath(map, map.getStart(), map.getGoal());

            if(!result.second.empty()){
                numVisitedCells = result.second.size();
                for (const auto& cell : result.second) {
                    map.setCellState(cell.x, cell.y, CellState::Visited);
                }
            }
            path = result.first;
            if (!path.empty()) {
                pathLength = path.size();
                for (const auto& cell : path) {
                    map.setCellState(cell.x, cell.y, CellState::Path);
                }
            }
            else{
                std::cout << "No path found.\n";
            }

            stateHasChanged = false;
        }


        // Gestione per il debug mode
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!dWasPressed) {
                map.toggleDebugMode();
                dWasPressed = true;
            }
        } else {
            dWasPressed = false;
        }

        // Gestione del reset della mappa
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!rWasPressed) {
                map.reset();
                numObstacles = dis(gen);
                map.generateRandomObstacles(numObstacles);
                pathLength = 0;
                rWasPressed = true;
                stateHasChanged = true;
            }
        } else {
            rWasPressed = false;
        }

        // Gestione del Perlin noise
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            if (!pWasPressed) {
                map.reset();
                map.generateObstaclesPerlin(threshold, scale, gen());
                numObstacles = 0;
                for (int i = 0; i < map.getWidth(); i++) {
                    for (int j = 0; j < map.getHeight(); j++) {
                        if (map.getCellState(i, j) == CellState::Obstacle) {
                            numObstacles++;
                        }
                    }
                }
                pathLength = 0;
                pWasPressed = true;
                stateHasChanged = true;
            }
        } else {
            pWasPressed = false;
        }

        // Gestione passaggio a edit mode
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            if (!eWasPressed) {
                editMode = !editMode;
                eWasPressed = true;
                hoverShapeCell.setPosition(-100, -100);
            }
        } else {
            eWasPressed = false;
        }

        // Gestione pulsante M per generare un nuovo labirinto
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            if (!mWasPressed) {
                map.generateMaze();
                pathLength = 0;
                mWasPressed = true;
                stateHasChanged = true;
            }
        } else {
            mWasPressed = false;
        }

        // Gestione dei pulsanti
        for (auto& button : buttons) {
            if (button.getLabel() == "border") {
                if (button.checkClick(mousePosAbsolute, sf::Mouse::isButtonPressed(sf::Mouse::Left),
                    sf::Keyboard::isKeyPressed(sf::Keyboard::B) ? sf::Keyboard::B : sf::Keyboard::Unknown)) {
                    //std::cout<<"Button " << button.getLabel() << " state: " << (button.getState() ? "true" : "false") << "\n";
                    drawBorder = button.getState();
                }
            } else if (button.getLabel() == "texture") {
                if (button.checkClick(mousePosAbsolute, sf::Mouse::isButtonPressed(sf::Mouse::Left),
                    sf::Keyboard::isKeyPressed(sf::Keyboard::T) ? sf::Keyboard::T : sf::Keyboard::Unknown)) {
                    //std::cout<<"Button " << button.getLabel() << " state: " << (button.getState() ? "true" : "false") << "\n";
                    drawTexture = button.getState();
                }
            } else if (button.getLabel() == "algorithm") {
                if (button.checkClick(mousePosAbsolute, sf::Mouse::isButtonPressed(sf::Mouse::Left),
                    sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? sf::Keyboard::A : sf::Keyboard::Unknown)) {
                    //std::cout<<"Button " << button.getLabel() << " state: " << (button.getState() ? "true" : "false") << "\n";
                    useAStar = button.getState();
                    stateHasChanged = true;
                }
            }
        }

        // Gestione dell'hover sulle celle
        if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
            hoverShapeCell.setPosition((float) mousePos.x * map.getCellSize(), (float) mousePos.y * map.getCellSize());
        }
        else{
            hoverShapeCell.setPosition(-100, -100);
        }

        map.draw(window, drawBorder, drawTexture);


        window.draw(hoverShapeCell);

        // Gestione grafica dei pulsanti
        for (auto & button : buttons) {
            if (button.isHovered(mousePosAbsolute))
                window.draw(button.getHoverShape());
            button.setColor();
            window.draw(button.getShape());
        }

        player.draw(window);

        window.display();
    }

    return 0;
}

void PrintMapConsole(const Map &map) {
    for (int i = 0; i < map.getWidth(); i++) {
        for (int j = 0; j < map.getHeight(); j++) {
            CellState state = map.getCellState(i, j);
            std::cout << static_cast<int>(state) << " ";
        }
        std::cout << "\n";
    }
}
