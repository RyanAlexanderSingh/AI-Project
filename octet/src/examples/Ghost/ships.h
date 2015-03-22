////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Creation of different types of ships are here
// Currently creating Player Ship, Seek Ship, Boss Ship, Third Ship??
//
#ifndef SHIPS_H_INCLUDED
#define SHIPS_H_INCLUDED

#include <random>

namespace octet {

  class ships : public resource {

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

    scene_node radar;

  public:
    ships(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;
    }

    void create_radar(float agro_range){
      mesh_box *radar_mesh = new mesh_box(vec3(agro_range*1.5f, 0.0f, agro_range*1.5f));
      material *circle = new material(new image("assets/circle.gif"));
      mat4t location;
      location.translate(0.0f, -2.0f, 0.0f);
      app_scene->add_shape(location, radar_mesh, circle, false, false);
    }

    //create a player node
    void create_player(){
      if (!loader.load_xml("assets/SpaceShip.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *player_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/playerShip_test.jpg"));
      mat4t location;
      app_scene->add_shape(location, player_mesh, mat, false);
    }

    //create a big boss enemy
    void create_boss_enemy(){
      if (!loader.load_xml("assets/boss_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));
   
      mat4t enemy_location;
      enemy_location.translate(vec3(50.0f, 0.0f, 50.0f));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
    }

    //create a random ship to be announced enemy
    void create_random_ship(){
      if (!loader.load_xml("assets/boss_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));

      float rand_x = float(rand() % 100);
      float rand_z = float(rand() % 100);
      mat4t enemy_location;
      enemy_location.translate(vec3(rand_x, 0.0f, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
    }

    //create a random seek enemy 
    void create_seek_enemy(){
      if (!loader.load_xml("assets/seek_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/seekenemyship_uv.jpg"));
      //get some random numbers for x and z pos
      float rand_x = float(rand() % 100);
      float rand_z = float(rand() % 100);
      mat4t enemy_location;
      enemy_location.translate(vec3(rand_x, 0.0f, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
    }
    ~ships() {
    }
  };
}

#endif
