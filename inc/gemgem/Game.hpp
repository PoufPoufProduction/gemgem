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

#ifndef GEMGEM_GAME_HPP_
#define GEMGEM_GAME_HPP_

#include <splashouille/Object.hpp>
#include <list>

namespace splashouille
{
class Animation;
class Engine;
}

namespace gemgem
{
class Gemgem;

/**
 * Class Game handles all the game interactions
 */
class Game : public splashouille::Object::Listener
{
private:
    enum sideEnum { none = 0, right = 1, bottom = 2, left = 3, top = 4 };
    const static int            sidekick[];


    Gemgem *                            gemgem;                     // The gemgem parent
    splashouille::Animation *           a_effects;                  // The effects animation
    splashouille::Animation *           a_grid;                     // The gems grid
    splashouille::Animation *           a_backgrid;                 // The back grid
    splashouille::Animation *           a_score;                    // The best score prompt
    splashouille::Object *              o_score[4];                 // The score values
    splashouille::Object *              o_gems[8][8];               // The gems
    splashouille::Object *              o_timebar;                  // The timebar
    splashouille::Image *               o_player[3];                // The best score player
    bool                                toDelete[8][8];             // The gems to delete
    bool                                isGrabbing;                 // Is the user grabbing a gem
    int                                 current[2];                 // The grab object coordinates
    sideEnum                            side;                       // The sideswap
    int                                 mouse[2];                   // The grabbing origin mouse position
    std::list<splashouille::Object*>    stars;                      // Animated stars
    int                                 score;                      // The score (from 0 to 9999)
    int                                 counter;                    // Internal counter

    void                                purgeGems();
    bool                                checkGrid();
    void                                deleteGrid();
    void                                openScore();
    void                                createGrid(int _ts);
    void                                updateScore(int _score);
    int                                 computeDelete(bool _swapCurrent = false);
    void                                deleteGems(int _timeStampInMilliSeconds);
    splashouille::Object *              createGem(int _i, int _j);

public:
    Game(Gemgem * _gemgem):gemgem(_gemgem) {}

    bool                                init(splashouille::Engine * _engine);
    void                                run(int _ts);
    void                                next(int _ts);
    void                                quit(int _ts);
    void                                end(int _ts);
    void                                checkTime(int _ts);
    void                                dropGems(int _ts);

    bool onEvent(SDL_Event & _event, int _timeStampInMilliSeconds);
    bool onMouseClick(int _ts, splashouille::Object * _object, int _x, int _y, int _state, bool _release);

};

}


#endif
