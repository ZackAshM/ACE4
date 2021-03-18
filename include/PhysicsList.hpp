#pragma once

#include "G4VUserPhysicsList.hh"

class PhysicsList final : public G4VUserPhysicsList {

public:
    
    PhysicsList() {};

    // necessary methods
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

    // construct physics processes
    void ConstructOp();
    void ConstructEM();
};

