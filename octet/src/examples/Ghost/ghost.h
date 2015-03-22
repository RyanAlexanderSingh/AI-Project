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
    player player;
    //scene node for the player
    ref<scene_node> player_node;

    //arrays to store the seek enemies (objects)
    dynarray<enemy_speed*> seek_enemies;
    ref<enemy_boss> boss_enemy;

    // scene for drawing box
    ref<visual_scene> app_scene;
    //shaders for the agro radius circles
    ref<color_shader> shader;
    GLuint vertices;

    //only using this for the skybox (think about moving this)
    collada_builder loader;

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


    void update_circles(){
      /// clear the background and the depth buffer
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /// allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      //glEnable(GL_DEPTH_TEST);

      // we use a unit matrix will not change the (-1..1, -1..1, -1..1) xyz space of OpenGL
      mat4t modelToProjection;

      // we use a simple solid color shader.
      vec4 emissive_color(0, 0, 1, 1);
      shader->render(modelToProjection, emissive_color);

      // use vertex attribute 0 for our vertices (we could use 1, 2, 3 etc for other things)
      glEnableVertexAttribArray(0);

      // use the buffer we made earlier.
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // tell OpenGL what kind of vertices we have
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

      // draw a triangle
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    /// this is called once OpenGL is initialized
    void app_init() {

      app_scene = new visual_scene();
      app_scene->set_world_gravity(btVector3(0, 0, 0));
      shader = new color_shader();

      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // corners (vertices) of the triangle
      static const float vertex_data[] = {
        -1.5f, -1.5f, 1.0f,
        1.5f, -1.5f, 1.0f,
        1.0f, 1.5f, 1.0f,
      };

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(10000);

      ai.init(); //essentially just creating the random seed

      //create the player ship
      player.init(this, app_scene);
      player_node = player.return_player_node();

      //create the speed enemy ships
      for (int i = 0; i < 5; ++i){
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

      /// set a viewport - includes whole window area
      glViewport(0, 0, vx, vy);

      update_circles();

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
