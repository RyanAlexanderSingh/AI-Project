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

    //shaders for the agro radius circles
    ref<color_shader> shader;
    GLuint vertices;

  public:
    ships(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      GLfloat vertex_data[722];

      vertex_data[0] = 0.0f;
      vertex_data[1] = 0.0f;
      for (int i = 0; i < 720; i += 3) {
        vertex_data[i] = (cos((3.14159265358979323846f * (i / 2) / 180.0f)) * 30);
        vertex_data[i + 1] = 0.0f;
        vertex_data[i + 2] = (sin((3.14159265358979323846f * (i / 2) / 180.0f)) * 30);
      }
      vertex_data[720] = 0.0f;
      //vertex_data[721] = 30.0f;


      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    }

    void update_triangle(){
      /// clear the background and the depth buffer
      /// set a viewport - includes whole window area
      int vx = 0, vy = 0;
      the_app->get_viewport_size(vx, vy);
      glViewport(0, 0, vx, vy);

      /// clear the background and the depth buffer
      /*glClearColor(0, 1, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

      /// allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      //glEnable(GL_DEPTH_TEST);

      // use vertex attribute 0 for our vertices (we could use 1, 2, 3 etc for other things)
      glEnableVertexAttribArray(0);

      // use the buffer we made earlier.
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // tell OpenGL what kind of vertices we have
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, NULL);
      glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
      // draw a triangle
      glDrawArrays(GL_POINTS, 0, 361);
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
