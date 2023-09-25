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
    td::INT4 tipid;
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
        //check if it is Tip user
        dp::IStatementPtr pstat1 = dp::getMainDatabase()->createStatement("SELECT TipID FROM Korisnik WHERE ID=?");
        dp::Params params(pstat1->allocParams());
        dp::Columns cols(pstat1->allocBindColumns(1));
        params << arg;
        cols << "TipID" << tipid;
        pstat1->execute();
        pstat1->moveNext();
 
        if (tipid == 3)
        {
            _viewTickUser.populateData();
            _viewReqUser.populateData();
            _viewStatUser.populateData();
        }
    }
    void onChangedView(int currSelection) {
        if(tipid==3)
         _viewStatUser.updateVelo();
        showView((int)currSelection);
    }
};