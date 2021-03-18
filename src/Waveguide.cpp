#include "Waveguide.hpp"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"

// create a single waveguide element
auto Waveguide::createWaveguide() -> G4AssemblyVolume* {

    // construct NIST manager for access to materials
    auto manager{G4NistManager::Instance()};
  
    // get the waveguide materials
    auto alumina{manager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE")};
    auto copper{manager->FindOrBuildMaterial("G4_Cu")};
    
    // create full solid rectangular shape
    auto solidWaveguide{new G4Box("solidWaveguide",
				  Waveguide::XWIDTH/2.,
				  Waveguide::YWIDTH/2.,
				  Waveguide::LENGTH/2.)};

    // create inner solid - will become the alumina bar
    auto solidWaveguideAl{new G4Box("solidWaveguideAl",
				    Waveguide::XWIDTH/2. - Waveguide::CuTHICKNESS,
				    Waveguide::YWIDTH/2. - Waveguide::CuTHICKNESS,
				    Waveguide::LENGTH/2. - Waveguide::CuTHICKNESS)};
    
    // hollow out the waveguide to get shell - will become the copper shell
    auto solidWaveguideCu{new G4SubtractionSolid("solidWaveguideCu",
						 solidWaveguide,
						 solidWaveguideAl)};

    // Assign materials to create the logical volumes
    // for alumina bar
    auto logicalWaveguideAl{new G4LogicalVolume(solidWaveguideAl, 
						alumina,
						"aluminaBar")};
    // for copper shell
    auto logicalWaveguideCu{new G4LogicalVolume(solidWaveguideCu,
						copper,
						"copperShell")};

    // create assembly volume to combine the bar and the shell
    auto waveguideAssembly{new G4AssemblyVolume()};

    // rotation for waveguide elements
    auto waveguideRotation{new G4RotationMatrix()};
    waveguideRotation->rotateX(M_PI/2.*CLHEP::rad);

    // a no translation vector
    auto translation{G4ThreeVector(0, 0, 0)};
    
    // add alumina bar to assembly
    waveguideAssembly->AddPlacedVolume(logicalWaveguideAl,
				       translation,
				       waveguideRotation);
    // and the copper shell
    waveguideAssembly->AddPlacedVolume(logicalWaveguideCu,
				       translation,
				       waveguideRotation);

    // return the waveguide assembly
    return waveguideAssembly;
}
