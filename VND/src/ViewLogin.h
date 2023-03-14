#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/Button.h>
#include <gui/PasswordEdit.h>
#include <gui/GridLayout.h>
#include <gui/DBComboBox.h>
#include <dp/IDatabase.h>

class ViewLogin : public gui::View
{
private:
protected:
    gui::Label _lblUn;
    gui::LineEdit _editUN;
    gui::Label _lblPwd;
    gui::PasswordEdit _editPwd;
    gui::GridLayout _gl;
    dp::IDatabase* _db;
public:
    ViewLogin()
        : _lblUn(tr("Username:"))
        , _editUN(gui::LineEdit::Messages::Send)
        , _lblPwd(tr("Password:"))
        , _editPwd(gui::LineEdit::Messages::Send)
        , _db(dp::getMainDatabase())
        , _gl(2, 2)
    {
        //_editPwd.setToolTip(tr("Enter password:"));
        _gl.insert(0, 0, _lblUn);
        _gl.insert(0, 1, _editUN);
        _gl.insert(1, 0, _lblPwd);
        _gl.insert(1, 1, _editPwd);


        gui::View::setLayout(&_gl);
    }
    td::String getUserName() const
    {
        return _editUN.getText();
    }

    td::String getPassword() const
    {
        return _editPwd.getText();
    }
    virtual bool onClick(gui::Button* pBtn)
    {
        return true;
    }
};
