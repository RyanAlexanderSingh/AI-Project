
////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
// This class just creates a player mesh and returns the player node -> could probably merge with other enemy mesh creations....
//
#ifndef AI_BEHAVIOURS_H_INCLUDED
#define AI_BEHAVIOURS_H_INCLUDED
namespace octet {

  class ai_behaviours : public resource {

    //handles player controls
    ship_controls inputs;

    app *the_app;
    visual_scene *app_scene;

    //variables for wandering
    float wandertheta = 0.0f;

    float current_angle = 0.0f;

  public:
    ai_behaviours(){}

    //predefined to pass back random numbers bettwen -0.1f and 0.1f
    float random_float() {
      float rand_num = 0;
      rand_num = rand() % 10 - 5; //gives me a number between -10 and 10;
      rand_num = rand_num / 100.0f;

      float r3 = -0.15f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.15f - -0.15f)));
      //printf("random number: %f \n", r3);
      return r3;
    }

    void init(){
      srand(static_cast <unsigned> (time(0)));
    }

    //Basic wandering behaviours
    void wander(scene_node *ship_node){
      float wander_radius = 5.0f;         // radius for our "wander circle"
      float wander_distance = 17.0f;         // distance for our "wander circle

      wandertheta += random_float(); //get a random float back between 

      vec3 circleloc = ship_node->get_z(); //get forward vector
      circleloc.normalize(); 
      circleloc *= wander_distance; //multiply by distance 
      circleloc += ship_node->get_position(); //make it relative to the ship nodes location

      vec3 forwardVec = ship_node->get_z(); //get forward vector again

      float h = atan2(forwardVec.x(), forwardVec.z()); //need to know heading to offset wandertheta
      vec3 circleOffSet = (wander_radius * cos(wandertheta + h), 0.0f, wander_radius * sin(wandertheta + h));
      vec3 target = circleloc + circleOffSet; //our final target to aim for

      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      vec3 facingVec = target - ship_node->get_position();

      float angle = atan2(facingVec.x(), facingVec.z());
      float angle_diff = angle - current_angle;
      current_angle = angle;

      inputs.rotate(ship_node, angle_diff);
      inputs.accelerate(ship_node, 4.0f);
    }

    //Basic seek behaviours 
    void seek(scene_node *ship_node, scene_node *target_ship){
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      vec3 facingVec = target_ship->get_position() - ship_node->get_position();

      float angle = atan2(facingVec.x(), facingVec.z());

      float angle_diff = angle - current_angle;
      current_angle = angle;

      inputs.rotate(ship_node, angle_diff);
      inputs.accelerate(ship_node, 10.0f);
    }

    ~ai_behaviours() {
    }
  };
}

#endif
