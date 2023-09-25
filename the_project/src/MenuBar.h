#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu menuApp;
    gui::SubMenu menuControl;
    gui::SubMenu menuRecap;
protected:
    void populateMenuApp()
    {
        auto& items = menuApp.getItems();
        items[0].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
    
    void populateMenuControl()
    {
        auto& items = menuControl.getItems();
        items[0].initAsActionItem(tr("Requests"), 30);
    }

    void populateMenuRecap()
    {
        auto& items = menuRecap.getItems();
        items[0].initAsActionItem(tr("Statistics"), 20);
    }
public:
    MenuBar()
    : gui::MenuBar(3) 
    , menuApp(10, tr("App"), 1) 
    , menuControl(20, tr("Control"), 1)
    , menuRecap(30, tr("Recap"), 1)
    {
        populateMenuApp();
        populateMenuControl();
        populateMenuRecap();
        _menus[0] = &menuApp;
        _menus[1] = &menuControl;
        _menus[2] = &menuRecap;
    }
    
    ~MenuBar()
    {
    }
};
