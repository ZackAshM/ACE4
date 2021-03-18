#include "EventAction.hpp"
#include "Analysis.hpp"
#include "UserEventInformation.hpp"
#include "UserTrackInformation.hpp"


EventAction::EventAction() : G4UserEventAction() {

    // TODO

}


EventAction::~EventAction() {

    // TODO

}

// get the hit collection id (HCID) for the current event
G4THitsMap<G4double>* 
EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const {
			       
    // TODO

}


void EventAction::BeginOfEventAction(const G4Event* /*event*/) {

    // create a User Event Information object
    G4EventManager::GetEventManager()->SetUserInformation(new UserEventInformation);

    // TODO
}


void EventAction::EndOfEventAction(const G4Event* event) {

    // TODO

}  

