#pragma once
#include "gui/ViewSwitcher.h"
#include "../../common/include/VNDLib.h"
#include "ViewTick.h"
#include "ViewTeam.h"
#include "ViewProj.h"
#include "ViewStat.h"
#include "ViewZah.h"
#include "../../common/include/Globals.h"

//#include "ViewStatUser.h"
//#include "ViewReqUser.h"
class VND_LIB_API PMSwitcher : public gui::ViewSwitcher {
private:
protected:
      ViewTick _viewTick;
      ViewTeam _viewTeam;
      ViewProj _viewProj;
      ViewStat _viewStat;
      ViewZah _viewZah;

public:
    PMSwitcher()
        : gui::ViewSwitcher(5)

    {
        addView(&_viewTick, true);
        addView(&_viewTeam, true);
        addView(&_viewProj, true);
        addView(&_viewZah, true);
        addView(&_viewStat, true);
        showView(0);
    }
    void refresh(td::INT4 arg) {
        Globals::_currentUserID = arg;
        _viewTick.populateData(1);
        _viewTeam.populateData(1);
        _viewZah.populateData(1);
        _viewStat.refresh();
        _viewProj.populateData(1);
    }

};