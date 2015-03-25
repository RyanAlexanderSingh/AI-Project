////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Big bad boss ship - stay clear of this
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

    const float agro_range = 40.0f;

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
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      vec3 enemy_position = target_ship->get_position();
      //we're only interested in the difference in the x and z axis
      vec3 facingVec = enemy_position - ship_node->get_position();
      float diff_x = facingVec.x();
      float diff_z = facingVec.z();
      //check if its within the range to run away from them
      if ((diff_x > -agro_range && diff_x < agro_range) && (diff_z > -agro_range && diff_z < agro_range)){
        //if the player is in range, flee from him 
        ai.seek(ship_node, facingVec);
      }
      else{
        ai.wander(ship_node);
      }
    }

    ~boss_ship() {
    }
  };
}

#endif
