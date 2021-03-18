#include "PhysicsList.hpp"

#include "G4OpBoundaryProcess.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4GammaConversion.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4Scintillation.hh"
#include "G4VModularPhysicsList.hh"
#include "G4Cerenkov.hh"

void PhysicsList::ConstructParticle() {

    // all particles in the simulation are constructed here
    G4BosonConstructor bConstructor;
    bConstructor.ConstructParticle();

    // construct leptons
    G4LeptonConstructor lConstructor;
    lConstructor.ConstructParticle();

    // and ions
    G4IonConstructor iConstructor;
    iConstructor.ConstructParticle();

    // positron's and electrons
    G4Positron::PositronDefinition();
    G4Electron::ElectronDefinition();

    // proton's and anti-protons
    G4Proton::ProtonDefinition();
    G4AntiProton::AntiProtonDefinition();

    // geantino for debuggin
    G4Geantino::GeantinoDefinition();
    

}

void PhysicsList::ConstructProcess() {
    AddTransportation();
    ConstructEM();
    ConstructOp();
}

// construct optical physics processes
void PhysicsList::ConstructOp() {

    // TODO

}

void PhysicsList::ConstructEM() {

    // TODO: Construct all EM processes

}

void PhysicsList::SetCuts() {

    // TODO: Set cuts for processes

    SetCutsWithDefault();
}
