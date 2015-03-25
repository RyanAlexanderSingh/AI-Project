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

    ships civilian;
    ai_behaviours ai;

    ship_controls inputs;

    ref<scene_node> ship_node;
    const float avoidance_range = 45.0f;
    const float flocking_range = 30.0f;
    const float speed = 5.0f;

    enum civilianState { FLEEING, WANDERING, FLOCKING }; //the different states the civilians can be in
    civilianState state; //create a new state

  public:
    civilian_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      civilian.init(the_app, app_scene);
      init_civilian_ship();
      state = WANDERING; //set the init state as wandering
    }

    void init_civilian_ship(){
      civilian.create_civilian_ship();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    //civilian ships are scared of everything except the player
    void update(dynarray<scene_node*> enemies, scene_node *player){
      state = WANDERING; //set the default state
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);
      //check the enemies, we want to evade them
      for (unsigned i = 0; i < enemies.size() && state != FLEEING; ++i){
        vec3 distanceVec = enemies[i]->get_position() - ship_node->get_position();
        //check if its within the range to run away from them
        if ((distanceVec.x() > -avoidance_range && distanceVec.x() < avoidance_range)
          && (distanceVec.z() > -avoidance_range && distanceVec.z() < avoidance_range)){
          //we want to use this vector to flee the opposite way
          ai.flee(ship_node, enemies[i]);
          state = FLEEING; //lets set it to fleeing
        }
      }
      //check the player - we want to follow him
      vec3 distanceVec = player->get_position() - ship_node->get_position();
      //check if its within the range to flock towards the player
      if ((distanceVec.x() > -avoidance_range && distanceVec.x() < avoidance_range)
        && (distanceVec.z() > -avoidance_range && distanceVec.z() < avoidance_range)){
        //we want to use this vector to flee the opposite way
        ai.flock(ship_node, player);
        state = FLOCKING; //lets set it to fleeing
      }
      //default behaviour, lets just let it wandering around
      if (state == WANDERING){
        ai.wander(ship_node, speed);
      }
    }

    ~civilian_ship() {
    }
  };
}

#endif
