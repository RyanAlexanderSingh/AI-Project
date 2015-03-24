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

  public:
    civilian_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      civilian.init(the_app, app_scene);
      init_boss_enemy();
    }

    void init_boss_enemy(){
      civilian.create_random_ship();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    //civilian ships are scared of everything except the player
    void update(scene_node *target_ship){

      boss.update_agro_circle();
      vec3 facingVec = target_ship->get_position() - ship_node->get_position();
      float ship_x = facingVec.x();
      float ship_z = facingVec.z();
      if ((ship_x < agro_range && ship_x > 0.0f) || (ship_z < agro_range && ship_z > 0.0f) ||
        (ship_x > -agro_range && ship_x < 0.0f) || (ship_z > -agro_range && ship_z < 0.0f))
      {
        //if any enemy is in range, flee from him 
        ai.flee(ship_node, target_ship, facingVec);
      }
      else{
        ai.wander(ship_node);
      }
    }

    ~civilian_ship() {
    }
  };
}

#endif
