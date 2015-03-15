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

    inputs inputs;

    ref<scene_node> player_node;

    //camera instance
    ref<camera_instance> the_camera;
      
    collada_builder loader;

    // scene for drawing box
    ref<visual_scene> app_scene;

    ref<material> custom_mat;

    ref<image> skybox_bg;
    ref<image> jupiter_mask;


  public:
    /// this is called when we construct the class before everything is initialised.
    ghost(int argc, char **argv) : app(argc, argv) {
    }

    void create_skybox(){
      skybox_bg = new image("assets/circle_bg.jpg");
     /* jupiter_mask = new image("assets/hi.jpg");

      param_shader *shader = new param_shader("shaders/default.vs", "shaders/multitexture.fs");
      custom_mat = new material(vec4(1, 1, 1, 1), shader);
      custom_mat->add_sampler(0, app_utils::get_atom("skybox_bg"), skybox_bg, new sampler());
      custom_mat->add_sampler(1, app_utils::get_atom("jupiter_mask"), jupiter_mask, new sampler());*/

      custom_mat = new material(vec4(0, 0, 0, 1));
      mesh_box *box = new mesh_box(vec3(100));
      scene_node *node = new scene_node();
      app_scene->add_child(node);
      app_scene->add_mesh_instance(new mesh_instance(node, box, custom_mat));
    }

    /// this is called once OpenGL is initialized
    void app_init() {

      //hide the cursor
      ShowCursor(false);
      inputs.init(this);

      app_scene = new visual_scene();
      app_scene->set_world_gravity(btVector3(0, 0, 0));

      if (!loader.load_xml("assets/SpaceShip.dae")) {
        printf("failed to load file!\n");
        exit(1);
      }
      resource_dict dict;
      loader.get_resources(dict);

      //note that this call will dump the code below to log.txt
      dict.dump_assets(log(""));

      mesh *player_mesh = dict.get_mesh("pCube3-lib+blinn1");
      material *mat = new material(new image("assets/playerShip_uv.jpg"));
      mat4t location;
      location.translate(vec3(0, 100, 0));
      app_scene->add_shape(location, player_mesh, mat, false);
      player_node = app_scene->get_mesh_instance(0)->get_node();

      app_scene->create_default_camera_and_lights();
      the_camera = app_scene->get_camera_instance(0);
      the_camera->set_far_plane(10000);
      the_camera->get_node()->translate(vec3(0, 4, 0));

      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();  
      //camera.rotateX(180);
      //camera.rotateZ(180);

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

      scene_node *camera_node = the_camera->get_node();
      inputs.update(player_node, camera_node);
      // update matrices. assume 30 fps.
      app_scene->update(1.0f / 30);
    
      // draw the scene
      app_scene->render((float)vx / vy);

      scene_node *skybox = app_scene->get_mesh_instance(1)->get_node();
      //skybox->rotate(0.02f, vec3(1, 0, 0));
      //skybox->rotate(0.02f, vec3(0, 1, 0));
    }
  };
  }

#endif
