////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Speedy ships
//
#ifndef ENEMY_BOSS_H_INCLUDED
#define ENEMY_BOSS_H_INCLUDED

namespace octet {

  class enemy_boss : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships boss_ship;
    ai_behaviours ai;

    ref<scene_node> ship_node;

    //shaders for the agro radius circles
    ref<color_shader> boss_shader;
    GLuint boss_vertices;

    const float agro_range = 25.0f;

  public:
    enemy_boss(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      boss_ship.init(the_app, app_scene);
      init_boss_enemy();
    }

    void init_boss_enemy(){
      boss_ship.create_boss_enemy();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    void update(scene_node *target_ship){

      //boss_ship.update_triangle();
      vec3 facingVec = target_ship->get_position() - ship_node->get_position();
      //if the player is in range, start to seek him out
      if ((facingVec.x() < agro_range && facingVec.x() > 0.0f) || (facingVec.z() < agro_range && facingVec.z() > 0.0f) ||
        (facingVec.x() > -agro_range && facingVec.x() < 0.0f) || (facingVec.z() > -agro_range && facingVec.z() < 0.0f))
      {
        ai.seek(ship_node, target_ship);
      }
      else{
        ai.wander(ship_node);
      }
    }

    ~enemy_boss() {
    }
  };
}

#endif
