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
   
   //basic default ai behaviours to inherit
    ai_behaviours ai;

    //create default player
    player_controller player;
    //scene node for the player
    ref<scene_node> player_node;

    //arrays to store the NPC's
    dynarray<merc_ship*> merc_array;
    ref<boss_ship> boss_enemy;
    //our array for storing all of the enemies
    dynarray<scene_node*> enemies;

    //for the civilians
    dynarray<civilian_ship*> civilian_array;
    dynarray<scene_node*> civilians;

    // scene for drawing box
    ref<visual_scene> app_scene;

    //only using this for the skybox (think about moving this)
    collada_builder loader;

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

      ai.init(); //essentially just creating the random seed

      //skybox
      //create_skybox();

      //create the merc enemy ships
      for (int i = 0; i < 5; ++i){
        merc_ship *merc = new merc_ship();
        merc->init(this, app_scene);
        merc_array.push_back(merc);
        enemies.push_back(merc->return_ship_node());  //lets add the enemies to the array so we can check all enemies
      }
      
      //create the boss ship
      boss_enemy = new boss_ship();
      boss_enemy->init(this, app_scene);
      enemies.push_back(boss_enemy->return_ship_node()); //lets add the enemies to the array so we can check all enemies

      //create the civilian ships
      for (int i = 0; i < 10; ++i){
        civilian_ship *civilian = new civilian_ship();
        civilian->init(this, app_scene); //let the civilians know who the enemies are 
        civilians.push_back(civilian->return_ship_node());
        civilian_array.push_back(civilian);        
      }

      //create the player ship
      player.init(this, app_scene);
      player_node = player.return_player_node();

    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      //update our ships
      player.update();
      //update the enemies

      boss_enemy->update(civilian_array, merc_array, player_node, player.get_orientation());
      for (unsigned i = 0; i < merc_array.size(); ++i){
        //lets get the civilian scene nodes
        merc_array[i]->update(civilian_array, enemies[enemies.size() - 1], player_node, player.get_orientation());
      }
      //update the civilians
      for (unsigned i = 0; i < civilian_array.size(); ++i){
        civilian_array[i]->update(enemies, player_node, player.get_orientation());
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
