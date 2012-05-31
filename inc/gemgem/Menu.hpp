/*
Copyright 2011 JohannC

This file is part of GEMGEM.

GEMGEM is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

GEMGEM is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
GEMGEM.  If not, see http://www.gnu.org/licenses/
*/

#ifndef GEMGEM_MENU_HPP_
#define GEMGEM_MENU_HPP_

#include <splashouille/Object.hpp>

namespace splashouille
{
class Animation;
class Engine;
}

namespace gemgem
{
class Gemgem;

/**
 * Class menu handles all the menu interactions
 */
class Menu : public splashouille::Object::Listener
{
private:
    Gemgem *                    gemgem;                     // The gemgem parent
    splashouille::Animation *   a_menu;                     // The menu animation
    splashouille::Object *      o_audio[5];                 // The audio button

    /** Change the option selection */
    void updateOption(int _option, int _value, int _timeStampInMilliSeconds);

public:
    Menu(Gemgem * _gemgem):gemgem(_gemgem) {}

    /** Initialize the menu class handler */
    bool    init(splashouille::Engine * _engine);

    bool    onMouseClick (int _ts, splashouille::Object * _object, int _x, int _y, int _state, bool _release);
};

}


#endif
