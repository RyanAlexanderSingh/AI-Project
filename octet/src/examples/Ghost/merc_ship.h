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

    const float agro_range = 50.0f;
    const float flee_range = 30.0f;
    const float speed = 2.0f;

    //the different states of the mercs
    enum mercState {TARGETING, FLEEING, WANDERING};
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

    void update(dynarray<scene_node*> civilians, scene_node *player_ship, float angle = 0){
      mercenarySpaceShip.update_agro_circle(ship_node, player_ship, angle);
      state = WANDERING;
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      for (unsigned i = 0; i < civilians.size() && state != TARGETING; ++i){
        //probably not the best way, think about putting this in a struct
        vec3 civilian_position = civilians[i]->get_position();
        vec3 distanceVec = civilian_position - ship_node->get_position();
        //check if its within the range to run away from them
        if ((distanceVec.x() > -agro_range && distanceVec.x() < agro_range)
          && (distanceVec.z() > -agro_range && distanceVec.z() < agro_range)){
          ai.seek(ship_node, civilians[i]);
          state = TARGETING;
        }
        //default behaviour, lets just let it wandering around
        if (state == WANDERING){
          //ai.wander(ship_node, speed);
        }
      }
    }
    ~merc_ship() {
    }
  };
}

#endif
