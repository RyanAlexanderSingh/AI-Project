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
    const float speed = 5.0f;
    float current_angle = 0.0f;

  public:
    civilian_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      civilian.init(the_app, app_scene);
      init_civilian_ship();
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
      for (int i = 0; i < enemies.size(); ++i){
        vec3 facingVec = enemies[i]->get_position() - ship_node->get_position();
        //check if its within the range to run away from them
        if ((facingVec.x() > -avoidance_range && facingVec.x() < avoidance_range)
          && (facingVec.z() > -avoidance_range && facingVec.z() < avoidance_range)){
          //we want to use this vector to flee the opposite way
          ai.flee(ship_node, enemies[i]);
        }
        else{
          //ai.wander(ship_node, speed);
        }
      }
    }

    ~civilian_ship() {
    }
  };
}

#endif
