////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
//
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
namespace octet {

  class player : public resource {

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

  public:
    player(){}

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

    ~player() {
    }
  };
}

#endif
