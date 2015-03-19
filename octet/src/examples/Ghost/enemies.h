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
    float heading = 0.0f;
    float velocity = 0.0f;

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
      int rand_x = rand() % -100 - 100;
      int rand_z = rand() % -100 - 100;
      mat4t enemy_location;;
      enemy_location.translate(vec3(rand_x, 100, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
      enemy_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances()-1)->get_node();
    }

    scene_node *return_ship_node(){
      return enemy_node;
    }

    void seek(scene_node *target_ship){
      enemy_node->activate();
      enemy_node->set_damping(0.5f, 0.5f);
      enemy_node->set_friction(1.0f);

      vec3 facingVec = target_ship->get_position() - enemy_node->get_position();

      float angle = atan2(facingVec.x(), facingVec.z());
      
      float angle_diff = angle - heading;
      heading = angle;

      inputs.rotate(enemy_node, angle_diff);

      if (facingVec.x() > 10.0f || facingVec.z() > 10.0f || facingVec.x() < -10.0f || facingVec.z() < -10.0f){
        inputs.accelerate(enemy_node, 10.0f);
      }
      else{
        enemy_node->set_damping(2.0f, 0.5f);
      }
    }

    ~enemies() {
    }
  };
}

#endif
