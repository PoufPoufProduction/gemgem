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

#include <splashouille/Defines.hpp>
#include <splashouille/Engine.hpp>
#include <splashouille/Library.hpp>
#include <splashouille/Style.hpp>
#include <splashouille/Fashion.hpp>
#include <splashouille/Crowd.hpp>
#include <splashouille/Sound.hpp>

#include <gemgem/Main.hpp>

#include <iostream>

using namespace gemgem;

/**
 * Initialize the menu  handler
 * @return true if every objects are found (todo)
 */
bool Menu::init(splashouille::Engine * _engine)
{
    a_menu      = _engine->getLibrary()->getAnimationById("a_menu");
    o_audio[0]  = _engine->getLibrary()->getObjectById("audio0");
    o_audio[1]  = _engine->getLibrary()->getObjectById("audio1");
    o_audio[2]  = _engine->getLibrary()->getObjectById("audio2");
    o_audio[3]  = _engine->getLibrary()->getObjectById("audio3");
    o_audio[4]  = _engine->getLibrary()->getObjectById("audio4");

    _engine->getLibrary()->getObjectById("play")->setListener(this);
    _engine->getLibrary()->getObjectById("options")->setListener(this);
    _engine->getLibrary()->getObjectById("credit")->setListener(this);
    _engine->getLibrary()->getObjectById("scores")->setListener(this);
    _engine->getLibrary()->getObjectById("quit")->setListener(this);
    _engine->getLibrary()->getObjectById("credits")->setListener(this);
    _engine->getLibrary()->getObjectById("a_menu_best")->setListener(this);
    _engine->getLibrary()->getObjectById("back14")->setListener(this);
    _engine->getLibrary()->getObjectById("back2")->setListener(this);
    _engine->getLibrary()->getObjectById("classic")->setListener(this);
    _engine->getLibrary()->getObjectById("time")->setListener(this);

    for (int i=0; i<5; i++) { o_audio[i]->setListener(this); }

    updateOption(0, gemgem->getSettings().getRoot()["audio"], 0);

    return true;
}

/**
 *
 */
bool Menu::onMouseClick (int _ts, splashouille::Object * _object, int _x UNUSED, int _y UNUSED, int _state UNUSED, bool _release)
{
    if (_release)
    {
        switch(_object->getState())
        {
            case 1  : gemgem->getEngine()->changeTimeline("t_level1ToLevel2"); break;
            case 2  : gemgem->getScore().update(gemgem->getSettings().getRoot());
                      gemgem->getEngine()->changeTimeline("t_level1ToBest"); break;
            case 3  : gemgem->getEngine()->changeTimeline("t_level1ToCredits"); break;
            case 4  : gemgem->getEngine()->changeTimeline("t_level1ToLevel14"); break;
            case 5  : gemgem->getEngine()->changeTimeline("t_level1ToEnd"); break;
            case 6  : gemgem->getEngine()->changeTimeline("t_bestToLevel1"); break;
            case 7  : gemgem->getEngine()->changeTimeline("t_creditsToLevel1"); break;
            case 8  : gemgem->getEngine()->changeTimeline("t_level14ToLevel1"); break;
            case 9  : gemgem->getEngine()->changeTimeline("t_level2ToLevel1"); break;
            case 10 : case 11 : case 12 : case 13 : case 14 : updateOption(0, _object->getState()-10, _ts); break;
            case 15 : gemgem->getEngine()->getLibrary()->getObjectById("classic")->changeFashion("selected");
                      gemgem->setGameType(false);
                      gemgem->getEngine()->changeTimeline("t_level2ToGame"); break;
            case 16 : gemgem->getEngine()->getLibrary()->getObjectById("time")->changeFashion("selected");
                      gemgem->setGameType(true);
                      gemgem->getEngine()->changeTimeline("t_level2ToGame"); break;
        }
    }

    return false;
}

/**
 * Change the option selection
 */
void Menu::updateOption(int _option, int _value, int _timeStampInMilliSeconds UNUSED)
{
    if (_option<1)
    {
        int a = gemgem->getSettings().getRoot()["audio"];

        if (_value>=0)
        {
            if (a!=_value) { o_audio[a]->changeFashion("mouseout");}
            a = _value;
        }

        splashouille::Sound::changeVolume((float)a/4);
        o_audio[a]->changeFashion("selected");
        gemgem->getSettings().getRoot()["audio"] = a;

        gemgem->getEngine()->getLibrary()->getObjectById("menusignes")->changeFashion(a?"sound":"nosound");
    }
}




