////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
//

#ifndef AI_ENEMY_H_INCLUDED
#define AI_ENEMY_H_INCLUDED

namespace octet {

  class ai_enemy : public resource {

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
    ai_enemy(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      inputs.init(app, app_scene);
    }

    void find_player(vec3 target_location, vec3 current_location, scene_node *enemy_ship){
      enemy_ship->activate();
      enemy_ship->set_damping(0.5f, 3.5f);
      enemy_ship->set_friction(1.0f);

      facingVec = target_location - current_location;

      btVector3 vec_x = get_btVector3(target_location);
      btVector3 vec_y = get_btVector3(current_location);

      float temp = atan2(facingVec.z(), facingVec.x());
      float tempDegrees = temp;

      tempDegrees += 90.0f;

      if (tempDegrees < 0.0f){
        tempDegrees = 360 - (-tempDegrees);
      }

      float angle_diff = oldAngle - tempDegrees;
      oldAngle = tempDegrees;

      inputs.rotate(enemy_ship, angle_diff);
      
      if (facingVec.x() > 10.0f || facingVec.z() > 10.0f || facingVec.x() < -10.0f || facingVec.z() < -10.0f){
        inputs.accelerate(enemy_ship, 6.0f);
      }
      else{
        enemy_ship->set_damping(5.0f, 3.5f);
      }
      
      //DEBUGGING   
      //printf("Distance:%f, %f, %f\n", facingVec.x(), facingVec.y(), facingVec.z());
      //printf("Degrees: %f\n", tempDegrees);
      //printf("Degrees: %f\n", angle_diff);
    }

    ~ai_enemy() {
    }
  };
}
#endif
