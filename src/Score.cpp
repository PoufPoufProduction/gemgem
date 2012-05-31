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
#include <splashouille/Image.hpp>

#include <gemgem/Main.hpp>

#include <libconfig.h++>

using namespace gemgem;

const int Score::ascii[] = {   'A','B','C','D','E','F','G','H','I','J','K','L','M',
                                'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};


/**
 * Update the best scores overview
 * @param _root is the root of the settings configuration file
 */
void Score::update(libconfig::Setting & _root)
{
    // UPDATE THE 10 BEST SCORES (5 FOR TYPE-A + 5 FOR TYPE-B)
    for (int i=0; i<10; i++)
    {
        libconfig::Setting &    setting = _root["score"][i];
        std::string             player = setting["player"];
        int                     score = setting["score"];

        // UPDATE THE PLAYER NAME
        for (int j=0; j<3; j++)
        {
            char vId[32];
            snprintf(vId, 32, "player%d%d", i, j);
            splashouille::Image* l = gemgem->getEngine()->getLibrary()->getImageById(vId);
            if (l) { l->setTileIndex(player[j]); }
        }

        // UPDATE THE SCORE VALUE
        for (int j=3; j>=0; j--)
        {
            char vId[32];
            snprintf(vId, 32, "score%d%d", i, j);
            splashouille::Image* l = gemgem->getEngine()->getLibrary()->getImageById(vId);
            if (l) { l->setTileIndex(score?('0'+score%10):' '); }
            score = score/10;
        }
    }
}

void Score::updateName(int _position, const int * _name)
{
    for (int i=0; i<3; i++)
    {
        char vId[32];
        snprintf(vId, 32, "player%d", i);
        splashouille::Image* l = gemgem->getEngine()->getLibrary()->getImageById(vId);
        if (l) { l->setTileIndex( (_position==i)?_name[i]:ascii[_name[i]]); }
    }
}

/** Update the splashouille objects */
bool Score::openBest(int _score)
{
    int best = gemgem->getSettings().getRoot()["score"][gemgem->getGameType()?9:4]["score"];
    bool ret = (_score>best);

    if (ret)
    {
        a_best->changeTimeline("intro");
        position = 0;
        name[0] = name[1] = name[2] = 0;
        updateName(position, name);

        score = _score;
        for (int i=3; i>=0; i--)
        {
            char vId[32];
            snprintf(vId, 32, "best%d", i);
            splashouille::Image* l = gemgem->getEngine()->getLibrary()->getImageById(vId);
            if (l) { l->setTileIndex(_score?('0'+_score%10):' '); }
            _score = _score/10;
        }
    }

    return ret;
}




