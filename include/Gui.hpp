//
// Created by vadim on 14.09.20.
//

#ifndef SERVER_GUI_HPP
#define SERVER_GUI_HPP

#include <string>
#include <ncurses.h>
#include "Client.hpp"
#include "Programmer.hpp"
#include "User.hpp"

class Screen
{
public:
    Screen();
    ~Screen();
    bool menu();
    bool switch_on_highlight(int highlight);
    bool authentificate();
    bool switch_on_auth(int highlight);
private:
    WINDOW* menuwin_;
    bool isRefreshed_;
};

std::string getstring();

#endif //SERVER_GUI_HPP
