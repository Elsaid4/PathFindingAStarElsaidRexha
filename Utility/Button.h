//
// Created by elsai on 21/11/2025.
//

#ifndef BUTTON_H
#define BUTTON_H



class Button {
private:
    float x, y, width, height;
    bool isPressed;
    bool lastMousePressed;
    bool lastKeyPressed;
    bool isToggleButton;
    sf::Keyboard::Key key;
    sf::RectangleShape shape;
    sf::RectangleShape hoverShape;
public:
    Button(float x, float y, float width, float height, bool isToggleButton = false)
        : x(x), y(y), width(width), height(height), isPressed(false), lastMousePressed(false), isToggleButton(isToggleButton) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);

        hoverShape.setSize(sf::Vector2f(width, height));
        hoverShape.setPosition(x, y);
        hoverShape.setFillColor(sf::Color(0, 0, 0, 50));
        hoverShape.setOutlineColor(sf::Color::Black);
        hoverShape.setOutlineThickness(2);
    }

    Button(const sf::Vector2f position, const sf::Vector2f size, bool pressed, sf::Keyboard::Key keyAssociated, bool isToggleButton = false)
        : Button(position.x, position.y, size.x, size.y, isToggleButton) {
        key = keyAssociated;
        isPressed = pressed;
    }

    bool checkClick(const sf::Vector2f mousePos, const bool mousePressed, sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown) {
        bool clicked = false;
        if (keyPressed == key && !lastKeyPressed) {
            if (isToggleButton) {
                isPressed = !isPressed;
                clicked = true;
            }
            else {
                if (!isPressed) {
                    isPressed = true;
                    clicked = true;
                }
            }
        }
        else {
            if (isHovered(mousePos)) {
                // rileva fronte di salita: tasto premuto adesso ma non prima
                if (mousePressed && !lastMousePressed) {
                    if (isToggleButton) {
                        isPressed = !isPressed;
                        clicked = true;
                    } else {
                        if (!isPressed) {
                            isPressed = true;
                            clicked = true;
                        }
                    }
                }
                // su rilascio, per i bottoni non toggle resettare lo stato
                if (!mousePressed && lastMousePressed && !isToggleButton) {
                    isPressed = false;
                }
            } else {
                // se il cursore esce e il pulsante viene rilasciato, resettare per non-toggle
                if (!mousePressed && lastMousePressed && !isToggleButton) {
                    isPressed = false;
                }
            }
        }
        lastKeyPressed = keyPressed == key;
        lastMousePressed = mousePressed;
        return clicked;
    }

    void setColor() {
        shape.setFillColor(isPressed ? sf::Color::Red : sf::Color::Green);
    }

    bool isHovered(const sf::Vector2f mousePos) const {
        return (mousePos.x >= x && mousePos.x <= x + width
            && mousePos.y >= y && mousePos.y <= y + height);
    }

    void setIsPressed(bool pressed) { isPressed = pressed; }

    sf::RectangleShape getShape() { return shape; }
    sf::RectangleShape getHoverShape() { return hoverShape; }

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

};



#endif //BUTTON_H
