////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Big bad boss ship - stay clear of this
//
#ifndef ENEMY_BOSS_H_INCLUDED
#define ENEMY_BOSS_H_INCLUDED

namespace octet {

  class boss_ship : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships bossSpaceShip;
    ai_behaviours ai;

    ref<scene_node> ship_node;
    
    enum bossState{WANDERING, SEEKING, CAPTURE};

    //consider changing the boss ships agro range
    const float agro_range = 40.0f;
    const float capture_range = 5.0f;


  public:
    boss_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      bossSpaceShip.init(the_app, app_scene);
      ship_node = bossSpaceShip.create_boss_enemy();
    }

    void init_boss_enemy(){
      
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    void update(dynarray<scene_node*> civilians, dynarray<scene_node*> enemies, scene_node *player_ship, float player_orientation){
      //activate bullet physics
      ship_node->activate();
      ship_node->set_damping(0.5f, 0.5f);
      ship_node->set_friction(1.0f);

      vec3 player_position = player_ship->get_position();
      //we're only interested in the difference in the x and z axis
      vec3 facingVec = player_position - ship_node->get_position();
      float diff_x = facingVec.x();
      float diff_z = facingVec.z();
      ai.wander(ship_node);

    }

    ~boss_ship() {
    }
  };
}

#endif
