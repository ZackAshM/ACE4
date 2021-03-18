#include "TrackingAction.hpp"
#include "UserTrackInformation.hpp"
#include "DetectorConstruction.hpp"


TrackingAction::TrackingAction() {}



void TrackingAction::PreUserTrackingAction(const G4Track* /*aTrack*/)
{
    // create a user track info object
    fpTrackingManager->SetUserTrackInformation(new UserTrackInformation);

}



void TrackingAction::PostUserTrackingAction(const G4Track* /*aTrack*/) {}

