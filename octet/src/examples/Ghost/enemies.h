////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Class of the different enemies
//
#ifndef ENEMIES_H_INCLUDED
#define ENEMIES_H_INCLUDED

#include <random>

namespace octet {

  class enemies : public resource {

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

    ship_controls inputs;

    ref<scene_node> enemy_node;

    //subject to change
    float acceleration = 5.0f;
    float angle_iteration = 0.01f;
    vec3 facingVec = (0.0f, 0.0f, 0.0f);
    float oldAngle = 0.0f;

  public:
    enemies(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      inputs.init(app, app_scene);
    }

    //create a random seek enemy 
    void create_seek_enemy(){
      if (!loader.load_xml("assets/seek_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));
      //get some random numbers for x and z pos
      int rand_x = rand() % 100;
      int rand_z = rand() % 100;
      mat4t enemy_location;;
      enemy_location.translate(vec3(rand_x, 100, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
      enemy_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    void create_boss_enemy(){
      if (!loader.load_xml("assets/boss_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));
      //get some random numbers for x and z pos
      int rand_x = rand() % 100;
      int rand_z = rand() % 100;
      mat4t enemy_location;;
      enemy_location.translate(vec3(rand_x, 100, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
      enemy_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances()-1)->get_node();
    }

    scene_node *return_ship_node(){
      return enemy_node;
    }

    void find_player(scene_node *target_ship){
      enemy_node->activate();
      enemy_node->set_damping(0.5f, 2.5f);
      enemy_node->set_friction(1.0f);

      vec3 targets_location = target_ship->get_position();
      vec3 current_location = enemy_node->get_position();

      facingVec = targets_location - current_location;

      float temp = atan2(facingVec.z(), facingVec.x());
      float tempDegrees = temp;

      tempDegrees += 90.0f;

      if (tempDegrees <= 0.0f){
        tempDegrees = 360.0f - (-tempDegrees);
      }

      float angle_diff = oldAngle - tempDegrees;
      oldAngle = tempDegrees;

      if (facingVec.x() > 10.0f || facingVec.z() > 10.0f || facingVec.x() < -10.0f || facingVec.z() < -10.0f){
        inputs.accelerate(enemy_node, 10.0f);
      }
      else{
        enemy_node->set_damping(5.0f, 3.5f);
      }
     
      inputs.rotate(enemy_node, angle_diff);

      //DEBUGGING   
      printf("Distance:%f, %f, %f\n", facingVec.x(), facingVec.y(), facingVec.z());
      //printf("Degrees: %f\n", tempDegrees);
      //printf("Degrees: %f\n", angle_diff);
    }

    ~enemies() {
    }
  };
}

#endif
