#include "PrimaryGeneratorAction.hpp"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "DetectorConstruction.hpp"
#include "G4ParticleTable.hh"
#include "CLHEP/Random/Randomize.h"

G4GeneralParticleSource* PrimaryGeneratorAction::particleGun(0);

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    particleGun = new G4GeneralParticleSource();

    // set default kinematics:
    auto def_particle{"e-"/*proton*/}; // default particle type
    auto def_energy{10*CLHEP::TeV}; // default beam energy
    
    G4ParticleDefinition* particleDefinition                                      
      = G4ParticleTable::GetParticleTable()->FindParticle(def_particle); // get default particle
    auto pos{G4ThreeVector(0, 0, -0.5*DetectorConstruction::calZWORLD + 0.5*DetectorConstruction::BEAMSPACE)};   // source 1 position
    auto mom{G4ThreeVector(0,0,1)};   // source 1 momentum direction
    auto ene{def_energy};   // source 1 energy
    
    // first source
    // particleGun->AddaSource(1.0);    // an initial source is created automatically
    particleGun->SetCurrentSourceto(0);
    auto firstSource{particleGun->GetCurrentSource()};
    // set to proton
    firstSource->SetParticleDefinition(particleDefinition);
    // set momentum direction
    firstSource->GetAngDist()->SetAngDistType("planar");
    firstSource->GetAngDist()->SetParticleMomentumDirection(mom);
    // set energy
    firstSource->GetEneDist()->SetEnergyDisType("Mono");
    firstSource->GetEneDist()->SetMonoEnergy(ene);
    // set position
    firstSource->GetPosDist()->SetPosDisType("Point");    
    firstSource->GetPosDist()->SetCentreCoords(pos);
    
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    particleGun->GeneratePrimaryVertex(anEvent);
}
