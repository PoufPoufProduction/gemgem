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

#include <gemgem/Main.hpp>

#include <libconfig.h++>

#include <iostream>
#include <iomanip>
#include <getopt.h>


using namespace gemgem;


/**
 * Run the player
 */
void Gemgem::run()
{
    bool rc = true;

    // READ THE SETTINGS CONFIGURATION FILE
    settings.scoreValues[5] = 500;
    settings.scoreValues[6] = 400;
    settings.scoreValues[7] = 250;
    settings.scoreValues[8] = 200;
    settings.scoreValues[9] = 150;
    settings.load();

    // INIT THE SPLASHOUILLE FRAMEWORK
    splashouille::Engine::init();

    // CREATE THE SDL WINDOW
    SDL_Surface * screen = SDL_SetVideoMode(screenSize[0], screenSize[1], screenDepth, SDL_SWSURFACE | SDL_DOUBLEBUF );
    SDL_WM_SetCaption("Gemgem", "Gemgem");

    // DEFINE THE WINDOW ICON
    SDL_Surface*    icon     = SDL_LoadBMP("res/img/icon.bmp");
    if (icon)
    {
        Uint32          colorKey = SDL_MapRGB(icon->format, 255, 0, 255);
        SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorKey);
        SDL_WM_SetIcon(icon, NULL);
    }

    // OPEN AND PARSE THE CONFIGURATION FILE
    libconfig::Config * configuration   = new libconfig::Config();
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));SDL_Flip(screen); 

    try { configuration->readFile("res/conf/gemgem.ini"); }
        catch(libconfig::FileIOException e) { std::cerr<<e.what()<<std::endl; rc = false; }
        catch(libconfig::ParseException  e) { std::cerr<<e.what()<<std::endl; rc = false; }

    // CREATE AND PREPARE THE ANIMATION
    engine = splashouille::Engine::createEngine();
    rc = rc && engine->import(configuration);
    delete configuration;

    if (rc)
    {

        // INITIALIZE THE DIFFERENT COMPONENTS OF GEMGEM
        menu.init(engine);
        game.init(engine);

        //engine->setDebug(true);
        engine->setFPS(60);
        engine->addListener(this);

        engine->run(screen);
    }

    // DELETE THE ENGINE
    splashouille::Engine::deleteEngine(engine);

    // SAVE THE CONFIGURATION
    settings.save();
}

/**
 * Callback called on each frame
 * @param _frame is the frame number from the beginning of the animation
 * @param _timeStampInMilliSeconds is the current timestamp
 */
void Gemgem::onFrame(int _frame UNUSED, int _timeStampInMilliSeconds)
{
    switch(engine->getState())
    {
    case 60: engine->stop(); break;
    case 70: game.run(_timeStampInMilliSeconds); break;
    case 90: game.dropGems(_timeStampInMilliSeconds); break;
    case 80: game.checkTime(_timeStampInMilliSeconds); break;
    case 100: game.next(_timeStampInMilliSeconds); break;
    case 110: game.quit(_timeStampInMilliSeconds); break;
    case 120: game.end(_timeStampInMilliSeconds); break;
    }
}

/**
 * Callback on SDL event
 * @param _event is the incoming SDL_Event
 * @return true if the event is consumed
 */
bool Gemgem::onEvent(SDL_Event & _event, int _timeStampInMilliSeconds)
{
    switch(engine->getState())
    {
    case 80: game.onEvent(_event, _timeStampInMilliSeconds); break;
    }
    return true;
}

/**
 * The player main function
 * @param argc is the number of arguments
 * @param argv is the argument values
 * @return 1
 */
int main()
{
    std::cout<<"Gemgem 2012"<<std::endl;

    // INSTANCE THE GAME ENGINE AND RUN!!!
    Gemgem * game = new Gemgem();
    game->run();

    delete game;

    return 1;

}
