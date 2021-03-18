#pragma once

#include "G4VUserTrackInformation.hh"

class UserTrackInformation final : public G4VUserTrackInformation
{
  public:

    UserTrackInformation() {};
    virtual ~UserTrackInformation() {};

    void Print() const override {};

  private:

    
};
