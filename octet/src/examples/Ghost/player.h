////////////////////////////////////////////////////////////////////////////////
//
// (C) Ryan Singh 2015
//
// Player class
//
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
namespace octet {

  ///Class to create a vehicle using hinge constraints.
  ///Class will create a vehicle out of rigid bodies. The vehicle consists of a chassis, 4 axils and 4 wheels.
  ///They are attached using hinge constraints. There are 2 hinge constraint relationships, Chassis-Axils and Axils-Wheels.
  class Player : public resource {


  public:

    Player()
    {
    }

    ~Player() {
    }
  };
}

#endif
