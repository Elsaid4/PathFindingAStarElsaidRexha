//
// Created by elsai on 21/11/2025.
//

#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <utility>


enum class Type { Toggle, Standard };

class Button {
private:
    std::string id;
    std::string group;              // Per eventuali gruppi di bottoni (es. radio buttons). Se vuoto, il bottone non appartiene a nessun gruppo.
    sf::RectangleShape shape;
    sf::RectangleShape hoverShape;
    sf::Text labelText;
    
    bool canBeUsed = true;
        
    bool state = false;
    bool lastMousePressed = false;
    bool lastKeyPressed = false;
    sf::Keyboard::Key hotkey = sf::Keyboard::Unknown;;
    Type type = Type::Toggle;
public:
    std::function<void(bool)> onChange; // Chiamata quando lo stato cambia


    Button(std::string  id_,
           const std::string& label,
           const sf::Vector2f& pos,
           const sf::Vector2f& size,
           const Type t = Type::Toggle,
           const sf::Keyboard::Key key = sf::Keyboard::Unknown,
           const sf::Font* font = nullptr)
        : id(std::move(id_)), hotkey(key), type(t)
    {
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::Green);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.f);

        hoverShape = shape;
        hoverShape.setFillColor(sf::Color(0, 0, 0, 50));
        hoverShape.setOutlineColor(sf::Color::Black);
        hoverShape.setOutlineThickness(2);

        if (font) {
            labelText.setFont(*font);
            labelText.setString(label);
            labelText.setCharacterSize(12);
            labelText.setFillColor(sf::Color::Black);
            // center text roughly
            sf::FloatRect tb = labelText.getLocalBounds();
            labelText.setPosition(pos.x + (size.x - tb.width) / 2.f - tb.left,
                                  pos.y + (size.y - tb.height) / 2.f - tb.top);
        }
    }

    /// Controlla se un punto passato è all'interno del pulsante.
    bool contains(const sf::Vector2f& point) const {
        return shape.getGlobalBounds().contains(point);
    }

    /// Processa un evento; ritorna true se lo stato è cambiato
    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        bool changed = false;

        if (!canBeUsed) {
            setColor();
            return false;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && !lastMousePressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            if (contains(mousePos)) {
                if (type == Type::Toggle) {
                    state = !state;
                } else {
                    // momentary: set true, caller può resettare se necessario
                    state = true;
                }
                changed = true;
                if (onChange)
                    onChange(state);
            }
        } else if (event.type == sf::Event::KeyReleased && event.key.code == hotkey && hotkey != sf::Keyboard::Unknown && !lastKeyPressed) {
            if (type == Type::Toggle) {
                state = !state;
            }
            else {
                state = true;
            }
            changed = true;
            if (onChange)
                onChange(state);
        }
        lastMousePressed = event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left;
        lastKeyPressed = event.type == sf::Event::KeyReleased && event.key.code == hotkey;
        setColor();
        return changed;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
        if (labelText.getString() != "")
            window.draw(labelText);
        sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (contains(mp)) {
            window.draw(hoverShape);
        }

    }


    void setColor() {
        if (!canBeUsed)
            shape.setFillColor(sf::Color(100, 100, 100)); // grigio se non utilizzabile
        else
            shape.setFillColor(!state ? sf::Color::Red : sf::Color::Green);
    }

    bool getState() const { return state; }
    void setState(bool s) { state = s; }

    std::string getGroup() const { return group; }
    void setGroup(const std::string& group_) { group = group_; }

    std::string getId() const { return id; }

    void setCanBeUsed(bool b) { canBeUsed = b; }



};



#endif //BUTTON_H
