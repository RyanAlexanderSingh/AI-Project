////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
// This class just creates a player mesh and returns the player node -> could probably merge with other enemy mesh creations....
//
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
namespace octet {

  class player : public resource {

    //handles player controls
    ship_controls inputs;

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

    ref<scene_node> player_node;
    //camera instance
    ref<scene_node> the_camera;

  public:
    player(){}

    scene_node *return_player_node(){
      return player_node;
    }

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      the_camera = app_scene->get_camera_instance(0)->get_node();

      inputs.init(app, vs);
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
      player_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances()-1)->get_node();
    }

    void update(){    
      inputs.update(player_node, the_camera);
    }

    ~player() {
    }
  };
}

#endif
