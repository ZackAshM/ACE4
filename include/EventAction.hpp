#pragma once

#include "G4UserEventAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

class EventAction final : public G4UserEventAction {

public:
    
    EventAction();
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
private:
    // methods
    G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
					    const G4Event* event) const;

  
    // data members
    G4int fPhotonsDetectedHCID;
    G4int fPhotonsProducedHCID;
};


    
