////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Class of the different enemies
//
#ifndef ENEMIES_H_INCLUDED
#define ENEMIES_H_INCLUDED
namespace octet {

  class enemies : public resource {

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

  public:
    enemies(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;
    }

    void create_enemy1(){
      if (!loader.load_xml("assets/SpaceShip.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/playerShip_test.jpg"));
      mat4t enemy_location;
      enemy_location.translate(vec3(0, 100, 50));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);
    }

    ~enemies() {
    }
  };
}

#endif
