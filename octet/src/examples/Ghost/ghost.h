////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#ifndef GHOST_H_INCLUDED
#define GHOST_H_INCLUDED

namespace octet {
  /// Scene containing a box with octet.
  class ghost : public app {

    //create default player
    player player;
    //scene node for the player
    ref<scene_node> player_node;

    //arrays to store the seek enemies (objects)
    dynarray<enemy_speed*> seek_enemies;
    ref<enemy_boss> boss_enemy;

    //only using this for the skybos
    collada_builder loader;

    // scene for drawing box
    ref<visual_scene> app_scene;

    bool test = true;

  public:
    /// this is called when we construct the class before everything is initialised.
    ghost(int argc, char **argv) : app(argc, argv) {
    }

    void create_skybox(){

      if (!loader.load_xml("assets/skybox_sphere.dae")) {
        printf("failed to load file skybox file!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      // note that this call will dump the code below to log.txt
      dict.dump_assets(log(""));

      mesh *player_mesh = dict.get_mesh("pSphere1-lib+lambert1");
      material *mat = new material(new image("assets/skybox.gif"));
      mat4t location;
      app_scene->add_shape(location, player_mesh, mat, true);
      printf("loading extremely inefficient skybox\n change the image...");
    }

    /// this is called once OpenGL is initialized
    void app_init() {

      app_scene = new visual_scene();
      app_scene->set_world_gravity(btVector3(0, 0, 0));

      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(10000);

      //create the player ship
      player.init(this, app_scene);
      player_node = player.return_player_node();

      //create the speed enemy ships
      for (int i = 0; i < 1; ++i){
        enemy_speed *seek_enemy = new enemy_speed();
        seek_enemy->init(this, app_scene);
        seek_enemies.push_back(seek_enemy);
      }

      //create the boss ship
      boss_enemy = new enemy_boss();
      boss_enemy->init(this, app_scene);

      //skybox
      //create_skybox();
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      //update our ships
      player.update();
      boss_enemy->update(player_node);
      for (int i = 0; i < seek_enemies.size(); ++i){
        seek_enemies[i]->update(player_node);
      }

      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);

      // draw the scene
      app_scene->render((float)vx / vy);

      /*scene_node *skybox = app_scene->get_mesh_instance(2)->get_node();
       skybox->rotate(0.003f, vec3(1, 0, 0));
       skybox->rotate(0.003f, vec3(0, 1, 0));*/
    }
  };
}



#endif
