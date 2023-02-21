#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "../../VND/src/NavigatorView.h" 

class MainWindow : public gui::Window
{
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    NavigatorView _view;

public:
    MainWindow()
    : gui::Window(gui::Geometry(0, 0, 1000, 500))
    {
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_view);
    }
    
    ~MainWindow()
    {
    }
    
protected:
    
    virtual bool onActionItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID, gui::ActionItem* pAI)
    {
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            return handleMenuControl(actionID, pAI);
        }
        if (menuID == 30 && firstSubMenuID == 0 && lastSubMenuID == 0)
        {
            return handleMenuRecap(actionID, pAI);
        }
        return true;
    }

    bool handleMenuControl(td::BYTE actionID, gui::ActionItem* pAI)
    {
        switch (actionID)
        {
        case 10:
        {
            //prikazi view za projekte
            return true;
        }

        case 20:
        {
            //prikazi view za uposleniike
            return true;
        }
        case 30:
        {
            //prikazi view za zahtjeve
        }
        default:
            return false;
        }
    }

    bool handleMenuRecap(td::BYTE actionID, gui::ActionItem* pAI)
    {
        switch (actionID)
        {
        case 10:
        {
            //prikazi Pretraga view
            return true;
        }

        case 20:
        {
            //prikazi Statistika view
            return true;
        }

        default:
            return false;
        }
    }
    
};
