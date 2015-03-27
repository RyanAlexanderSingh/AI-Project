////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Creation of different types of ships are here
// Currently creating Player Ship, Seek Ship, Boss Ship, Third Ship??
//
#ifndef SHIPS_H_INCLUDED
#define SHIPS_H_INCLUDED

#define PI 3.14159265358979323846f  /* pi */

#include <random>

namespace octet {

  class ships : public resource {

    collada_builder loader;
    app *the_app;
    visual_scene *app_scene;

    GLfloat vertex_data[722];
    GLuint vertices;

  public:
    ships(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      //create basic default circle to work with
      for (int i = 0; i < 720; i += 3) {
        vertex_data[i] = (cos((PI * (i / 2) / 180.0f)) * 1); //x pos
        vertex_data[i + 1] = 0.0f; // y pos (we don't need this)
        vertex_data[i + 2] = (sin((PI * (i / 2) / 180.0f)) * 1); //z pos
      }
    }

    void statusCircle(float lineWidth, float radius, scene_node *ship = NULL, scene_node *player_ship = NULL, float angle = NULL){

      glGenBuffers(1, &vertices);
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      GLfloat relative_vertex_data[722];

      //if we're drawing relative to the player...
      if (!ship == NULL){
        vec4 distanceVec = vec4(ship->get_position() - player_ship->get_position(), 0);
        mat4t rotation;
        rotation.loadIdentity();
        rotation.rotateY(angle * (180.0f / PI));
        distanceVec = rotation.rmul(distanceVec);

        for (int i = 0; i < 720; i += 3){
          relative_vertex_data[i] = (vertex_data[i] * radius) + distanceVec.x();
          relative_vertex_data[i + 1] = vertex_data[i + 1] + distanceVec.y(); // y pos (we don't need this)
          relative_vertex_data[i + 2] = (vertex_data[i + 2] * radius) + distanceVec.z(); //z pos
        }
      }
      //if we're the player...just draw on us
      else{
        for (int i = 0; i < 720; i += 3){
          relative_vertex_data[i] = (vertex_data[i] * radius);
          relative_vertex_data[i + 1] = vertex_data[i + 1]; // y pos (we don't need this)
          relative_vertex_data[i + 2] = (vertex_data[i + 2] * radius); //z pos
        }
      }

      glBufferData(GL_ARRAY_BUFFER, sizeof(relative_vertex_data), relative_vertex_data, GL_STATIC_DRAW);
      /// allow Z buffer depth testing (closer objects are always drawn in front of far ones)
      glEnable(GL_DEPTH_TEST);

      // use vertex attribute 0 for our vertices (we could use 1, 2, 3 etc for other things)
      glEnableVertexAttribArray(0);

      // use the buffer we made earlier.
      glBindBuffer(GL_ARRAY_BUFFER, vertices);

      // tell OpenGL what kind of vertices we have
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
      glLineWidth(lineWidth);
      glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
      glDrawArrays(GL_LINE_LOOP, 0, 239);
    }

    //create a player node
    scene_node *create_player(){
      if (!loader.load_xml("assets/ships/player_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      // note that this call will dump the code below to log.txt
      dict.dump_assets(log(""));

      mesh *player_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/ships/playership_uv.jpg"));
      mat4t location;
      app_scene->add_shape(location, player_mesh, mat, false);

      return app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    //create a big boss enemy
    scene_node *create_boss_enemy(){
      if (!loader.load_xml("assets/ships/boss_ship1.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("StarShip-lib+blinn1");
      material *mat = new material(new image("assets/ships/bossship_uv.jpg"));
      float rand_x = float(rand() % 1000 + -500);
      float rand_z = float(rand() % 1000 + -500);
      mat4t enemy_location;
      enemy_location.translate(vec3(rand_x, 0.0f, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);

      return app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    //create a civilian ship
    scene_node *create_civilian_ship(){
      if (!loader.load_xml("assets/ships/civilian_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/ships/civilianship_uv.jpg"));

      float rand_x = float(rand() % 1000 + -500);
      float rand_z = float(rand() % 1000 + -500);
      mat4t enemy_location;
      enemy_location.translate(vec3(rand_x, 0.0f, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);

      return app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    //create a merc enemy 
    scene_node *create_merc_ship(){
      if (!loader.load_xml("assets/ships/merc_ship.dae")) {
        printf("failed to load file player ship!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      mesh *enemy_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/ships/mercship_uv.jpg"));
      //get some random numbers for x and z pos
      float rand_x = float(rand() % 1000 + -500);
      float rand_z = float(rand() % 1000 + -500);
      mat4t enemy_location;
      enemy_location.translate(vec3(rand_x, 0.0f, rand_z));
      app_scene->add_shape(enemy_location, enemy_mesh, mat, false);

      return app_scene->get_mesh_instance(app_scene->get_num_mesh_instances() - 1)->get_node();
    }

    ~ships() {
    }
  };
}

#endif
