#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>

#include "Map.h"
#include "AStar.h"
#include "GameCharacter.h"

void PrintMapConsole(const Map &map);

sf::Font font;

int main() {
    int width = 800;
    int height = 800;
    int menuHeight = 90;
    sf::RenderWindow window(sf::VideoMode(width, height + menuHeight), "A* Pathfinder",sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);


    if (!font.loadFromFile("../Font/OpenSans-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Error loading font\n";
    }

    int mapWidth = 40;
    int mapHeight = 40;
    Map map(mapWidth, mapHeight, width / mapWidth);


    int numObstacles;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, mapWidth * mapHeight / 1.5);
    numObstacles = dis(gen);
    //map.generateRandomObstacles(numObstacles);
    float threshold = 0.55f;
    float scale = 0.2f;
    map.generateObstaclesPerlin(threshold, scale, gen());

    /*
    std::cout << "Mappa iniziale\n";
    PrintMapConsole(map);
    */

    GameCharacter player(map.getStart(), map.getCellSize() / 2.0f);
    sf::Clock clock;
    float moveDelay = 0.1f;

    bool editMode = true;

    int pathLength = 0;
    int numVisitedCells = 0;

    // Variabili per la gestione dei pulsanti. Per evitare che il tasto venga premuto più volte in un frame.
    bool dWasPressed = false;
    bool sWasPressed = false;
    bool gWasPressed = false;
    bool rWasPressed = false;
    bool bWasPressed = false;
    bool eWasPressed = false;
    bool pWasPressed = false;
    bool tWasPressed = false;

    // Inizializzo a true per forzare il calcolo del percorso all'inizio
    bool stateHasChanged = true;

    bool startSetMode = false;
    bool goalSetMode = false;

    bool setBorder = false;
    bool drawTexture = true;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);

    // Pulsanti
    sf::Vector2i buttonSize(15, 15);

    sf::RectangleShape startModeButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i startModeButtonPosition(775, height + 4);
    startModeButton.setPosition((float) startModeButtonPosition.x, (float) startModeButtonPosition.y);

    sf::RectangleShape goalModeButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i goalModeButtonPosition(775, height + 27);
    goalModeButton.setPosition((float) goalModeButtonPosition.x, (float) goalModeButtonPosition.y);

    sf::RectangleShape borderButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i borderButtonPosition(775, height + 50);
    borderButton.setPosition((float)borderButtonPosition.x, (float)borderButtonPosition.y);

    sf::RectangleShape showTextureButton(sf::Vector2f((float)buttonSize.x, (float)buttonSize.y));
    sf::Vector2i showTextureButtonPosition(775, height + 73);
    showTextureButton.setPosition((float) showTextureButtonPosition.x, (float) showTextureButtonPosition.y);


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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mousePos.x /= (int)map.getCellSize();
        mousePos.y /= (int)map.getCellSize();

        if(editMode){
            window.clear(sf::Color::White);

            // Gestione posizionamento delle celle
            // Si ottiene la posizione del mouse e si verifica se è all'interno della mappa
            // Se il tasto destro è premuto si piazza una cella walkable, altrimenti si piazza un ostacolo
            if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
                if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    if(map.canPlaceWalkable(mousePos.x, mousePos.y)){
                        //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") walkable\n";
                        map.setCellState(mousePos.x, mousePos.y, CellState::Walkable);
                        stateHasChanged = true;
                        numObstacles--;
                    }
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                    // Si posiziona start/goal con il tasto sinistro. Si verifica se si è in modalità, altrimenti si piazza un ostacolo
                    if(startSetMode){
                        //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") start\n";
                        if(map.getCellState(mousePos.x, mousePos.y) == CellState::Obstacle){
                            numObstacles--;
                        }
                        map.setCellState(mousePos.x, mousePos.y, CellState::Start);
                        map.setStart(mousePos);
                        stateHasChanged = true;
                    }
                    else if(goalSetMode){
                        //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") goal\n";
                        if(map.getCellState(mousePos.x, mousePos.y) == CellState::Obstacle){
                            numObstacles--;
                        }
                        map.setCellState(mousePos.x, mousePos.y, CellState::Goal);
                        map.setGoal(mousePos);
                        stateHasChanged = true;
                    }
                    else if(map.canPlaceObstacle(mousePos.x, mousePos.y)){
                        //std::cout << "Cella (" << mousePos.x << ", " << mousePos.y << ") ostacolo\n";
                        map.setCellState(mousePos.x, mousePos.y, CellState::Obstacle);
                        stateHasChanged = true;
                        numObstacles++;
                    }
                }


                // Gestione della modalità di piazzamento del punto di partenza
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
                   (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                   && sf::Mouse::getPosition(window).x >= startModeButtonPosition.x && sf::Mouse::getPosition(window).x <= startModeButtonPosition.x + buttonSize.x
                   && sf::Mouse::getPosition(window).y >= startModeButtonPosition.y && sf::Mouse::getPosition(window).y <= startModeButtonPosition.y + buttonSize.y) {
                    if (!sWasPressed) {
                        startSetMode = !startSetMode;
                        goalSetMode = false;
                        sWasPressed = true;
                    }
                } else {
                    sWasPressed = false;
                }

                // Gestione della modalità di piazzamento del punto di arrivo
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) ||
                    (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    && sf::Mouse::getPosition(window).x >= goalModeButtonPosition.x && sf::Mouse::getPosition(window).x <= goalModeButtonPosition.x + buttonSize.x
                    && sf::Mouse::getPosition(window).y >= goalModeButtonPosition.y && sf::Mouse::getPosition(window).y <= goalModeButtonPosition.y + buttonSize.y) {
                    if (!gWasPressed) {
                        goalSetMode = !goalSetMode;
                        startSetMode = false;
                        gWasPressed = true;
                    }
                } else {
                    gWasPressed = false;
                }
            }

        } else{
            window.clear(sf::Color(150, 150, 150));

            // Gestione movimento del personaggio ogni moveDelay secondi
            if (clock.getElapsedTime().asSeconds() >= moveDelay) {
                auto playerPos = player.update(map, path);
                if(playerPos != map.getStart()){
                    map.setStart(player.update(map, path));
                    stateHasChanged = true;
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

        text.setString("Press 'S' to set start point\nPress 'G' to set goal point\nPress 'B' to toggle border\nPress 'T' to change texture");
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



        // Gestione dei pulsanti
        borderButton.setFillColor(setBorder ? sf::Color::Green : sf::Color::Red);
        window.draw(borderButton);

        startModeButton.setFillColor(startSetMode ? sf::Color::Green : sf::Color::Red);
        window.draw(startModeButton);

        goalModeButton.setFillColor(goalSetMode ? sf::Color::Green : sf::Color::Red);
        window.draw(goalModeButton);

        showTextureButton.setFillColor(drawTexture ? sf::Color::Green : sf::Color::Red);
        window.draw(showTextureButton);

        if(stateHasChanged && map.getGoal() != map.getStart()){
            map.resetForRecalculation();
            /*
            std::cout << "Prima\n";
            PrintMapConsole(map);
            std::cout << "\n\n\n";
            */

            auto result = AStar::findPath(map, map.getStart(), map.getGoal());

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

            /*
            std::cout << "DOPO\n";
            PrintMapConsole(map);
            std::cout << "\n\n\n";
            */
        }

        // Gestione del pulsante per il debug mode
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) ||
            (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            && sf::Mouse::getPosition(window).x >= showTextureButtonPosition.x && sf::Mouse::getPosition(window).x <= showTextureButtonPosition.x + buttonSize.x
            && sf::Mouse::getPosition(window).y >= showTextureButtonPosition.y && sf::Mouse::getPosition(window).y <= showTextureButtonPosition.y + buttonSize.y) {
            if (!tWasPressed) {
                drawTexture = !drawTexture;
                tWasPressed = true;
            }
        } else {
            tWasPressed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!dWasPressed) {
                map.toggleDebugMode();
                dWasPressed = true;
            }
        } else {
            dWasPressed = false;
        }



        // Gestione bottone per il bordo
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) ||
            (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            && sf::Mouse::getPosition(window).x >= borderButtonPosition.x && sf::Mouse::getPosition(window).x <= borderButtonPosition.x + buttonSize.x
            && sf::Mouse::getPosition(window).y >= borderButtonPosition.y && sf::Mouse::getPosition(window).y <= borderButtonPosition.y + buttonSize.y) {
            if (!bWasPressed) {
                setBorder = !setBorder;
                bWasPressed = true;
            }
        } else {
            bWasPressed = false;
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

        // Gestione dell'hover sulle celle
        if(mousePos.x >= 0 && mousePos.x < map.getWidth() && mousePos.y >= 0 && mousePos.y < map.getHeight()){
            hoverShapeCell.setPosition((float) mousePos.x * map.getCellSize(), (float) mousePos.y * map.getCellSize());
        }
        else{
            hoverShapeCell.setPosition(-100, -100);
        }

        map.draw(window, font, setBorder, drawTexture);


        // TODO: Migliore gestione dei pulsanti con classe dedicata
        // Gestione dell'hover sui bottoni
        if(editMode && sf::Mouse::getPosition(window).x >= goalModeButtonPosition.x && sf::Mouse::getPosition(window).x <= goalModeButtonPosition.x + buttonSize.x
           && sf::Mouse::getPosition(window).y >= goalModeButtonPosition.y && sf::Mouse::getPosition(window).y <= goalModeButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) goalModeButtonPosition.x, (float) goalModeButtonPosition.y));
        }
        else if(editMode && sf::Mouse::getPosition(window).x >= startModeButtonPosition.x && sf::Mouse::getPosition(window).x <= startModeButtonPosition.x + buttonSize.x
                && sf::Mouse::getPosition(window).y >= startModeButtonPosition.y && sf::Mouse::getPosition(window).y <= startModeButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) startModeButtonPosition.x, (float) startModeButtonPosition.y));
        }
        else if(sf::Mouse::getPosition(window).x >= borderButtonPosition.x && sf::Mouse::getPosition(window).x <= borderButtonPosition.x + buttonSize.x
                && sf::Mouse::getPosition(window).y >= borderButtonPosition.y && sf::Mouse::getPosition(window).y <= borderButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) borderButtonPosition.x, (float) borderButtonPosition.y));
        }
        else if(sf::Mouse::getPosition(window).x >= showTextureButtonPosition.x && sf::Mouse::getPosition(window).x <= showTextureButtonPosition.x + buttonSize.x
                && sf::Mouse::getPosition(window).y >= showTextureButtonPosition.y && sf::Mouse::getPosition(window).y <= showTextureButtonPosition.y + buttonSize.y)
        {
            hoverShapeButton.setPosition(sf::Vector2f((float) showTextureButtonPosition.x, (float) showTextureButtonPosition.y));
        }
        else{
            hoverShapeButton.setPosition(-100, -100);
        }

        window.draw(hoverShapeCell);
        window.draw(hoverShapeButton);

        player.draw(window, map.getCellSize());

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
