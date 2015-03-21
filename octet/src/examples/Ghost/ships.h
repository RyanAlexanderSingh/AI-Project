////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Ships created here
//
#ifndef SHIPS_H_INCLUDED
#define SHIPS_H_INCLUDED

#include <random>

namespace octet {

  class ships : public resource {

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

  public:
    ships(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;
    }

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
      location.translate(vec3(0, 100, 0));
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
      enemy_location.translate(vec3(125.0f, 100.0f, 125.0f));
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
      enemy_location.translate(vec3(rand_x, 100.0f, rand_z));
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
      enemy_location.translate(vec3(rand_x, 100.0f, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
    }
    ~ships() {
    }
  };
}

#endif
