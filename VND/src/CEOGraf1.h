//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>
#include <gui/Font.h>


class CEOGraf1 : public gui::Canvas
{
private:
protected:

    gui::DrawableString _xosa, _yosa;
    gui::Shape _polje, _x, _y;
    gui::Shape _strelica1, _strelica2;
    gui::Size _size;

protected:

    void createShapes()
    {
        _xosa = "Težina";
        _yosa = "Dani";

        double a = 200, b=100;
        gui::Point pointst[] = { {a,-b}, {a,b}, {-a,b}, {-a,-b} };
        size_t nPointst = sizeof(pointst) / sizeof(gui::Point);
        _polje.createPolygon(pointst, nPointst);

        a -= 20; b -= 20;
        gui::Point pointst2[] = { {a,-1}, {a,1}, {-a,1}, {-a,-1} };
        size_t nPointst2 = sizeof(pointst2) / sizeof(gui::Point);
        _x.createPolygon(pointst2, nPointst2);

        gui::Point pointst3[] = { {1,-b}, {1,b}, {-1,b}, {-1,-b} };
        size_t nPointst3 = sizeof(pointst3) / sizeof(gui::Point);
        _y.createPolygon(pointst3, nPointst3);

        double c = 5;
        gui::Point pointst4[] = { {0, c}, {0, -c}, {c * 1.5, 0} };
        size_t nPointst4 = sizeof(pointst4) / sizeof(gui::Point);
        _strelica1.createPolygon(pointst4, nPointst4);

        gui::Point pointst5[] = { {-c, 0}, {c, 0}, {0, -c * 1.5} };
        size_t nPointst5 = sizeof(pointst5) / sizeof(gui::Point);
        _strelica2.createPolygon(pointst5, nPointst5);

    }

public:
    CEOGraf1()
    {
        enableResizeEvent(true);
        createShapes();
    }

    void onDraw(const gui::Rect& rect) override
    {
        gui::Transformation zoomTr;
        double zoom = 1;
        zoomTr.scale(zoom);
        zoomTr.appendToContext();
        
        {
            gui::Transformation tr;
            gui::Transformation::saveContext();
            tr.translate(_size.width * 0.5, _size.height * 0.5);
            tr.appendToContext();
            _polje.drawFillAndWire(td::ColorID::White, td::ColorID::SysText);
            gui::Transformation::restoreContext();
        }
        {   
            gui::Point pt(_size.width * 0.5 - 2*strlen("Dani"), _size.height * 0.5 + 110);
            _yosa.draw(pt, gui::Font::ID::SystemNormalBold, td::ColorID::Black);
        } 
        {
            gui::Transformation tr2;
            gui::Transformation::saveContext();
            tr2.translate(_size.width * 0.5, _size.height * 0.5 + 80);
            tr2.appendToContext();
            _x.drawFill(td::ColorID::Black);

            gui::Transformation tr4;
            tr4.translate(180, 0);
            tr4.appendToContext();
            _strelica1.drawFill(td::ColorID::Black);
            gui::Transformation::restoreContext();
        }
        {
            gui::Transformation tr3;
            tr3.translate(_size.width * 0.5-180, _size.height * 0.5);
            tr3.appendToContext();
            _y.drawFill(td::ColorID::Black);

            gui::Transformation tr4;
            tr4.translate(0, -80);
            tr4.appendToContext();
            _strelica2.drawFill(td::ColorID::Black);
            gui::Transformation::restoreContext();
        }


        {
            
            
            gui::Transformation tr1;
            //tr1.rotateDeg(-90);
            tr1.appendToContext();
            gui::Point pt1(600, 600);
            _xosa.draw(pt1, gui::Font::ID::SystemNormalBold, td::ColorID::Red);
            gui::Transformation::restoreContext();
            
            /*
            gui::Transformation tr;
            
                        gui::Point pt(_size.height * 0.5, _size.width * 0.5);
                        tr.rotateDeg(-90);
                        tr.appendToContext();
                        _xosa.draw(pt, gui::Font::ID::SystemNormalBold, td::ColorID::Red);
                        gui::Transformation::restoreContext();
                        */
                        //gui::Point pt(_size.height * 0.5-200, -_size.width * 0.35);
                        //gui::Point pt(_size.height * 0.5 - 200, -_size.width * 0.5);
        }
    }

    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
    }

    void setVisualEffects(bool bUseCustomColor, td::ColorID jetClrID, td::ColorID etfColorID)
    {
        reDraw();
    }
};
