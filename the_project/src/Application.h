#pragma once
#include <gui/Application.h>
#include "MainWindow.h"
#include <dp/IDatabase.h>
class Application : public gui::Application
{
protected:
    
    gui::Window* createInitialWindow()
    {
        return new MainWindow();
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
    }
    bool connectToDB()
    {
        dp::IDatabase* pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);

        fo::fs::path home;
        mu::getHomePath(home);

        //fo::fs::path DBPath = (home / "other_bin/TestData/SQLite/PoS.db");
        fo::fs::path DBPath = (home / "Work/PZI_Baza/Baza_proj.db");
        if (!pDB->connect(DBPath.string().c_str()))
            return false;

        dp::setMainDatabase(pDB);
        return true;
    }
};
