////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Mercenaries ships - will attack most things, love to attack civilians
//
#ifndef MERC_SHIP_H_INCLUDED
#define MERC_SHIP_H_INCLUDED

#include <random>

namespace octet {

  class merc_ship : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships mercenarySpaceShip;
    ai_behaviours ai;

    ref<scene_node> ship_node;

    const float sq_agro_range = 35.0*35.0f;
    const float sq_flee_range = 35.0*35.0f;
    const float speed = 2.0f;

    //the different states of the mercs
    enum mercState { TARGETING, FLEEING, WANDERING, DEAD };
    mercState state;

  public:
    merc_ship(){}

    scene_node *return_ship_node(){
      return ship_node;
    }

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      mercenarySpaceShip.init(the_app, app_scene);
      ship_node = mercenarySpaceShip.create_merc_ship();
      state = WANDERING; //lets set a default for the mercs
    }

    void update(dynarray<scene_node*> civilians, scene_node *boss, scene_node *player_ship, float angle = 0){
      state = WANDERING;
      float lineWidth = 1.0f;
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      for (unsigned i = 0; i < civilians.size() && state != TARGETING; ++i){
        //probably not the best way, think about putting this in a struct
        vec3 civilian_position = civilians[i]->get_position();
        vec3 distanceVec = civilian_position - ship_node->get_position();
        //check if its within the range to run away from them
        if ((distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_agro_range)){
          state = TARGETING;
          ai.seek(ship_node, civilians[i], 7.0f);
        }
      }
      //the mercs will flee from the boss (perhaps add a randomization on this)
      vec3 distanceVec = boss->get_position() - ship_node->get_position();
      if ((distanceVec.x()*distanceVec.x() + distanceVec.z() * distanceVec.z() < sq_flee_range) && state != TARGETING){
        state = FLEEING;
        ai.flee(ship_node, boss);
      }
      //default behaviour, lets just let it wandering around
      if (state == WANDERING){
        ai.wander(ship_node, speed);
      }
      switch (state){
      case WANDERING:
        lineWidth = 1.0f;
        break;
      case FLEEING:
        lineWidth = 2.0f;
        break;
      case TARGETING:
        lineWidth = 2.0f;
        break;
      }
      mercenarySpaceShip.statusCircle(lineWidth, 20.0f, ship_node, player_ship, angle);
    }

    ~merc_ship() {
    }
  };
}

#endif
