
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
 
    ships the_player;
    
    //handles player controls
    ship_controls inputs;

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

      the_player.init(the_app, app_scene);
      //create the player mesh and scene node
      init_player_ship();

      inputs.init(the_app, app_scene);

      the_camera = app_scene->get_camera_instance(0)->get_node();
    }

    void init_player_ship(){
      the_player.create_player();
      player_node = app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    void update(){    
      inputs.update(player_node, the_camera);
      the_player.update_triangle(player_node->access_nodeToParent());
    }

    ~player() {
    }
  };
}

#endif
