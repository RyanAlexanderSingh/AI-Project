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
    const float sq_flee_range = 35.0f*35.0f;
    const float sq_flocking_range = 30.0f*30.0f;
    const float sq_following_range = 10.0f*10.0f;
    float acceleration = 0.0f;

    bool set_free = false;

    enum civilianState { FLEEING, WANDERING, FLOCKING, LEADER_FOLLOWING, DEAD, SAVED }; //the different states the civilians can be in
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

    //bad way of deactivating the ship
    void deactive_ship(){
      state = DEAD;
    }

    bool active_state(){
      if (state == DEAD){
        return false;
      }
      else {
        return true;
      }
    }

    bool freed(){
      return set_free;
    }

    //civilian ships are scared of everything except the player
    void update(dynarray<scene_node*> enemies, scene_node *player, float player_orientation){
      //check if the ship is active -> if its not, we'll pool it and place it elsewhere for the time being
      if (state != DEAD){
        state = WANDERING;
        float lineWidth = 1.0f; //default size of glLines
        //activate bullet physics
        ship_node->activate();
        ship_node->set_damping(0.5f, 0.5f);

        //check the player - we want to follow him
        vec3 distanceVec = player->get_position() - ship_node->get_position();
        //check if its within the range to flock towards the player
        if (distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_flocking_range){
          //if hes already within range and hes within range to follow, let it follow but now he has no control
          if (distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_following_range){
            if (the_app->is_key_going_down(' ')){
              set_free = true; //set the ship free
              ship_node->set_position(vec3(8000.0f, 0.0f, 8000.0f));
            }
            ai.flock(ship_node, player);
          }
          else{
            state = FLOCKING; //flock but he might still try and escape
            ai.flock(ship_node, player);
          }
          ai.flock(ship_node, player);
        }
        //check the enemies, we want to evade them
        for (unsigned i = 0; i < enemies.size() && state != SAVED; ++i){
          vec3 distanceVec = enemies[i]->get_position() - ship_node->get_position();
          //check if its within the range to run away from them
          if ((distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_flee_range)){
            //we want to use this vector to flee the opposite way
            ai.flee(ship_node, enemies[i]);
            state = FLEEING; //lets set it to fleeing
          }
        }
        //default behaviour, lets just let it wandering around
        if (state == WANDERING && !set_free){
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
        if (!set_free){
          civilianSpaceShip.statusCircle(lineWidth, 30.0f, ship_node, player, player_orientation);
        }
      }
      //if its not active, its state is 'destroyed' / dead
      else if (state == DEAD){
        ship_node->set_position(vec3(1000.0f, 0.0f, 1000.0f));
      }
    }

    ~civilian_ship() {
    }
  };
}

#endif
