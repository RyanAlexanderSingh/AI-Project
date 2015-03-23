////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Speedy ships
//
#ifndef ENEMY_BOSS_H_INCLUDED
#define ENEMY_BOSS_H_INCLUDED

namespace octet {

  class boss_ship : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships boss;
    ai_behaviours ai;

    ref<scene_node> ship_node;

    //shaders for the agro radius circles
    ref<color_shader> boss_shader;
    GLuint boss_vertices;

    const float agro_range = 25.0f;

  public:
    boss_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      boss.init(the_app, app_scene);
      init_boss_enemy();
    }

    void init_boss_enemy(){
      boss.create_boss_enemy();
      ship_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    void update(scene_node *target_ship){

      vec3 facingVec = target_ship->get_position() - ship_node->get_position();
      float ship_x = facingVec.x();
      float ship_z = facingVec.z();
      if ((ship_x < agro_range && ship_x > 0.0f) || (ship_z < agro_range && ship_z > 0.0f) ||
        (ship_x > -agro_range && ship_x < 0.0f) || (ship_z > -agro_range && ship_z < 0.0f))
      {
        //if the player is in range, flee from him 
        ai.flee(ship_node, target_ship, facingVec);
        printf("State Fleeing\n");
        //if the player is within range, seek
        //ai.seek(ship_node, target_ship, facingVec);
      }
      else{
        printf("State Wandering\n");
        ai.wander(ship_node);
        //printf("wandering...");
      }
    }

    ~boss_ship() {
    }
  };
}

#endif
