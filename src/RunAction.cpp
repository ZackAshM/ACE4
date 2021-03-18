#include <sstream>
#include <unistd.h>

#include "Analysis.hpp"
#include "RunAction.hpp"
#include "DetectorConstruction.hpp"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

auto RunAction::BeginOfRunAction(const G4Run* /*run*/) -> void {

    // Create analysis manager
    auto analysisManager{G4AnalysisManager::Instance()};
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);

    // set the default filename - this can be overriden
    analysisManager->SetFileName("ACE4Sim.root");
    
    analysisManager->OpenFile();

    // create the NTuple to store charged tracks
    this->CreateTrackNTuple();

    // create the ACE 2D charge histograms
    this->CreateChargedTrackHistograms();

    // create the hit times histogram
    this->CreateTimeHistograms();

    // create the NTuple to store total kinetic energy in each ACE element
    this->CreateEnergyNTuple();
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
auto RunAction::EndOfRunAction(const G4Run* /*run*/) -> void {

    // get an instance of the analysis manager
    auto analysisManager{G4AnalysisManager::Instance()};

    // write the output file
    analysisManager->Write();

    // and close it down
    analysisManager->CloseFile();

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
auto RunAction::CreateTrackNTuple() const -> void {

    // get an instance of the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // and create the ntuple
    analysisManager->CreateNtuple("ChargedTracks", "Charged tracks that intersect ACE elements");

    // the event ID
    analysisManager->CreateNtupleFColumn("event");

    // and the particle charged
    analysisManager->CreateNtupleFColumn("charge");

    // and which ACE element was hit
    analysisManager->CreateNtupleFColumn("element");

    // the location of the start of the track
    analysisManager->CreateNtupleFColumn("start.x");
    analysisManager->CreateNtupleFColumn("start.y");
    analysisManager->CreateNtupleFColumn("start.z");
    analysisManager->CreateNtupleFColumn("start.t");

    // the location of the end of the track
    analysisManager->CreateNtupleFColumn("end.x");
    analysisManager->CreateNtupleFColumn("end.y");
    analysisManager->CreateNtupleFColumn("end.z");
    analysisManager->CreateNtupleFColumn("end.t");

    // and we are done with this Ntuple
    analysisManager->FinishNtuple();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
auto RunAction::CreateChargedTrackHistograms() const -> void {

    // the number of bins in each axis of the histogram
    static constexpr G4int nbinx{480};   // 0.5 sized bins
    static constexpr G4int nbiny{480};

    // the minimum and maximum range for each axis of the histogram
    static constexpr G4double minx{-60.}; static constexpr G4double maxx{60.};
    static constexpr G4double miny{-60.}; static constexpr G4double maxy{60.};

    // get an instance of the analysis manager
    auto analysisManager{G4AnalysisManager::Instance()};

    // get the total number of waveguides
    // static G4double wgNum{DetectorConstruction::NWAVEGUIDES};
    static G4double wgNum{DetectorConstruction::LAYERS * DetectorConstruction::NELEMENTS};  // for ACECal

    // create the total charge histograms for all the ACE element.
    for (int n = 0; n < wgNum; n++) {

	// we want a total and net charge histogram
	for (const auto& kind : { "Total", "Excess" }) {

	    // create the name and description programmatically
	    std::stringstream name;
	    std::stringstream desc;

	    // and build up the name
	    name << kind << "ACE" << n << "Charge";
	    desc << kind << " Charge in ACE " << n;

	    // and construct the histogram
	    analysisManager->CreateH2(name.str().c_str(), desc.str().c_str(),
				      nbinx, minx, maxx,
				      nbiny, miny, maxy);

	} // END: for (const auto& kind : { "Total", "Excess" })

    } // END: for (int n = 0; n < DetectorConstruct::NumElements; n++)

}

auto RunAction::CreateTimeHistograms() const -> void {

    // the number of bins in each axis of the histogram
    static constexpr G4int nbins{2000};  // 0.01 sized bins

    // the min max range of the histogam
    static constexpr G4double mint{0.};
    static constexpr G4double maxt{5./*1.5*/};

    // get an instance of the analysis manager
    auto analysisManager{G4AnalysisManager::Instance()};

    // get the total number of waveguides
    // static G4double wgNum{DetectorConstruction::NWAVEGUIDES};
    static G4double wgNum{DetectorConstruction::LAYERS * DetectorConstruction::NELEMENTS};  // for ACECal

    // create the total charge histograms for all the ACE element.
    for (int n = 0; n < wgNum; n++) {

	// create the name/desc of the histogram programmatically
	std::stringstream name;
	std::stringstream desc;

	// build the name
	name << "HitTimesACE" << n;
	desc << "Hit Times in ACE" << n;

	// and construct the histogram
	analysisManager->CreateH1(name.str().c_str(), desc.str().c_str(),
				  nbins, mint, maxt);

    }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
auto RunAction::CreateEnergyNTuple() const -> void {

    // get an instance of the analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // and create the ntuple
    analysisManager->CreateNtuple("Energy", "Total Kinetic energy through each ACE element");

    // a column for each ACE element
    static const G4int noACE{DetectorConstruction::LAYERS * DetectorConstruction::NELEMENTS};
    for (G4int ace = 0; ace < noACE; ace++ ){
      analysisManager->CreateNtupleFColumn("ACE" + std::to_string(ace+1));
    }

    // and a column for all ACE elements
    analysisManager->CreateNtupleFColumn("Total"); // index = noACE

    // and we are done with this Ntuple
    analysisManager->FinishNtuple();
    
}
