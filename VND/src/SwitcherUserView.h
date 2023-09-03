#pragma once
#include "gui/ViewSwitcher.h"
#include "../../common/include/VNDLib.h"
#include "ViewTickUser.h"
#include "../../common/include/Globals.h"

//#include "ViewStatUser.h"
//#include "ViewReqUser.h"
class VND_LIB_API UserSwitcher : public gui::ViewSwitcher {
private:
protected:
    ViewTickUser _viewTickUser;
 //   ViewStatUser _viewStatUser;
 //   ViewReqUser  _viewReqUser;

public:
    UserSwitcher()
        : gui::ViewSwitcher(3)

    {
        addView(&_viewTickUser, true);
        showView(0);
    }
    void refresh(td::INT4 arg) {
        Globals::_currentUserID = arg;
        _viewTickUser.populateData(1);
    }

};