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

    ships speed_ship;
    ai_behaviours ai;

    ref<scene_node> ship_node;

    const float agro_range = 45.0f;
    const float speed = 2.0f;

  public:
    merc_ship(){}

    scene_node *return_ship_node(){
      return ship_node;
    }

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      speed_ship.init(the_app, app_scene);
      init_seek_enemy();
    }

    //create a random seek enemy 
    void init_seek_enemy(){
      speed_ship.create_merc_ship();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    void update(dynarray<scene_node*> civilians, scene_node *player_ship){
      for (int i = 0; i < civilians.size(); ++i){
        //probably not the best way, think about putting this in a struct
        vec3 enemy_position = civilians[i]->get_position();
        vec3 facingVec = enemy_position - ship_node->get_position();
        //check if its within the range to run away from them
        if ((facingVec.x() > -agro_range && facingVec.x() < agro_range)
          && (facingVec.z() > -agro_range && facingVec.z() < agro_range)){
          ai.shoot(ship_node, facingVec);
        }
        else{
          ai.wander(ship_node, speed);
        }
      }
    }
    ~merc_ship() {
    }
  };
}

#endif
