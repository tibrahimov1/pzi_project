#pragma once
#include <gui/MenuBar.h>

class Menu2 : public gui::MenuBar
{
private:
    gui::SubMenu menuApp;
protected:
    void populateMenuApp()
    {
        auto& items = menuApp.getItems();
        items[0].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }

public:
    Menu2()
        : gui::MenuBar(1)
        , menuApp(10, tr("App"), 1)
    {
        populateMenuApp();
        _menus[0] = &menuApp;

    }

    ~Menu2()
    {
    }
};
