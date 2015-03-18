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

      inputs.init(app);
    }

    //create a random seek enemy 
    void create_seek_enemy(){
      if (!loader.load_xml("assets/SpaceShip.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));
      mat4t enemy_location;
      //srand(time(NULL));
      int rand_x = rand() % 100;
      int rand_z = rand() % 100;
      enemy_location.translate(vec3(rand_x, 100, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
    }

    void find_player(scene_node *target_ship, scene_node *ai_ship){
      ai_ship->activate();
      ai_ship->set_damping(0.5f, 0.5f);
      ai_ship->set_friction(1.0f);

      vec3 targets_location = target_ship->get_position();
      vec3 current_location = ai_ship->get_position();

      facingVec = targets_location - current_location;

      float temp = atan2(facingVec.z(), facingVec.x());
      float tempDegrees = temp;

      tempDegrees += 90.0f;

      if (tempDegrees <= 0.0f){
        tempDegrees = 360 - (-tempDegrees);
      }

      float angle_diff = oldAngle - tempDegrees;
      oldAngle = tempDegrees;

      inputs.rotate(ai_ship, angle_diff);

      if (facingVec.x() > 10.0f || facingVec.z() > 10.0f || facingVec.x() < -10.0f || facingVec.z() < -10.0f){
        inputs.accelerate(ai_ship, 10.0f);
      }
      else{
        ai_ship->set_damping(3.0f, 1.5f);
      }

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
