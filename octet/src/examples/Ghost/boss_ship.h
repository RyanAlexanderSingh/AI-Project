////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Big bad boss ship - stay clear of this
//
#ifndef ENEMY_BOSS_H_INCLUDED
#define ENEMY_BOSS_H_INCLUDED

#include <random>

namespace octet {

  class boss_ship : public resource {

    app *the_app;
    visual_scene *app_scene;

    ships bossSpaceShip;
    ai_behaviours ai;

    ref<scene_node> ship_node;

    enum bossState{ WANDERING, SEEKING, DESTROY, DEAD };
    bossState state;

    //consider changing the boss ships agro range
    const float sq_agro_range = 40.0f*40.0f;
    const float sq_capture_range = 5.0f*5.0f;


  public:
    boss_ship(){}

    void init(app *app, visual_scene *vs){
      this->the_app = app;
      this->app_scene = vs;

      bossSpaceShip.init(the_app, app_scene);
      ship_node = bossSpaceShip.create_boss_enemy();
    }

    scene_node *return_ship_node(){
      return ship_node;
    }

    void deactive_bossShip(){
      state = DEAD;
    }

    void update(dynarray<civilian_ship*> civilians, dynarray<scene_node*> enemies, scene_node *player_ship, float player_orientation){
      //activate bullet physics
      if (state != DEAD){
        state = WANDERING; //default behaviour
        float lineWidth = 1.0f;
        ship_node->activate();
        ship_node->set_damping(0.5f, 0.5f);
        ship_node->set_friction(1.0f);

        //check the civilians, the boss is nicer to the civilians but he'll still probably kill them
        for (unsigned i = 0; i < civilians.size(); ++i){
          scene_node *civilianSceneNode = civilians[i]->return_ship_node();
          vec3 distanceVec = civilianSceneNode->get_position() - ship_node->get_position();
          //if the civilians are within the agro range
          if (distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_agro_range){
            int attackChance = rand() % 100 + 1;
            if (distanceVec.x()*distanceVec.x() + distanceVec.z()*distanceVec.z() < sq_capture_range && attackChance >= 70){
              printf("Attack Chance: %i\n", attackChance);
              state = DESTROY;
              //do something here to destroy the civilians 
              civilians[i]->deactive_ship();
            }
            else{
              state = SEEKING;
              ai.seek(ship_node, civilianSceneNode, 8.0f);
            }
          }
        }
        if (state == WANDERING){
          ai.wander(ship_node);
        }
        switch (state){
        case WANDERING:
          lineWidth = 1.0f;
          break;
        case SEEKING:
          lineWidth = 2.0f;
          break;
        }
        bossSpaceShip.statusCircle(lineWidth, 40.0f, ship_node, player_ship, player_orientation);
      }
      else if(state == DEAD){
        ship_node->set_position(vec3(500.0f, 0.0f, 500.0f));
      }
    }

    ~boss_ship() {
    }
  };
}

#endif
