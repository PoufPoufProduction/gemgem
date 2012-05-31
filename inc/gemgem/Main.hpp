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

#ifndef GEMGEM_MAIN_HPP_
#define GEMGEM_MAIN_HPP_

#include <string>
#include <splashouille/Engine.hpp>
#include <SDL.h>

#include <utility/Configuration.hpp>
#include <gemgem/Score.hpp>
#include <gemgem/Menu.hpp>
#include <gemgem/Game.hpp>

namespace gemgem
{

class Gemgem : public splashouille::Engine::Listener
{
private:
    splashouille::Engine *      engine;                         // The splashouille engine

    int                         screenSize[2];                  // The screen size
    int                         screenDepth;                    // The screen depth
    bool                        gameType;                       // The gameType false=normal, true=time

    // -------------- GEMGEM COMPONENTS -----------------------------
    utility::Configuration      settings;                       // The gemgem settings (volume, bestscores, etc.)
    Score                       score;                          // The score handler
    Menu                        menu;                           // The menu handler
    Game                        game;                           // The game handler

public:
    Gemgem():
        engine(0), screenDepth(32), gameType(false), settings("gemgem"),
        score(this), menu(this), game(this)
    {
        screenSize[0] = 480;
        screenSize[1] = 640;
    }

    /** Accessors */
    Score &                     getScore()              { return score; }
    Menu &                      getMenu()               { return menu; }
    Game &                      getGame()               { return game; }
    utility::Configuration &    getSettings()           { return settings; }
    splashouille::Engine *      getEngine() const       { return engine; }
    bool                        getGameType() const     { return gameType; }

    void                        setGameType(bool _g)    { gameType = _g; }

    /**
     * Callback called each second
     * @param _frame is the frame number from the beginning of the animation
     * @param _frameSec is the number of frames played during the last second
     * @param _second is the current second
     */
     void onSecond(int _frame UNUSED, int _frameSec UNUSED, int _second UNUSED) {}

    /**
     * Callback called on each frame
     * @param _frame is the frame number from the beginning of the animation
     * @param _timeStampInMilliSeconds is the current timestamp
     */
    void onFrame(int _frame, int _timeStampInMilliSeconds);

    /**
     * Callback on SDL event
     * @param _event is the incoming SDL_Event
     * @return true if the event is consumed
     */
    bool onEvent(SDL_Event & _event, int _timeStampInMilliSeconds);

    /**
     * Callback on the quit event
     * @return true if the event is consumed
     */
    bool onStop() { return true; }

    /**
     * Run the player
     */
    void run();
};
}

#endif
