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

#include "ship_controls.h"
#include "ships.h"
#include "player.h"
#include "enemy_boss.h"
#include "enemy_flock.h"
#include "enemy_speed.h"
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


