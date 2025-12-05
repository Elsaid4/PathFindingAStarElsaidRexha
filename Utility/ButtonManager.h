//
// Created by elsai on 05/12/2025.
//

#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>
#include "Button.h"

class ButtonManager {
private:
    std::vector<Button> buttons;
    std::unordered_map<std::string, std::vector<size_t>> groups;
public:
    void addButton(Button& button) {
        const size_t idx = buttons.size();
        if (!button.getGroup().empty())
            groups[button.getGroup()].push_back(idx);
        buttons.emplace_back(std::move(button));
    }

    /// Processa un evento SFML; gestisce anche l'esclusività dei gruppi
    void processEvent(const sf::Event& event, const sf::RenderWindow& window) {
        for (size_t i = 0; i < buttons.size(); ++i) {
            bool changed = buttons[i].handleEvent(event, window);
            if (changed && !buttons[i].getGroup().empty() && buttons[i].getState()) {
                // se è toggled on e appartiene a un gruppo, spegni gli altri
                auto &vec = groups[buttons[i].getGroup()];
                for (size_t j : vec) {
                    if (j != i && buttons[j].getState()) {
                        buttons[j].setState(false);
                        if (buttons[j].onChange)
                            buttons[j].onChange(false);
                    }
                }
            }
        }
    }

    void drawAll(sf::RenderWindow& window) const {
        for (auto& b : buttons)
            b.draw(window);
    }

    /// Utility per trovare e settare stato da codice esterno
    void setState(const std::string& id, const bool s) {
        for (auto& b : buttons) {
            if (b.getId() == id) {
                b.setState(s);
                if (b.onChange)
                    b.onChange(s);
                // gestire gruppi
                if (s && !b.getGroup().empty()) {
                    auto &vec = groups[b.getGroup()];
                    for (const size_t j : vec) {
                        if (buttons[j].getId() != id && buttons[j].getState()) {
                            buttons[j].setState(false);
                            if (buttons[j].onChange)
                                buttons[j].onChange(false);
                        }
                    }
                }
                break;
            }
        }
    }

    bool getButtonState(const std::string& id) const {
        for (const auto& b : buttons) {
            if (b.getId() == id) {
                return b.getState();
            }
        }
        return false;
    }

    void setCanBeUsedButton(const std::string& id, const bool b) {
        for (auto& but : buttons) {
            if (but.getId() == id) {
                but.setEnabled(b);
                break;
            }
        }
    }
};


#endif //BUTTONMANAGER_H
