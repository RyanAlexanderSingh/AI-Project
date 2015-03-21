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
    ship_controls inputs;

    ai_behaviours ai;

    ref<scene_node> ship_node;

    //subject to change
    float current_angle = 0.0f;

    float wandertheta = 0.0f;

    const vec3 agro_range = (100.0f, 0.0f, 100.0f);

  public:
    enemy_speed(){}



    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      speed_ship.init(app, app_scene);
      inputs.init(app, app_scene);

      init_seek_enemy();
    }

    //create a random seek enemy 
    void init_seek_enemy(){
      speed_ship.create_seek_enemy();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    void update(scene_node *target_ship){
      
      ai.wander(ship_node);
      //seek(target_ship);  
    }

    void seek(scene_node *target_ship){
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      vec3 facingVec = target_ship->get_position() - ship_node->get_position();

      float angle = atan2(facingVec.x(), facingVec.z());
      
      float angle_diff = angle - current_angle;
      current_angle = angle;

      inputs.rotate(ship_node, angle_diff);
    
      if (facingVec.x() > 10.0f || facingVec.z() > 10.0f || facingVec.x() < -10.0f || facingVec.z() < -10.0f){
        inputs.accelerate(ship_node, 10.0f);
      }
      else{
        ship_node->set_damping(2.0f, 0.5f);
      }
    }

    ~enemy_speed() {
    }
  };
}

#endif
