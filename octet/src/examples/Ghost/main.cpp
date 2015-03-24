////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// Text overlay
//

#define OCTET_BULLET 1


#include "../../octet.h"

#include "ships.h"
#include "ship_controls.h"
#include "ai_behaviours.h"
#include "player.h"
#include "civilian_ship.h"
#include "boss_ship.h"
#include "merc_ship.h"
#include "ghost.h"


int main(int argc, char **argv) {
  // set up the platform.
  octet::app::init_all(argc, argv);

  // our application.
  octet::ghost app(argc, argv);
  app.init();

  // open windows
  octet::app::run_all_apps();
}


