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

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

    ship_controls inputs;

    ref<scene_node> enemy_node;

    //subject to change
    float heading = 0.0f;
    float velocity = 0.0f;

  public:
    enemy_boss(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      inputs.init(app, app_scene);
    }

    void create_boss_enemy(){
      if (!loader.load_xml("assets/boss_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));
      mat4t enemy_location;;
      enemy_location.translate(vec3(200.0f, 100.0f, 400.0f));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
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
      if ((facingVec.x() < 50.0f && facingVec.x() > 0.0f) || (facingVec.z() < 50.0f && facingVec.z() > 0.0f) ||
        (facingVec.x() > -50.0f && facingVec.x() < 0.0f) || (facingVec.z() > -50.0f && facingVec.z() < 0.0f))
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
