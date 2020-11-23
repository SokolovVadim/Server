//
// Created by vadim on 12.09.20.
//

#include "Gui.hpp"
#include <iostream>

Screen::Screen():
    isRefreshed_(false)
{
    // Start curses mode
    initscr();

    // exit out with CTRL + C
    cbreak();

    // do not print characters
    noecho();

    int yMax(0), xMax(0);
    getmaxyx(stdscr, yMax, xMax);

    // create a window for output

    menuwin_ = newwin(CHOICES_NUM + 2, xMax / 2, yMax / 2, xMax / 2);
    box(menuwin_, 0, 0);
    refresh();
    wrefresh(menuwin_);

    // allow using keypad

    keypad(menuwin_, true);
}

Screen::~Screen()
{
    if(!isRefreshed_)
    {
        delwin(menuwin_);

        // end curses mode
        endwin();
    }
}

bool Screen::authentificate()
{
    int choice(0);
    int highlight(0);

    while(true)
    {
        for(int i(0); i < OPTIONS_NUM; ++i)
        {
            if(i == highlight)
            {
                wattron(menuwin_, A_REVERSE);
            }
            mvwprintw(menuwin_, i + 1, 1, options_list[i].c_str());
            wattroff(menuwin_, A_REVERSE);
        }

        choice = wgetch(menuwin_);

        switch(choice)
        {
            case KEY_UP:
            {
                highlight--;
                if(highlight == -1)
                    highlight = 0;
                break;
            }
            case KEY_DOWN:
            {
                highlight++;
                if(highlight == OPTIONS_NUM)
                    highlight = OPTIONS_NUM - 1;
                break;
            }
            default:
            {
                break;
            }
        }
        if(choice == 10) // Enter hitted
            break;
    }
    return switch_on_auth(highlight);
}

bool Screen::menu()
{
    int choice(0);
    int highlight(0);

    while(true)
    {
        for(int i(0); i < CHOICES_NUM; ++i)
        {
            if(i == highlight)
            {
                wattron(menuwin_, A_REVERSE);
            }
            mvwprintw(menuwin_, i + 1, 1, command_list[i].c_str());
            wattroff(menuwin_, A_REVERSE);
        }

        choice = wgetch(menuwin_);

        switch(choice)
        {
            case KEY_UP:
            {
                highlight--;
                if(highlight == -1)
                    highlight = 0;
                break;
            }
            case KEY_DOWN:
            {
                highlight++;
                if(highlight == CHOICES_NUM)
                    highlight = CHOICES_NUM - 1;
                break;
            }
            default:
            {
                break;
            }
        }
        if(choice == 10) // Enter hitted
            break;
    }
    // printw("Your choice was: %s", command_list[highlight].c_str());
    // getch();
    return switch_on_highlight(highlight);
}

bool Screen::switch_on_highlight(int highlight)
{
    switch(highlight)
    {
        case UPLOAD_SCRIPT:
        {
            // clear previous screen
            wrefresh(menuwin_);
            clear();

            // get string from stdin
            mvprintw(0, 0, "%s", "Enter script name: ");
            std::string input_str = getstring();

            // delete window and clear screen
            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;

            // perform client routine
            ClientRoutine(input_str);

            break;
        }
        case DOWNLOAD_SCRIPT:
        {
            wrefresh(menuwin_);
            clear();

            mvprintw(0, 0, "%s", "Enter script name: ");
            std::string input_str = getstring();

            // delete window and clear screen
            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;

            ProgrammerRoutine(input_str);
            break;
        }
        case SHOW_SCRIPT_LIST:
        {
            // delete window and clear screen
            wrefresh(menuwin_);
            clear();
            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;

            ShowScriptList();

            break;
        }
        case EXECUTE_SCRIPT:
        {
            wrefresh(menuwin_);
            clear();

            mvprintw(0, 0, "%s", "Enter script name: ");

            std::string script_name = getstring();

            // delete window and clear screen
            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;

            ExecuteScript(script_name);

            break;
        }
        case EXIT:
        {
            wrefresh(menuwin_);
            clear();
            // delete window and clear screen
            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;
            return true;
        }
        default:
            break;
    }
    return false;
}

bool Screen::switch_on_auth(int highlight)
{
    switch(highlight)
    {
        case SIGN_IN:
        {
            wrefresh(menuwin_);
            clear();

            mvprintw(0, 0, "%s", "Username: ");
            std::string username = getstring();

            mvprintw(1, 0, "%s", "Password: ");
            std::string password = getstring();

            // delete window and clear screen

            wrefresh(menuwin_);
            clear();
            move(0, 0);
            int ret_clear = clrtoeol();
            std::cout << "ret clear " << ret_clear << std::endl;

            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;

            sign_in(username, password);
            std::cout << "sign in\n";

            bool isExit(false);
            while(!isExit){
                isExit = this->menu();
            }

            break;
        }
        case SIGN_UP:
        {
            wrefresh(menuwin_);
            clear();

            mvprintw(0, 0, "%s", "Username: ");
            std::string username = getstring();
            mvprintw(1, 0, "%s", "Password: ");
            std::string password = getstring();

            clear();

            // delete window and clear screen

            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;

            sign_up(username, password);
            std::cout << "sign up\n";

            break;
        }
        case EXIT_AUTH:
        {
            wrefresh(menuwin_);
            clear();
            // delete window and clear screen
            delwin(menuwin_);
            endwin();
            isRefreshed_ = true;
            return true;
        }
        default:
            break;
    }
    return false;
}

std::string getstring()
{
    std::string input;

    // let the terminal do the line editing
    nocbreak();
    echo();

    // this reads from buffer after <ENTER>, not "raw"
    // so any backspacing etc. has already been taken care of
    int ch = getch();

    while ( ch != '\n' )
    {
        input.push_back( ch );
        ch = getch();
    }

    // restore your cbreak / echo settings here
    cbreak();
    noecho();

    return input;
}