#pragma once

#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"

class UserEventInformation : public G4VUserEventInformation
{
  public:

    UserEventInformation() {};
    virtual ~UserEventInformation() {};

    void Print() const override {};

  private:

};
