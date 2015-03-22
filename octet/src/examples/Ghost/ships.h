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

      shader = new color_shader();

      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // corners (vertices) of the triangle
      static const float vertex_data[] = {
        -1.5f, -1.5f, 0.0f,
        1.5f, -1.5f, 0.0f,
        0.0f, 1.5f, 0.0f,
      };

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    }

    void update_triangle(mat4t modelToProjection){
      /// clear the background and the depth buffer
      glClearColor(0, 0, 0, 1);
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /// allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glEnable(GL_DEPTH_TEST);

      // we use a unit matrix will not change the (-1..1, -1..1, -1..1) xyz space of OpenGL
      /*mat4t modelToProjection;*/

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
