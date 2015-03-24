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

    ref<scene_node> ship_node;

    const float agro_range = 25.0f;
    const float speed = 2.0f;

  public:
    civilian_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      civilian.init(the_app, app_scene);
      init_boss_enemy();
    }

    void init_boss_enemy(){
      civilian.create_civilian_ship();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    //civilian ships are scared of everything except the player
    void update(dynarray<scene_node*> enemies){

      for (int i = 0; i < enemies.size(); ++i){
        vec3 enemy_position = enemies[i]->get_position();
        //we're only interested in the difference in the x and z axis
        vec3 facingVec = enemy_position - ship_node->get_position();
        //check if its within the range to run away from them
        if ((facingVec.x() > -agro_range && facingVec.x() < agro_range) && (facingVec.z() > -agro_range && facingVec.z() < agro_range)){
          ai.flee(ship_node, facingVec);
        }
        else{
          ai.wander(ship_node, speed);
        }
      }
    }

    ~civilian_ship() {
    }
  };
}

#endif
