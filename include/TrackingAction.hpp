#pragma once

#include "G4UserTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

class TrackingAction final : public G4UserTrackingAction {

public:
    TrackingAction();
    virtual ~TrackingAction() {};

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

private:

};

