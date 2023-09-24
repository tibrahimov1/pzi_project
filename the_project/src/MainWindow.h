#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "NewToolbar.h"
#include "../../VND/src/MainSwitcher.h"
#include "../../VND/src/NavigatorView.h" 
#include "../../VND/src/DialogLogin.h"
#include "../../VND/src/ViewProj.h"
#include "../../VND/src/InitView.h"
#include "../../common/include/Globals.h"
<<<<<<< HEAD
=======
#include "../../VND/src/PMNavigatorView.h"
#include "../../VND/src/UserNavigatorView.h"

>>>>>>> makov
td::INT4 Globals::_currentUserID = -1;

class MainWindow : public gui::Window
{
protected:
    MenuBar _mainMenuBar;

    ToolBar _toolBar;
    NavigatorView _view;
    ViewProj _vProj;
    InitView _initView;
    NewToolBar _newToolBar;

    //NavigatorView _viewNav;
    //PMNavigatorView _viewPM;
    //UserNavigatorView _viewUser;

public:
    MainWindow()
    : gui::Window(gui::Geometry(0, 0, 1000, 500))
    {
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_initView);
    }
    
    ~MainWindow()
    {
    }
    
protected:
    bool shouldClose() override
    {
        return true;
    }

    void onClose() override
    {
        gui::Window::onClose();
    }

    void onInitialAppearance() override
    {
        showLogin();
    }
    void setView() {

        if (Globals::_currentUserID == 1)
            _initView.showCEO(); //for now only change to CEO
        else if (Globals::_currentUserID == 2) {
            _initView.showPM();
            //_toolBar.hide(true);
            //NewToolBar mak();
            //setToolBar(mak);
            //mak.show(true);
        }
        else {
            _initView.showUser();
        }
    }

    void MainWindow::showLogin()
    {
        DialogLogin* pDlg = new DialogLogin(this, (std::function<void()>)std::bind(&MainWindow::setView, this));
        pDlg->openModal(DlgID::Login, this);
        pDlg->setTitle(tr("Login"));
    }

    bool MainWindow::onClick(gui::Dialog* pDlg, td::UINT4 dlgID)
    {
        if ((DlgID)dlgID == DlgID::Login)
        {
            auto btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK)
            {
                //insert
                //setToolBar(_newToolBar);
            }
            else
                close();

            return true;
        }
        return false;
    }
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
            //_view.setNavigatorSelection(0);
            //_viewPM.setNavigatorSelection(0);
            _initView.dajTo(Globals::_currentUserID,1);
            return true;
        }

        case 20:
        {
            //prikazi view za uposlenike
            //_view.setNavigatorSelection(1);
            //_viewPM.setNavigatorSelection(1);
            _initView.dajTo(Globals::_currentUserID,2);
            return true;
        }
        case 30:
        {
            //prikazi view za zahtjeve
            _initView.dajTo(Globals::_currentUserID, 3);
            return true;
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
            _initView.dajTo(Globals::_currentUserID, 4);
            return true;
        }

        case 20:
        {
            //prikazi Statistika view
            _initView.dajTo(Globals::_currentUserID, 5);
            return true;
        }

        default:
            return false;
        }
    }
    
};
