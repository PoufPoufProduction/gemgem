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
#include <splashouille/Image.hpp>

#include <gemgem/Main.hpp>

#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <list>

using namespace gemgem;


const int Game::sidekick[] =   {  0, 0, 1, 0, 0, 1, -1, 0, 0, -1 };

/**
 * Initialize the game handler
 * @return true if every objects are found
 */
bool Game::init(splashouille::Engine * _engine)
{
    // GET THE INTERACTIVE OBJECT
    a_effects   = _engine->getLibrary()->getAnimationById("effects");
    a_grid      = _engine->getLibrary()->getAnimationById("grid");
    a_backgrid  = _engine->getLibrary()->getAnimationById("backgrid");
    a_score     = _engine->getLibrary()->getAnimationById("gamescore");
    for (int i=0; i<8; i++) for (int j=0; j<8; j++) o_gems[i][j] = 0;

    o_score[0]  = _engine->getLibrary()->getObjectById("score0");
    o_score[1]  = _engine->getLibrary()->getObjectById("score1");
    o_score[2]  = _engine->getLibrary()->getObjectById("score2");
    o_score[3]  = _engine->getLibrary()->getObjectById("score3");

    o_player[0] = _engine->getLibrary()->getImageById("player0");
    o_player[1] = _engine->getLibrary()->getImageById("player1");
    o_player[2] = _engine->getLibrary()->getImageById("player2");

    o_timebar   = _engine->getLibrary()->getObjectById("timebar");

    _engine->getLibrary()->getObjectById("up0")->setListener(this);
    _engine->getLibrary()->getObjectById("up1")->setListener(this);
    _engine->getLibrary()->getObjectById("up2")->setListener(this);
    _engine->getLibrary()->getObjectById("down0")->setListener(this);
    _engine->getLibrary()->getObjectById("down1")->setListener(this);
    _engine->getLibrary()->getObjectById("down2")->setListener(this);
    _engine->getLibrary()->getObjectById("valid")->setListener(this);

    counter = 0;

    return true;
}


/**
 * Update the score and the number of lines values
 * @param _score is the current score
 */
void Game::updateScore(int _score)
{
    // THE SCORE
    for (int i=0; i<4; i++)
    {
        if (_score || !i)
        {
            o_score[3-i]->getStyle()->setDisplay(true);
            o_score[3-i]->getStyle()->setPosition(32*(_score%10), 0);
            _score=_score/10;
        }
        else
        {
            o_score[3-i]->getStyle()->setDisplay(false);
        }
    }
}

/**
 * Open the the score panel for asking player's name
 */
void Game::openScore()
{
    a_score->changeTimeline("intro");

    int s = score;
    for (int i=3; i>=0; i--)
    {
        char vId[32];
        snprintf(vId, 32, "best%d", i);
        splashouille::Image* l = gemgem->getEngine()->getLibrary()->getImageById(vId);
        if (l) { l->setTileIndex(s?('0'+s%10):' '); }
        s = s/10;
    }
}

/**
 * Game is over if time bar is full
 * @param _ts is the current timestamp
 */
void Game::checkTime(int _ts UNUSED)
{
    if (o_timebar->getFashion()->getCurrent()->getWidth() == 326)
    {
        a_effects->changeTimeline("end");
    }
}

/**
 * Check the grid
 * @return true if there are moves still available
 */
bool Game::checkGrid()
{
    int             values[8][8];
    int             tmp;
    bool            ret = false;
    for (int i=0; i<8; i++) for (int j=0; j<8; j++)
    {   values[i][j] = o_gems[i][j]->getState()%10; toDelete[i][j] = false; }

    for (int i=0; i<8 && !ret; i++) for (int j=0; j<8 && !ret; j++)
    {
        if (i<7)
        {
            // SWAP i and i+1
            tmp = values[i][j]; values[i][j] = values[i+1][j]; values[i+1][j] = tmp;
            for (int ii=-2; ii<=1; ii++)
                ret|=((i+ii>=0) && (i+ii+2<8) && (values[i+ii][j]==values[i+ii+1][j]) && (values[i+ii][j]==values[i+ii+2][j]));
            for (int ii=0; ii<=1; ii++) for (int jj=-2; jj<=0; jj++)
                ret|=((j+jj>=0) && (j+jj+2<8) && (values[i+ii][j+jj]==values[i+ii][j+jj+1]) && (values[i+ii][j+jj]==values[i+ii][j+jj+2]));
            tmp = values[i][j]; values[i][j] = values[i+1][j]; values[i+1][j] = tmp;
        }

        if (j<7 && !ret)
        {
            // SWAP j and j+1
            tmp = values[i][j]; values[i][j] = values[i][j+1]; values[i][j+1] = tmp;
            for (int jj=-2; jj<=1; jj++)
                ret|=((j+jj>=0) && (j+jj+2<8) && (values[i][j+jj]==values[i][j+jj+1]) && (values[i][j+jj]==values[i][j+jj+2]));
            for (int jj=0; jj<=1; jj++) for (int ii=-2; ii<=0; ii++)
                ret|=((i+ii>=0) && (i+ii+2<8) && (values[i+ii][j+jj]==values[i+ii+1][j+jj]) && (values[i+ii][j+jj]==values[i+ii+2][j+jj]));
            tmp = values[i][j]; values[i][j] = values[i][j+1]; values[i][j+1] = tmp;
        }
    }
    return ret;
}


/**
 * Look for the gems to delete
 * @param _swapCurrent is true then current gems are swapped before deletion
 * @return the number of gems to delete
 */
int Game::computeDelete(bool _swapCurrent)
{
    int             ret = 0;
    int             values[8][8];
    int             offset;
    int             count;

    // BUILD THE VALUES ARRAY
    for (int i=0; i<8; i++) for (int j=0; j<8; j++)
    {   values[i][j] = o_gems[i][j]->getState()%10; toDelete[i][j] = false; }
    if (_swapCurrent)
    {
        int v = values[current[0]][current[1]];
        values[current[0]][current[1]] = values[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]];
        values[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]] = v;
    }

    // LOOK FOR GEMS TO DELETE
    for (int i=0; i<8; i++) for (int j=0; j<8; j++)
    {
        // HORIZONTAL
        offset = count = 0;
        do { offset++; count++; } while ((i+count<8) && (values[i][j]==values[i+count][j]));
        if (count>2) { for (int k=0; k<count; k++) { if (!toDelete[i+k][j]) { ret++; toDelete[i+k][j] = true; } } }

        // VERTIVAL
        offset = count = 0;
        do { offset++; count++; } while ((j+count<8) && (values[i][j]==values[i][j+count]));
        if (count>2) { for (int k=0; k<count; k++) { if (!toDelete[i][j+k]) { ret++; toDelete[i][j+k] = true; } } }
    }

    return ret;
}


/**
 * Launch animation for deleted gems
 * @param _timeStampInMilliSeconds is the current timestamp
 */
void Game::deleteGems(int _timeStampInMilliSeconds)
{
    static int  counterStars = 0;
    char        msg[128];

    for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (toDelete[i][j])
    {
        // DELETE FASHION FOR DELETED GEMS
        o_gems[i][j]->changeFashion("delete");
        score++;
        if (score>9999) { score = 9999; }

        // ADD ANIMATION
        int     value = rand()%3;
        snprintf(msg, 128, "star%d", counterStars++);
        splashouille::Object * o = gemgem->getEngine()->getLibrary()->copyObject("_stars", msg);
        o->getStyle()->setTop(j*58-3);
        o->getStyle()->setLeft(i*58-3);
        snprintf(msg, 128, "anim%d", value);
        o->changeFashion(msg);
        stars.push_back(o);
        a_backgrid->getCrowd()->insertObject(_timeStampInMilliSeconds, o);
    }
    updateScore(score);
}

/**
 * Remove the deleted gems from crowd and library. Remove the animated stars too
 */
void Game::purgeGems()
{
    // REMOVE GEMS
    for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (toDelete[i][j])
    {
        a_grid->getCrowd()->dropObject(o_gems[i][j]);
        gemgem->getEngine()->getLibrary()->deleteObject(o_gems[i][j]);
        o_gems[i][j]=0;
    }

    // REMOVE STARS
    while (!stars.empty())
    {
        a_backgrid->getCrowd()->dropObject(stars.front());
        gemgem->getEngine()->getLibrary()->deleteObject(stars.front());
        stars.pop_front();
    }
}

/**
 * Make the gems drop (if hole below)
 * @param _ts is the current timestamp
 */
void Game::dropGems(int _ts)
{
    char fashion[128];

    // DELETE THE GEMS DEFINITELY
    purgeGems();

    // DROP GEMS AND CREATE NEW ONES
    for (int i=0; i<8; i++)
    {
        int nbMissingGems = 0;

        // DROP GEMS
        for (int j=7; j>=0; j--)
        {
            if (o_gems[i][j])
            {
                if (nbMissingGems)
                {
                    splashouille::Object * o = o_gems[i][j];
                    o_gems[i][j+nbMissingGems] = o;
                    o_gems[i][j] = 0;
                    o->getStyle()->setTop((j+nbMissingGems)*58);

                    o->setState(o->getState()%10+i*10+(j+nbMissingGems)*100);

                    snprintf(fashion, 128, "mouseintro%d", nbMissingGems);
                    o->changeFashion(fashion, true);
                }
            }
            else
            {
                nbMissingGems++;
            }
        }

        // CREATE GEMS
        snprintf(fashion, 128, "mouseintro%d", nbMissingGems);
        for (int j=0; j<nbMissingGems; j++)
        {
            o_gems[i][j] = createGem(i, j);
            a_grid->getCrowd()->insertObject(_ts, o_gems[i][j]);
            o_gems[i][j]->changeFashion(fashion);
        }
    }

    a_effects->changeTimeline("waitdrop");
}

/**
 * Not interactive step: delete gems, check if game over or continue
 * @param _timeStampInMilliSeconds is the timestamp
 */
void Game::next(int _timeStampInMilliSeconds)
{
    if (computeDelete(false))
    {
        // DELETE SOME REMAINING GEMS
        deleteGems(_timeStampInMilliSeconds);
        a_effects->changeTimeline("waitdelete");
    }
    else
    {
        if (checkGrid())
        {
            // CONTINUE THE GAME
            gemgem->getEngine()->getMouse()->changeFashion("active");
            a_effects->changeTimeline("dragmode");

            // RELAUNCH THE TIMEBAR ANIMATION REGARDING THE LEVEL
            if (gemgem->getGameType())
            {
                int valtmp = score/500+1;
                if (valtmp>10) { valtmp=10; }
                char strtmp[128];
                snprintf(strtmp, 128, "level%d", valtmp);
                o_timebar->changeFashion(strtmp, true);
            }
        }
        else
        {
            // IF THERE IS NO MORE AVAILABLE MOVE
            if (gemgem->getGameType())
            {
                // REBUILD THE GRID IN TIME MODE
                deleteGrid();
                createGrid(_timeStampInMilliSeconds);
                a_effects->changeTimeline("waitdragmode");
            }
            else
            {
                // GAME OVER IN CLASSIC MODE
                a_effects->changeTimeline("end");
            }
        }
    }
}

/**
 * Callback on SDL event
 * @param _event is the incoming SDL_Event
 * @return true if the event is consumed
 */
bool Game::onEvent(SDL_Event & _event, int _timeStampInMilliSeconds)
{
    // RELEASE THE BUTTON
    if (_event.type == SDL_MOUSEBUTTONUP && isGrabbing)
    {
        isGrabbing = false;
        splashouille::Object *  o = o_gems[current[0]][current[1]];
        float                   move = std::abs(o->getStyle()->getRelativeTop())+abs(o->getStyle()->getRelativeLeft());
        bool                    performSwap = ((side!=none) && (move>50) && (computeDelete(true)));

        if (performSwap)
        {
            if (gemgem->getGameType()) { o_timebar->changeFashion("none", true); }

            // SWAP AND DELETE THE GEMS
            o->changeFashion("mouseout");
            splashouille::Object *  o2 = o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]];
            o_gems[current[0]][current[1]] = o2;
            o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]] = o;
            o->getStyle()->setRelativeTop(0);   o->getStyle()->setRelativeLeft(0);
            o2->getStyle()->setRelativeTop(0);  o2->getStyle()->setRelativeLeft(0);
            o->getStyle()->setLeft((current[0]+sidekick[side*2])*58);
            o->getStyle()->setTop((current[1]+sidekick[side*2+1])*58);
            o2->getStyle()->setLeft(current[0]*58);
            o2->getStyle()->setTop(current[1]*58);
            o->setState(o->getState()%10 + (current[0]+sidekick[side*2]) * 10 + (current[1]+sidekick[side*2+1]) * 100);
            o2->setState(o2->getState()%10 + (current[0]) * 10 + (current[1]) * 100);

            deleteGems(_timeStampInMilliSeconds);

            gemgem->getEngine()->getMouse()->changeFashion("display");
            a_effects->changeTimeline("waitdelete");
        }
        else
        {
            // CENTER THE MOVED GEMS
            o->changeFashion("mousecenter", true);
            if (side!=none)
            {
                o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]]->changeFashion("mousecenter", true);
            }

            gemgem->getEngine()->getMouse()->changeFashion("intro");
        }

    }
    // MOVE THE GEMS
    else if (_event.type == SDL_MOUSEMOTION && isGrabbing)
    {
        int x = _event.button.x-mouse[0];
        int y = _event.button.y-mouse[1];

        splashouille::Object * o = o_gems[current[0]][current[1]];
        int s = o->getStyle()->getWidth();

        if (std::abs(x)>=std::abs(y))
        {
            // HORIZONTAL GRABBING
            if (side==top || side==bottom)
            {
                o->getStyle()->setRelativeTop(0);
                o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]]->getStyle()->setRelativeTop(0);
            }

            float move = std::pow((float) x/s,5)*s;

            if (move>0)                     { side = right; }
            if (move<0)                     { side = left; }
            if (move<-s)                    { move = -s; }
            if (move>s)                     { move = s; }
            if (current[0]==0 && move<0)
            {
                move = 0; side = none;
                o_gems[current[0]+sidekick[right*2]][current[1]+sidekick[right*2+1]]->getStyle()->setRelativeLeft(0);
            }
            if (current[0]==7 && move>0)
            {
                move = 0; side = none;
                o_gems[current[0]+sidekick[left*2]][current[1]+sidekick[left*2+1]]->getStyle()->setRelativeLeft(0);
            }
            o->getStyle()->setRelativeLeft(move);

            if (side==right || side==left)
            { o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]]->getStyle()->setRelativeLeft(-move); }

        }
        else
        {
            // VERTICAL GRABBING
            if (side==right || side==left)
            {
                o->getStyle()->setRelativeLeft(0);
                o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]]->getStyle()->setRelativeLeft(0);
            }

            float move = std::pow((float) y/s,5)*s;
            if (move>0)                     { side = bottom; }
            if (move<0)                     { side = top; }
            if (move<-s)                    { move = -s; }
            if (move>s)                     { move = s; }
            if (current[1]==0 && move<0)
            {
                move = 0; side = none;
                o_gems[current[0]+sidekick[bottom*2]][current[1]+sidekick[bottom*2+1]]->getStyle()->setRelativeTop(0);
            }
            if (current[1]==7 && move>0)
            {
                move = 0; side = none;
                o_gems[current[0]+sidekick[top*2]][current[1]+sidekick[top*2+1]]->getStyle()->setRelativeTop(0);
            }
            o->getStyle()->setRelativeTop(move);

            if (side==top || side==bottom)
            { o_gems[current[0]+sidekick[side*2]][current[1]+sidekick[side*2+1]]->getStyle()->setRelativeTop(-move); }
        }
    }

    return true;
}

/**
 * Click callback
 * @param ... a lot of parameters
 */
bool Game::onMouseClick (int _ts UNUSED, splashouille::Object * _object, int _x, int _y, int _state UNUSED, bool _release)
{
    // ONLY THE BUTTON DOWN IS TREATED
    if (!_release)
    {
        // CLICK ON A GEM
        if (gemgem->getEngine()->getMouse()->getStyle()->getUser()==0 && (_object->getState()<=777) )
        {
            gemgem->getEngine()->getMouse()->changeFashion("outro");
            isGrabbing = true;
            side = none;
            current[0] = (_object->getState()/10)%10;
            current[1] = (_object->getState()/100);
            mouse[0] = _x;
            mouse[1] = _y;
        }
        else
        // CLICK ON A BEST PLAYER LETTER
        if ( gemgem->getEngine()->getMouse()->getStyle()->getUser()==1 && (_object->getState()>=1000) )
        {
            o_player[((_object->getState()-1000)/10)%3]->setTileIndex(
                ( o_player[((_object->getState()-1000)/10)%3]->getTileIndex()+ (_object->getState()%10) - 1 -'A' + 26)%26 + 'A' );

        }
        else
        // CLICK ON VALID
        if ( gemgem->getEngine()->getMouse()->getStyle()->getUser()==1 && (_object->getState()==999) )
        {
            int i = gemgem->getGameType()?9:4;
            while(i%5)
            {
                int up = gemgem->getSettings().getRoot()["score"][i-1]["score"];
                if (up<score)
                {
                    gemgem->getSettings().getRoot()["score"][i]["score"] = (int)gemgem->getSettings().getRoot()["score"][i-1]["score"];
                    gemgem->getSettings().getRoot()["score"][i]["player"] = (const std::string&) gemgem->getSettings().getRoot()["score"][i-1]["player"];
                }
                else break;
                i--;
            };
            char nameStr[4];
            snprintf(nameStr, 4, "%c%c%c", o_player[0]->getTileIndex(), o_player[1]->getTileIndex(), o_player[2]->getTileIndex());

            gemgem->getSettings().getRoot()["score"][i]["score"] = score;
            gemgem->getSettings().getRoot()["score"][i]["player"] = nameStr;

            a_effects->changeTimeline("quit");
        }
    }
    return false;
}

/**
 * Gem constructor (state attribute is used for storing position and value)
 * @param _i x-axis coordinate
 * @param _j y-axis coordinate
 * @return newly allocated object
 */
splashouille::Object * Game::createGem(int _i, int _j)
{
    char    gemId[128];
    int     value = rand()%7;

    snprintf(gemId, 128, "gem%d", counter++);
    splashouille::Object * o = gemgem->getEngine()->getLibrary()->copyObject("_gem", gemId);
    o->getStyle()->setPositionY(58*value);
    o->setState(value+_i*10+_j*100);
    o->getStyle()->setTop(_j*58);
    o->getStyle()->setLeft(_i*58);
    o->setListener(this);

    return o;
}

/**
 * Regarding the score, open the name asking prompt or quit the game
 * @param _ts is the timestamp
 */
void Game::end(int _ts UNUSED)
{
    int best = gemgem->getSettings().getRoot()["score"][gemgem->getGameType()?9:4]["score"];
    bool ret = (score>best);

    if (ret)
    {
        openScore();
    }
    else 
    {
        a_effects->changeTimeline("quit");
    }
}

/**
 * Delete the grid and quit the game
 * @param _ts is the timestamp
 */
void Game::quit(int _ts UNUSED)
{
    deleteGrid();
    gemgem->getEngine()->changeTimeline("t_menu");
}

/**
 * Delete the grid gems from crowd and library
 */
void Game::deleteGrid()
{
    for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (o_gems[i][j])
    {
        a_grid->getCrowd()->dropObject(o_gems[i][j]);
        gemgem->getEngine()->getLibrary()->deleteObject(o_gems[i][j]);
        o_gems[i][j]=0;
    }
}

/**
 * Create a new grid
 * @param _ts is the timestamp
 */
void Game::createGrid(int _ts)
{
    int nbDeleted = 0;
    do
    {
        for (int i=0; i<8; i++) for (int j=0; j<8; j++) if (!o_gems[i][j])
        {
            o_gems[i][j] = createGem(i, j);
            a_grid->getCrowd()->insertObject(_ts, o_gems[i][j]);
            o_gems[i][j]->changeFashion("mouseintro");
        }
        if ((nbDeleted=computeDelete())) { purgeGems(); }
    } while (nbDeleted);
}

/**
 * Launch the game
 */
void Game::run(int _ts)
{
    srand(time(NULL));

    for (int i=0; i<8; i++) for (int j=0; j<8; j++) { delete o_gems[i][j]; }
    createGrid(_ts);

    isGrabbing  = false;
    side        = none;
    score       = 0;
    updateScore(score);
    a_effects->changeTimeline("waitdragmode");
}

