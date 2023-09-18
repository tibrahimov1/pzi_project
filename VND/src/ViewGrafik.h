//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/Transformation.h>
#include <math/Constants.h>
#include <math/math.h>
#include <gui/DrawableString.h>


const float dS = 50;
const float scaleAttached = 0.12f;
const float scaleFighter = 0.82f;

class ViewGrafik : public gui::Canvas
{
private:
    std::vector<td::INT4> generateAxisNumbers(td::INT4 x, int maxElements) {
        std::vector<td::INT4> result;

        // Add the minimum number (0) to the vector
        result.push_back(0);

        // Choose a suitable step size (divisible by 5 or 10)
        int step = 1;

        // Calculate the number of elements to add between 0 and x
        int numElements = x / step;

        // Make sure we don't exceed the maximum allowed elements
        if (numElements > maxElements) {
            step = x / maxElements;
            numElements = maxElements;
        }

        // Add numbers based on the chosen step until we reach x
        for (int i = 1; i < numElements; i++) {
            int number = i * step;
            result.push_back(number);
        }

        // Add x to the vector if it's not already there
        if (x != result.back()) {
            result.push_back(x);
        }

        return result;
    }

protected:
    std::vector<td::INT4> _xOsa, _yOsa;
    td::INT4 _brojDana, _ukupDana, _sadTez, _ukupTez;
    gui::Size _size;
    gui::Shape _linija;
    gui::Shape _moje;
    float _angle = math::FLT_PI;
    bool _useDrawingAttribs = false;
public:
    ViewGrafik()
       
    {
        enableResizeEvent(true);


        gui::Point p1(20, 20), p2(520,120);
        gui::Point points[]={p1,p2};
        _linija.createLines(points, 2);

        
        //setBackgroundColor(td::ColorID::AquaMarine);


        /*gui::Point r1(_size.width * 0.1, _size.height * 0.1), r2(_size.width * 0.9, _size.height * 0.9);
        gui::Point rr[] = { r1,r2 };
        _moje.createLines(rr, 2);*/

    }



    void StaviTacke(td::INT4 ukupTez, td::INT4 sadTez, td::INT4 brojDana, td::INT4 ukupDana) 
    {
        //stavi u vektore vrijednosti po nekom pravilu (npr 5 vrijednosti pa ih ravnomjerno raspodjeli)
        _xOsa = generateAxisNumbers(ukupDana, 5);
        _yOsa = generateAxisNumbers(ukupTez, 5);
        _brojDana = brojDana;
        _ukupDana = ukupDana;
        _sadTez = sadTez;
        _ukupTez = ukupTez;

        reDraw();
    }

    void initAngle(float initialAngle)
    {
        _angle = initialAngle;
    }

    void incrementAngle(float dTheta)
    {
        //obrni sve da se krece u suprotnom smjeru
        _angle -= dTheta;

        if (_angle < -2 * math::FLT_PI)
            _angle += 2 * math::FLT_PI;
        reDraw();
    }

    void setVisualEffects(bool bUseCustomColor, td::ColorID jetClrID, td::ColorID etfColorID)
    {
        
        reDraw();
    }

    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
    }

    void onDraw(const gui::Rect& rect) override
    {
        gui::Transformation zoomTr;
        double zoom = 1.; //experiment with this and see impact
        double scrollX = 0;
        double scrollY = 0;
        zoomTr.translate(scrollX, scrollY);
        zoomTr.scale(zoom);
        zoomTr.appendToContext(); //this is transformationfor all objects
        gui::Point cp(_size.width / 2, _size.height / 2);

        double rotationDeg1 = -_angle;
        double rotationDeg2 = 0;
        double rotationDeg3 = _angle;


        gui::Point r3(_size.width * 0.25, _size.height * 0.75), r4(_size.width *0.25, _size.height*0.2), r5(_size.width *0.8, _size.height*0.75);
        gui::Point r34[] = { r3,r4 }, r35[] = { r3,r5 };
        gui::Shape prvi, drugi;
        prvi.createLines(r34, 2, 3);
        drugi.createLines(r35, 2, 3);

        //StaviTacke(524, 81, 12,2);
        for (int i = 0; i < _xOsa.size(); i++) {
            td::String broj = std::to_string(_xOsa[i]);
            gui::DrawableString str(broj);

            str.draw({ _size.width * (0.25+0.5*i/(_xOsa.size()-1)), _size.height * 0.8}, gui::Font::ID::ViewNormal, td::ColorID::Blue);

            
            if (i == _xOsa.size() - 1) {
                gui::Point p(_size.width * 0.75, _size.height * 0.75);
                gui::Circle c(p,5);
                gui::Shape krug;
                krug.createCircle(c);
                krug.drawFill(td::ColorID::DarkBlue);
                continue;
            }
            if (i == 0) continue;
            gui::Point p1(_size.width * (0.25 + 0.5 * i / (_xOsa.size() - 1)), _size.height * 0.74),p2(_size.width * (0.25 + 0.5 * i / (_xOsa.size() - 1)), _size.height * 0.76);
            gui::Point p12[] = { p1,p2 };
            gui::Shape crta;
            crta.createLines(p12, 2);
            crta.drawWire(td::ColorID::DarkBlue);

        }

        for (int i = 0; i < _yOsa.size(); i++) {
            td::String broj = std::to_string(_yOsa[i]);
            gui::DrawableString str(broj);

            str.draw({_size.width*0.2, _size.height * (0.75 - 0.5 * i / (_yOsa.size() - 1))}, gui::Font::ID::ViewNormal, td::ColorID::Blue);


            if (i == _yOsa.size() - 1) {
                gui::Point p(_size.width * 0.25, _size.height * 0.25);
                gui::Circle c(p, 5);
                gui::Shape krug;
                krug.createCircle(c);
                krug.drawFill(td::ColorID::DarkBlue);
                continue;
            }
            if (i == 0) continue;
            gui::Point p1(_size.width * 0.245, _size.height * (0.75 - 0.5 * i / (_yOsa.size() - 1))), p2(_size.width * 0.255, _size.height * (0.75 - 0.5 * i / (_yOsa.size() - 1)));
            gui::Point p12[] = { p1,p2 };
            gui::Shape crta;
            crta.createLines(p12, 2);
            crta.drawWire(td::ColorID::DarkBlue);
            
            //_osa.draw(rec, gui::Font::ID::SystemLargerBold, td::ColorID::Black, td::TextAlignment::Left, td::TextEllipsize::End);

        }

        {
            gui::DrawableString _osa1("Proteklo vrijeme [dani]");

            gui::Transformation::saveContext();
            gui::Transformation tr;

            tr.translate(0, 0);
            tr.scale(1);
            if (_size.width < 650) {
                tr.translate(-_size.width * 0.06, 0);
                if (_size.width < 600) {
                    tr.translate(-25, 0);
                }
            }
            if (_size.height < 300) {
                tr.translate(0, 5);
            }
            tr.setToContext();

            _osa1.draw({ _size.width * 0.45,_size.height * 0.85 }, gui::Font::ID::SystemLargerBold, td::ColorID::Black);
            gui::Transformation::restoreContext();
        }

        gui::DrawableString _osa("Preostali posao [težina]");

        gui::Rect rec(_size.width * 0.15, _size.height * 0.2, _size.width * 0.16, _size.height * 0.5);

        gui::Rect rec1(0, 0, 150, 175);

        {
            gui::Transformation::saveContext();
            gui::Transformation tr;
            tr.translate(_size.width * 0.11, _size.height * 0.65);
            if (_size.height < 300) {
                tr.translate(0, _size.height * 0.1);
                //rec1.toZero();
                if (_size.height < 250) {
                    tr.translate(-15, 0);
                    rec1.setLeftAndWidth(0, 100);
                    if (_size.width < 650) {
                        tr.translate(-30, 0);
                    }
                }
            }
            else if (_size.height > 575) {
                rec1.setLeftAndWidth(0, 250);
                if (_size.width < 700) {
                    tr.translate(10, 0);
                }
                else if (_size.width > 700) {
                    tr.translate(_size.width*0.03, 0);
                    if (_size.width > 850) {
                        tr.translate(_size.width * 0.03, 0);
                    }
                }
            }
            else if (_size.height > 500) {
                tr.translate(0, -_size.height * 0.05);
            }

            if (_size.height < 575 && _size.height>250 && _size.width < 700) {
                tr.translate(-15, 0);
            }
            else if (_size.height < 575 && _size.height>250 && _size.width > 700) {
                tr.translate(_size.width * 0.01, 0);
                if (_size.width > 910) {
                    tr.translate(_size.width * 0.03, 0);
                }
            }
            else if (_size.height <= 250 && _size.width > 925) {
                tr.translate( _size.width * 0.03, 0);
            }
            
            //tr.translate(792, 364);
            tr.scale(1);
            tr.rotateDeg(-90);
            tr.setToContext();
            //_osa.draw({ 0,0 }, gui::Font::ID::SystemLargerBold, td::ColorID::Black);
            _osa.draw(rec1, gui::Font::ID::SystemLargerBold, td::ColorID::Black, td::TextAlignment::Center, td::TextEllipsize::End);
            //_osa.draw({ _size.width * 0.15,_size.height * 0.5 }, gui::Font::ID::SystemLargerBold, td::ColorID::Black);
            gui::Transformation::restoreContext(); 
            
        }

        {
            double pozicija = _brojDana / 1. / _ukupDana;

            td::String broj = std::to_string(_brojDana);
            gui::DrawableString str(broj);

            str.draw({ _size.width * (0.25+0.5*pozicija), _size.height * 0.8 }, gui::Font::ID::ViewNormal, td::ColorID::Red);

            gui::Point p1(_size.width * (0.25 + 0.5 * pozicija), _size.height * 0.74), p2(_size.width * (0.25 + 0.5 * pozicija), _size.height * 0.76);
            gui::Point p12[] = { p1,p2 };
            gui::Shape crta;
            crta.createLines(p12, 2);
            crta.drawWire(td::ColorID::DarkMagenta);
        }

        {
            double pozicija = _sadTez / 1. / _ukupTez;

            td::String broj = std::to_string(_sadTez);
            gui::DrawableString str(broj);

            str.draw({ _size.width * 0.2, _size.height * (0.75 - 0.5 * pozicija) }, gui::Font::ID::ViewNormal, td::ColorID::Red);

            gui::Point p1(_size.width * 0.245, _size.height * (0.75 - 0.5 * pozicija)), p2(_size.width * 0.255, _size.height * (0.75 - 0.5 * pozicija));
            gui::Point p12[] = { p1,p2 };
            gui::Shape crta;
            crta.createLines(p12, 2);
            crta.drawWire(td::ColorID::DarkMagenta);
        }

        {
            double pozx = _brojDana / 1. / _ukupDana, pozy =_sadTez/1./_ukupTez;
            gui::Point p(_size.width * (0.25+0.5*pozx), _size.height * (0.75-0.5*pozy));
            gui::Circle c(p, 5);
            gui::Shape krug;
            krug.createCircle(c);
            krug.drawFill(td::ColorID::DarkMagenta);

            gui::Point r1(_size.width * 0.25, _size.height * 0.25), r2(_size.width * 0.75, _size.height * 0.75);
            gui::Point rr[] = { r1, p}, rr2[]={p, r2};
            _moje.createLines(rr, 2);

            _moje.drawWire(td::ColorID::Gold);

            gui::Shape _nezavrseni;
            _nezavrseni.createLines(rr2, 2,1, td::LinePattern::Dash);
            _nezavrseni.drawWire(td::ColorID::Khaki);
        }
       
        /*gui::Shape wow;
        gui::Rect rec(_size.width * 0.15, _size.height * 0.35, _size.width * 0.15, _size.height * 0.66);
        //gui::Rect rec(10, 50, 100, 200);
        wow.createRect(rec);
        wow.drawFillAndWire(td::ColorID::Pink, td::ColorID::DarkMagenta);*/
        //_linija.drawWire(td::ColorID::OrangeRed);
        
        prvi.drawWire(td::ColorID::Chocolate);
        drugi.drawWire(td::ColorID::Chocolate);
    }
};
