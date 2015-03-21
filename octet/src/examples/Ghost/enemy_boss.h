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
    ship_controls inputs;

    ref<scene_node> enemy_node;

    float agro_range = 25.0f;

  public:
    enemy_boss(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      boss_ship.init(the_app, app_scene);
      inputs.init(app, app_scene);

      init_boss_enemy();
    }

    void init_boss_enemy(){
      boss_ship.create_boss_enemy();
      enemy_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    scene_node *return_ship_node(){
      return enemy_node;
    }

    void update(scene_node *target_ship){

      enemy_node->activate();
      enemy_node->set_damping(0.5f, 0.5f);
      enemy_node->set_friction(1.0f);

      vec3 facingVec = target_ship->get_position() - enemy_node->get_position();

      //if the player is in range, start to seek him out
      if ((facingVec.x() < agro_range && facingVec.x() > 0.0f) || (facingVec.z() < agro_range && facingVec.z() > agro_range) ||
        (facingVec.x() > -agro_range && facingVec.x() < 0.0f) || (facingVec.z() > -agro_range && facingVec.z() < agro_range))
      {
        seek(target_ship, facingVec);
      }
      else{
        enemy_node->set_damping(2.0f, 0.5f);
      }
    }

    //seek the player by moving towards him
    void seek(scene_node *target_ship, vec3 facingVec){

      float angle = atan2(facingVec.x(), facingVec.z());
      float angle_diff = angle - heading;
      heading = angle;

      inputs.rotate(enemy_node, angle_diff);
      inputs.accelerate(enemy_node, 6.0f);
    }

    ~enemy_boss() {
    }
  };
}

#endif
