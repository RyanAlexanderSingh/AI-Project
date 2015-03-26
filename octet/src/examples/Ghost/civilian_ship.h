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
    const float sq_agro_range = 45.0f*45.0f;
    const float sq_flocking_range = 30.0f*30.0f;
    const float speed = 5.0f;


    enum civilianState { FLEEING, WANDERING, FLOCKING, FOLLOWING }; //the different states the civilians can be in
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

    //civilian ships are scared of everything except the player
    void update(dynarray<scene_node*> enemies, scene_node *player, float player_orientation){

      state = WANDERING; //set the default state
      vec3 colour = (0.0f, 0.0f, 0.0f); //colour for the radius
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      //check the player - we want to follow him
      vec3 distanceVec = player->get_position() - ship_node->get_position();
      //check if its within the range to flock towards the player
      if ((distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_flocking_range)
        && state != FLOCKING){
        //we want to use this vector to flee the opposite way
        ai.flock(ship_node, player);
        state = FLOCKING; //lets set it to fleeing
      }

      //check the enemies, we want to evade them
      for (unsigned i = 0; i < enemies.size() && state != FLEEING; ++i){
        vec3 distanceVec = enemies[i]->get_position() - ship_node->get_position();
        //check if its within the range to run away from them
        if ((distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_agro_range)){
          //we want to use this vector to flee the opposite way
          ai.flee(ship_node, enemies[i]);
          state = FLEEING; //lets set it to fleeing
        }
      }
      //default behaviour, lets just let it wandering around
      if (state == WANDERING){
        ai.wander(ship_node, speed);
      }
      switch (state){
      case WANDERING:
        colour.y() = 1.0f; //set the colour green
        break;
      case FLEEING:
        break;
      case FLOCKING:
        break;
      }
      civilianSpaceShip.update_radius(ship_node, player, player_orientation, colour);
    }

    ~civilian_ship() {
    }
  };
}

#endif
