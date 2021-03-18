#include <cmath>

#include "SteppingAction.hpp"
#include "EventAction.hpp"
#include "UserEventInformation.hpp"
#include "UserTrackInformation.hpp"
#include "Analysis.hpp"


// process all step-related physics
auto SteppingAction::UserSteppingAction(const G4Step* step) -> void {

  // check if the step hits any of the ACE elements
  this->CheckForACETracks(step);
    
}

// check if the track intersects any of the ACE elements
auto SteppingAction::CheckForACETracks(const G4Step* step) -> void {

  // get analysis manager
  auto analysisManager{G4AnalysisManager::Instance()};

  // get the particle charge
  const auto charge{step->GetTrack()->GetDefinition()->GetPDGCharge()};

  // if the charge is zero, we ignore this hit entirely
  if ( charge == 0.0 ) return;

  // and the associated steps
  const auto prestep{step->GetPreStepPoint()};
  const auto poststep{step->GetPostStepPoint()};

  // get volume of the current step
  const auto volume{prestep->GetTouchableHandle()->GetVolume()};

  // get the total number of waveguides
  // static G4double wgNum{DetectorConstruction::NWAVEGUIDES};
  static const G4int wgNum{DetectorConstruction::LAYERS * DetectorConstruction::NELEMENTS};  // for ACECal
  
  // iterate over all the defined waveguides
  for (int i = 0; i < wgNum; i++) {

    // check that the track is leaving the waveguide
    if ( volume == this->detector->ACE_PVs.at(i) ) {
	
      // get the sign of the charge
      const auto csign{charge/std::fabs(charge)};

      // get the time of the hit
      const auto pretime{prestep->GetGlobalTime()};
      const auto posttime{poststep->GetGlobalTime()};

      // get position of the hit
      const auto prepos{prestep->GetPosition()};
      const auto postpos{poststep->GetPosition()};

      // get the incoming energy of the hit
      const auto energy{prestep->GetKineticEnergy()};

      // fill in element histogram
      analysisManager->FillNtupleFColumn(0, 2, i);

      // fill in charge histograms - particle charge in e- units
      analysisManager->FillNtupleFColumn(0, 1, csign);
      
      // fill in total and excess charge histogram
      analysisManager->FillH2(2*i, prepos.getX(), prepos.getY()); // total
      
      // excess computed by weighing electrons +1 and positron -1
      analysisManager->FillH2(2*i + 1, prepos.getX(), prepos.getY(), -1.*csign);
      
      // fill in time histograms
      analysisManager->FillNtupleFColumn(0, 6, pretime);
      analysisManager->FillNtupleFColumn(0, 10, posttime);
      analysisManager->FillH1(i, pretime);

      // fill in position histograms
      analysisManager->FillNtupleFColumn(0, 3, prepos.getX());
      analysisManager->FillNtupleFColumn(0, 4, prepos.getY());
      analysisManager->FillNtupleFColumn(0, 5, prepos.getZ());
      analysisManager->FillNtupleFColumn(0, 7, postpos.getX());
      analysisManager->FillNtupleFColumn(0, 8, postpos.getY());
      analysisManager->FillNtupleFColumn(0, 9, postpos.getZ());

      // fill in energy Ntuple
      analysisManager->FillNtupleFColumn(1, i, energy/CLHEP::TeV);
      analysisManager->FillNtupleFColumn(1, wgNum, energy/CLHEP::TeV);

      // save the row for Ntuple:0
      analysisManager->AddNtupleRow(0);
      analysisManager->AddNtupleRow(1);
	    
      } // END: if ( volume == this->detector.ACE_PVs[i] )

  } // END: for (int i = 0; i < DetectorConstruction::NWAVEGUIDES; i++)

}
