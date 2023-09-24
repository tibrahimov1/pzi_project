#pragma once
#include "gui/ViewSwitcher.h"
#include "../../common/include/VNDLib.h"
#include "ViewTickUser.h"
#include "../../common/include/Globals.h"
#include "ViewReqUser.h"
#include "ViewStatUser.h"
//#include "ViewReqUser.h"
class VND_LIB_API UserSwitcher : public gui::ViewSwitcher {
private:
protected:
    ViewReqUser  _viewReqUser;
    ViewTickUser _viewTickUser;
    ViewStatUser _viewStatUser;

public:
    UserSwitcher()
        : gui::ViewSwitcher(3)

    {
        addView(&_viewReqUser, true);
        addView(&_viewTickUser, true);
        addView(&_viewStatUser, true);
        showView(0);
    }
    void refresh(td::INT4 arg) {
        Globals::_currentUserID = arg;
        _viewTickUser.populateData();
        _viewReqUser.populateData();
        _viewStatUser.populateData();
    }
    void onChangedView(int currSelection) {
        showView((int)currSelection);
    }
};