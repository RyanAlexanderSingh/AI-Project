////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
// mouse ball for rotating cameras around points of interest.

namespace octet { namespace helpers {
  /// Class for rotating the camera with the mouse.
  class helper_fps_controller {
    app *the_app;
    float max_angle = 0.5f; //max angle we can reach
    float angle = 0.0f; //angle float to be modified
    float angle_iteration = 0.05f;
  public:
    helper_fps_controller() {
    }

    void init(app *the_app) {
      this->the_app = the_app;
    }

    /// called every frame to update the player physics
    void update(scene_node *player_node, scene_node *camera_node) {
      #ifdef OCTET_BULLET
        player_node->set_damping(0, 0);
        //player_node->set_angular_factor(0);
        //player_node->set_angular_velocity(vec3(0, 0, 0));
        //player_node->set_rotation(mat4t());
        player_node->clamp_linear_velocity(10);
        int x, y;
        the_app->get_mouse_pos(x, y);
        
        float friction = 0.0f;
        if (the_app->is_key_down('A')) {
            player_rotate(player_node, angle_iteration);
        } else if (the_app->is_key_down('D')) {
            player_rotate(player_node, -angle_iteration);
        } else if (the_app->is_key_down('W')) {
          player_node->activate();
          player_node->apply_central_force(player_node->get_z() * (10.0f));
        } else if (the_app->is_key_down('S')) {
          player_node->activate();
          player_node->apply_central_force(player_node->get_z() * (-10.0f));
        } else {
          friction = 1.0f;
        }
        if (the_app->is_key_down(' ')) {
          player_node->activate();
          player_node->apply_central_force(player_node->get_y() * (+50.0f));
        }
        else if (the_app->is_key_down('R')) {
          player_node->activate();
          player_node->apply_central_force(player_node->get_y() * (-50.0f));
        }
        player_node->set_friction(friction);
        
        player_node->add_child(camera_node);
        camera_node->loadIdentity();
        camera_node->translate(vec3(0.0f, 30.0f, -30.0f));

        mat4t modelToWorld;
        modelToWorld.loadIdentity();
        modelToWorld[3] = vec4(camera_node->access_nodeToParent().w().x(), camera_node->access_nodeToParent().w().y(), camera_node->access_nodeToParent().w().z(), 1);
        modelToWorld.rotateY((float)-x*2.0f);
        modelToWorld.rotateX((float)-y*2.0f);
        /*if (vy / 2 - y < 70 && vy / 2 - y > -70)
          modelToWorld.rotateX((float)vy / 2 - y);
        if (vy / 2 - y >= 70)
          modelToWorld.rotateX(70);
        if (vy / 2 - y <= -70)
          modelToWorld.rotateX(-70);*/
        camera_node->access_nodeToParent() = modelToWorld;//apply to the node

      #endif
    }

    void player_rotate(scene_node *player_node, float angle){
      player_node->activate();
      btTransform trans = player_node->get_rigid_body()->getCenterOfMassTransform();
      btQuaternion transrot = trans.getRotation();
      btQuaternion rotquat;
      rotquat = rotquat.getIdentity();
      rotquat.setY(angle);
      transrot = rotquat * transrot;
      trans.setRotation(transrot);
      player_node->get_rigid_body()->setCenterOfMassTransform(trans);
    }
  };
}}
