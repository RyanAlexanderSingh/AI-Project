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
    // scene for drawing box

    //handles player controls
    inputs inputs;

    //scene node for the player
    ref<scene_node> player_node;
    //camera instance
    ref<camera_instance> the_camera;
      
    collada_builder loader;

    // scene for drawing box
    ref<visual_scene> app_scene;

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

      mesh *player_mesh = dict.get_mesh("pSphere1-lib+lambert1");
      material *mat = new material(new image("assets/skybox.gif"));
      mat4t location;
      app_scene->add_shape(location, player_mesh, mat, true);
    }

    /// this is called once OpenGL is initialized
    void app_init() {

      inputs.init(this);

      app_scene = new visual_scene();
      app_scene->set_world_gravity(btVector3(0, 0, 0));

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
      player_node = app_scene->get_mesh_instance(0)->get_node();

      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->set_far_plane(10000);

      inputs.init(this);

      //skybox
      create_skybox();

      material *red = new material(vec4(0, 1, 1, 1));
      mesh_box *box = new mesh_box(vec3(100, 1, 100));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      location.loadIdentity();
      app_scene->add_shape(location, box, red, true, 0.0f);

      material *blue = new material(vec4(0, 0, 1, 1));
      mesh_sphere *sphere = new mesh_sphere(vec3(0, 0, 0), 0.2f);
      for (int i = -10; i <= 10; ++i) {
        for (int j = -10; j <= 10; ++j) {
          mat4t location;
          location.translate((float)i, 70, (float)j);
          app_scene->add_shape(location, sphere, blue, true);
        }
      }
    }
     
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      inputs.update(player_node, the_camera->get_node());
      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);
    
      // draw the scene
      app_scene->render((float)vx / vy);

      scene_node *skybox = app_scene->get_mesh_instance(1)->get_node();
      skybox->rotate(0.003f, vec3(1, 0, 0));
      skybox->rotate(0.003f, vec3(0, 1, 0));
    }
  };
  }

#endif
