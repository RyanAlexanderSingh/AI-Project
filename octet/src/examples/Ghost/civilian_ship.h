////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Speedy ships
//
#ifndef CIVILIAN_SHIP_H_INCLUDED
#define CIVILIAN_SHIP_H_INCLUDED

namespace octet {

  class civilian_ship : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships civilianSpaceShip;
    ai_behaviours ai;

    ref<scene_node> ship_node;
    const float sq_flee_range = 30.0f*30.0f;
    const float sq_flocking_range = 30.0f*30.0f;
    const float sq_following_range = 10.0f*10.0f;
    float acceleration = 0.0f;

    enum civilianState { FLEEING, WANDERING, FLOCKING, LEADER_FOLLOWING }; //the different states the civilians can be in
    civilianState state; //create a new state

  public:
    civilian_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      civilianSpaceShip.init(the_app, app_scene);
      ship_node = civilianSpaceShip.create_civilian_ship();
      state = WANDERING; //set the init state as wandering
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    ///map_values allows the mapping of two values (used to set the acceleration depending on position)
    float map_values(float x, float in_min, float in_max, float out_min, float out_max){
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    //civilian ships are scared of everything except the player
    void update(dynarray<scene_node*> enemies, scene_node *player, float player_orientation){

      state = WANDERING; //set the default state
      float lineWidth = 1.0f; //default size of glLines
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);

      //check the player - we want to follow him
      vec3 distanceVec = player->get_position() - ship_node->get_position();
      //check if its within the range to flock towards the player
      if ((distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_flocking_range)
        && state != FLOCKING){
        float desiredAcceleration = distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z();
        acceleration = map_values(desiredAcceleration, 0.0f, sq_flocking_range, 0.0f, 7.0f);
        acceleration = acceleration < 0.7f ? 0.0f : acceleration; //cap it because we have zero gravity (causes all kinds of problems)
        printf("Acceleration: %f\n", acceleration);
        //if hes already within range and hes within range to follow, let it follow but now he has no control
        if (distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_following_range){
          ai.flock(ship_node, player, acceleration);
          state = LEADER_FOLLOWING;
        }
        else{
          ai.flock(ship_node, player, acceleration);
          state = FLOCKING; //flock but he might still try and escape
        }
      }

      //check the enemies, we want to evade them
      for (unsigned i = 0; i < enemies.size() && state != LEADER_FOLLOWING; ++i){
        vec3 distanceVec = enemies[i]->get_position() - ship_node->get_position();
        //check if its within the range to run away from them
        if ((distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_flee_range)){
          //we want to use this vector to flee the opposite way
          ai.flee(ship_node, enemies[i]);
          state = FLEEING; //lets set it to fleeing
        }
      }
      //default behaviour, lets just let it wandering around
      if (state == WANDERING){
        ai.wander(ship_node, 5.0f);
      }
      switch (state){
      case WANDERING:
        lineWidth = 1.0f;
        break;
      case FLEEING:
        lineWidth = 2.0f;
        break;
      case FLOCKING:
        lineWidth = 2.0f;
        break;
      }
      if (state != LEADER_FOLLOWING){
        civilianSpaceShip.statusCircle(lineWidth, 30.0f, ship_node, player, player_orientation);
      }
    }

    ~civilian_ship() {
    }
  };
}

#endif
