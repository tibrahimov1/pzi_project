#pragma once
#include <gui/Dialog.h>
#include "ViewLogin.h"
#include <dp/IDatabase.h>
#include "../../common/include/Globals.h"
#include "../../common/include/VNDLib.h"
#include <functional>

class VND_LIB_API DialogLogin : public gui::Dialog
{
	ViewLogin _viewLogin;
	std::function<void()> _callBack;
protected:
    bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == gui::Dialog::Button::ID::OK && Globals::_currentUserID == -1)
        {
            td::String strPwd = _viewLogin.getPassword();
            td::String userName = _viewLogin.getUserName();
            td::INT4 userID;
            td::String passDB;

            dp::IStatementPtr pSel(dp::getMainDatabase()->createStatement("select ID, Sifra from Korisnik where Username = ?"));
            dp::Params pParams(pSel->allocParams());
            pParams << dp::toNCh(userName, 100);

            dp::Columns pCols = pSel->allocBindColumns(2);
            pCols << "ID" << userID << "Sifra" << passDB;
            if (!pSel->execute())
            {
                showAlert(tr("WrongPass"), tr("EnterCorrPass"));
                return false;

            }
            if (!pSel->moveNext())
            {
                showAlert(tr("WrongPass"), tr("EnterCorrPass"));
                return false;
            }
            if (strPwd != passDB)
            {
                showAlert(tr("WrongPass"), tr("EnterCorrPass"));
                return false;
            }

            Globals::_currentUserID = userID;
            _callBack();
            return true;
        }
        else if (btnID == gui::Dialog::Button::ID::Cancel)
            return true;
    }
public:
    DialogLogin(gui::Frame* pView,std::function <void()> pfun)
        : gui::Dialog(pView, {  {gui::Dialog::Button::ID::OK, tr("OK"), gui::Button::Type::Default},
                                {gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 150))
        , _viewLogin()
        , _callBack(pfun)
    {

        setCentralView(&_viewLogin);
    }
};
