////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Speedy ships
//
#ifndef ENEMY_SPEED_H_INCLUDED
#define ENEMY_SPEED_H_INCLUDED

#include <random>

namespace octet {

  class enemy_speed : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships speed_ship;
    ai_behaviours ai;

    ref<scene_node> ship_node;

    const float agro_range = 15.0f;

  public:
    enemy_speed(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      speed_ship.init(app, app_scene);
      init_seek_enemy();
    }

    //create a random seek enemy 
    void init_seek_enemy(){
      speed_ship.create_seek_enemy();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
      //create a radar for the enemy_speed ships
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    void update(scene_node *target_ship){
      
      ai.wander(ship_node);
      //seek(target_ship);  
    }

    ~enemy_speed() {
    }
  };
}

#endif
