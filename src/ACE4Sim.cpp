#include "QGSP_BERT.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4MTRunManager.hh"
#include "G4VisExecutive.hh"
#include "G4ScoringManager.hh"

#include "ACE4Sim.hpp"
#include "PhysicsList.hpp"
#include "DetectorConstruction.hpp"
#include "ActionInitialization.hpp"

// simulation start
auto main(const int argc, char **argv) -> int {

    // construct the default run manager
    auto runManager{new G4RunManager};

    // we must IMMEDIATELY construct the scoring manager
    auto scoringManager{G4ScoringManager::GetScoringManager()};
    (void)scoringManager;

    // construct the detector
    const auto detector{new DetectorConstruction};

    // register the geometry
    runManager->SetUserInitialization(detector);

    // create physics list
    runManager->SetUserInitialization(new QGSP_BERT);

    // create the actions and register them
    runManager->SetUserInitialization(new ActionInitialization(detector));

    // initialize the Geant4 kernel
    runManager->Initialize();

    // create vis manager
    auto visManager{new G4VisExecutive};
    visManager->Initialize();

    // get pointer to UI manager
    auto UImanager{G4UImanager::GetUIpointer()};

    // read a macro and execute it
    if (argc != 1) { //read a macro file of commands
	G4String command{"/control/execute "};
	G4String fileName{argv[1]};
	UImanager->ApplyCommand(command+fileName);
    }
    else { // we wish to run interactively
      UImanager->ApplyCommand("/control/macroPath ../macros");
      UImanager->ApplyCommand("/control/macroPath macros");
      auto ui{new G4UIExecutive(argc, argv)};
      ui->SessionStart();
      delete ui;
    }

    // cleanup
    delete runManager;

    // and we are done
    return 0;
}
