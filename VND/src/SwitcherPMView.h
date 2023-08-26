#pragma once
#include "gui/ViewSwitcher.h"
#include "../../common/include/VNDLib.h"
#include "ViewTick.h"
#include "ViewTeam.h"
#include "ViewProj.h"
#include "../../common/include/Globals.h"

//#include "ViewStatUser.h"
//#include "ViewReqUser.h"
class VND_LIB_API PMSwitcher : public gui::ViewSwitcher {
private:
protected:
      ViewTick _viewTick;
      ViewTeam _viewTeam;
      ViewProj _viewProj;

public:
    PMSwitcher()
        : gui::ViewSwitcher(3)

    {
        addView(&_viewTick, true);
        addView(&_viewTeam, true);
        addView(&_viewProj, true);
        showView(0);
    }
    void refresh(td::INT4 arg) {
        Globals::_currentUserID = arg;
        _viewTick.populateData(1);
    }

};