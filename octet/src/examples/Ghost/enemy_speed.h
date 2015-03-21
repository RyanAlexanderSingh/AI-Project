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

    ref<scene_node> enemy_node;

    //subject to change
    float current_angle = 0.0f;

    float wandertheta = 0.0f;
    vec3 acceleration = (0.0f, 0.0f, 0.0f);
    vec3 velocity = (0.0f, 0.0f, 0.0f);

    const vec3 agro_range = (100.0f, 0.0f, 100.0f);

  public:
    enemy_speed(){}

    //predefined to pass back random numbers bettwen -0.2f and 0.2f
    float random_float() {
      srand(time(0));
      float rand_num = 0;
      rand_num = rand() % 20 - 10; //gives me a number between -30 and 30;
      rand_num = rand_num / 100.0f;
      /*printf("random number: %f \n", rand_num);*/
      return rand_num;
    }

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
      enemy_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return enemy_node;
    }

    void update(scene_node *target_ship){
      
      //velocity += acceleration;
      wander();
      //seek(target_ship);
     
    }

    void wander(){
      float wanderR = 5.0f;         // Radius for our "wander circle"
      float wanderD = 16.0f;         // Distance for our "wander circle

      wandertheta += random_float();

      vec3 circleloc = enemy_node->get_z();
      circleloc.normalize();
      circleloc *= wanderD;
      circleloc += enemy_node->get_position();

      vec3 forwardVec = enemy_node->get_z();

      float h = atan2(forwardVec.x(), forwardVec.z());
      vec3 circleOffSet = (wanderR * cos(wandertheta + h), 0.0f,  wanderR * sin(wandertheta + h));
      vec3 target = circleloc + circleOffSet;

      printf("Current Position: %f %f \n", enemy_node->get_position().x(), enemy_node->get_position().z());
      printf("Target: %f %f \n", target.x(), target.z());

      wander_seek(target);
    }

    void wander_seek(vec3 wander_target){
      enemy_node->activate();
      enemy_node->set_damping(0.5f, 0.5f);
      enemy_node->set_friction(1.0f);

      vec3 facingVec = wander_target - enemy_node->get_position();

      float angle = atan2(facingVec.x(), facingVec.z());

      float angle_diff = angle - current_angle;
      current_angle = angle;

      inputs.rotate(enemy_node, angle_diff);
      inputs.accelerate(enemy_node, 4.0f);
    }

    void seek(scene_node *target_ship){
      enemy_node->activate();
      enemy_node->set_damping(0.5f, 0.5f);
      enemy_node->set_friction(1.0f);

      vec3 facingVec = target_ship->get_position() - enemy_node->get_position();

      float angle = atan2(facingVec.x(), facingVec.z());
      
      float angle_diff = angle - current_angle;
      current_angle = angle;


      inputs.rotate(enemy_node, angle_diff);
    
      if (facingVec.x() > 10.0f || facingVec.z() > 10.0f || facingVec.x() < -10.0f || facingVec.z() < -10.0f){
        inputs.accelerate(enemy_node, 10.0f);
      }
      else{
        enemy_node->set_damping(2.0f, 0.5f);
      }
    }

    ~enemy_speed() {
    }
  };
}

#endif
