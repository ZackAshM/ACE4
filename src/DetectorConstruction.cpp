#include "Waveguide.hpp"
#include "DetectorConstruction.hpp"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"


using namespace CLHEP;

// create the physical world
auto DetectorConstruction::createPhysWorld() -> void {

    // create a rectangular world
  auto solidWorld{new G4Box("solidWorld", this->calXWORLD/2., this->calYWORLD/2., this->calZWORLD/2.)};

    // assume the world is made of vacuum
    auto vacuum{new G4Material("Vacuum", 1.0, 1.e-8*g/mole, 1.e-25*g/cm3,
				      kStateGas, 0.1*kelvin, 1.e-19*pascal)};

    // and create the world logical volume
    auto worldLV{new G4LogicalVolume(solidWorld, vacuum, "logicWorld")};
    this->logicWorld = worldLV;

    // insert waveguide assembly into logical volume
    // this->createWaveguides();

    // insert waveguide physical volumes into ACE_PVs map
    // this->ACE_PVs.insert (std::pair<int, G4VPhysicalVolume*>(0, this->pvStore->GetVolume("av_1_impr_1_aluminaBar_pv_0")) );
    // this->ACE_PVs.insert (std::pair<int, G4VPhysicalVolume*>(1, this->pvStore->GetVolume("av_1_impr_2_aluminaBar_pv_0")) );
    // this->ACE_PVs.insert (std::pair<int, G4VPhysicalVolume*>(2, this->pvStore->GetVolume("av_1_impr_3_aluminaBar_pv_0")) );

    // insert the ACECal
    this->createACECal();

    // insert waveguide physical volumes into ACE_PVs map
    int ACE_count{this->LAYERS * this->NELEMENTS};
    for (G4int it=2; it<=2*ACE_count; it+=2) {
      // find volume names using /vis/drawTree in simulation
      G4String volumeName{"av_1_impr_" + std::to_string(it) + "_aluminaBar_pv_0"};
      this->ACE_PVs.insert ( std::pair<int, G4VPhysicalVolume*>(it/2-1, this->pvStore->GetVolume(volumeName)) );
    }
    
    // construct a physical world placed at the origin
    G4VPhysicalVolume* world{new G4PVPlacement(0, // no rotation,
					       G4ThreeVector(0, 0, 0), // at the origin
					       this->logicWorld, "World",
					       0, false, 0, true)};

    // fill in the member variable
    this->physWorld = world;

}


// // create all of the waveguides
// auto DetectorConstruction::createWaveguides() -> void {

//     // construct the waveguide assembly (copper and alumina logical volumes)
//     auto wgAssembly{Waveguide::createWaveguide()};

//     // insert waveguide(s)                                                
//     for (G4int i=0; i<DetectorConstruction::NWAVEGUIDES; i++) {    // for each waveguide    

//       // get the spacing between each waveguide                           
//       G4double wgSpacing = DetectorConstruction::SPACING;

//       // waveguide position according to spacing                          
//       G4ThreeVector wgPos = G4ThreeVector(0,
//                                           0,
//                                           i*wgSpacing);  // stack along +z-axis

//       // insert (make imprints) waveguides into mother volume             
//       wgAssembly->MakeImprint(this->logicWorld,   // mother volume              
//                               wgPos,        // translation                
//                               0,            // rotation                   
//                               i,            // copy number                
//                               true);        // surf check

//     }
    
// }


// // create the tungsten bar
// auto DetectorConstruction::createTungBar() -> void {
//     // create solid bar
//     auto solidTungBar{new G4Box("solidTungBar", this->sTXWIDTH/2., this->sTYWIDTH/2., this->sTZWIDTH/2.)};
//     // create tungsten alloy elements
//     auto elFe{new G4Element("Iron", "Fe", 26., 55.85*g/mole)};
//     auto elW{new G4Element("Tungsten", "W", 74., 183.84*g/mole)};
//     auto elNi{new G4Element("Nickel", "Ni", 28., 58.6934*g/mole)};

//     // construct EF17tungstenAlloy
//     auto Walloy{new G4Material("EF17tungstenAlloy", 17.0*g/cm3, 3)};
//     Walloy->AddElement(elW, 90*perCent);
//     Walloy->AddElement(elFe, 3*perCent);
//     Walloy->AddElement(elNi, 7*perCent);

//     // get tungsten material
//     auto tungsten{G4Material::GetMaterial("EF17tungstenAlloy")};

//     // create logical tungsten bar
//     auto logicTungBar{new G4LogicalVolume(solidTungBar, tungsten, "logicTungBar")};

//     // tungsten bar rotation
//     auto rotTung{new G4RotationMatrix()};
//     rotTung->rotateX(M_PI/2.*CLHEP::rad);

//     // tungsten bar position
//     auto posTung{G4ThreeVector( 0, 0, -(this->sTSPACING) )};
    
//     // place tungsten bar
//     auto physTungBar{new G4PVPlacement(rotTung,               // rotation
// 				       posTung,               // translation
// 				       logicTungBar, "physTungBar",   // logic volume
// 				       this->logicWorld,      // mother volume
// 				       false, 0, true)};
  
// }


// create the ACE calorimeter
auto DetectorConstruction::createACECal() -> void {

  // construct the ACECal element assembly (copper, alumina, and tungsten volumes)
  auto calAssembly{new G4AssemblyVolume()};
  
  // construct the waveguide assembly (copper and alumina logical volumes)
    auto wgAssembly{Waveguide::createWaveguide()};

    // create solid bar for tungsten element
    auto solidTungBar{new G4Box("solidTungBar", this->calTXWIDTH/2., this->calTYWIDTH/2., Waveguide::LENGTH/2.)};
    
    // create tungsten alloy elements
    auto elFe{new G4Element("Iron", "Fe", 26., 55.85*g/mole)};
    auto elW{new G4Element("Tungsten", "W", 74., 183.84*g/mole)};
    auto elNi{new G4Element("Nickel", "Ni", 28., 58.6934*g/mole)};
    // construct EF17tungstenAlloy
    auto Walloy{new G4Material("EF17tungstenAlloy", 17.0*g/cm3, 3)};
    Walloy->AddElement(elW, 90*perCent);
    Walloy->AddElement(elFe, 3*perCent);
    Walloy->AddElement(elNi, 7*perCent);
    // get tungsten material
    auto tungsten{G4Material::GetMaterial("EF17tungstenAlloy")};

    // create logical tungsten bar
    auto logicTungBar{new G4LogicalVolume(solidTungBar, tungsten, "logicTungBar")};

    // tungsten bar rotation
    auto rotTung{new G4RotationMatrix()};
    rotTung->rotateX(M_PI/2.*CLHEP::rad);
    
    // get the element and layer spacings
    G4double xSpacing{DetectorConstruction::XSPACING};
    G4double layerSpacing{DetectorConstruction::LAYERSPACING};

    // construct the ACECal
    if (!SINGLETUNG) { // tungsten w/ each layer

      // combine the tungsten and waveguide into the ACECal assembly
      G4ThreeVector wgPos{G4ThreeVector(0, 0, this->calTYWIDTH + this->T2WGGAP + 0.5*Waveguide::YWIDTH)};
      calAssembly->AddPlacedAssembly(wgAssembly, wgPos, 0);
      G4ThreeVector tPos{G4ThreeVector(0, 0, 0.5*this->calTYWIDTH)};
      calAssembly->AddPlacedVolume(logicTungBar, tPos, rotTung);
      // (0,0,0) for this assembly should be on the open face of the tungsten bar

      // place every ACECal element
      G4int copy{0};
      for (G4int layer=0; layer<DetectorConstruction::LAYERS; layer++) { // for each layer

	for (G4int n=0; n<DetectorConstruction::NELEMENTS; n++) { // for each element   

	  // set element position                 
	  G4ThreeVector calPos{G4ThreeVector(-0.5*this->calXWORLD + 0.5*this->calTXWIDTH + n*xSpacing + this->OFFSET*( 1 + layer%2 ) + this->calTXWIDTH*((DetectorConstruction::NELEMENTS+1)%2),
					     0,
					     -0.5*this->calZWORLD + this->BEAMSPACE + layer*layerSpacing) };  // stack along +z-axis

	  // insert (make imprints) waveguides into mother volume             
	  calAssembly->MakeImprint(this->logicWorld,   // mother volume              
				   calPos,             // translation                
				   0,                  // rotation                   
				   copy,               // copy number                
				   true);              // surf check
	
	} // END for (G4int layer...
      } // END for (G4int n...
    } // END for if (!SINGLETUNG)
    else { // a single tungsten bar in front, each layer only contains the waveguides

      // start assembly
      G4ThreeVector wgPos{G4ThreeVector(0, 0, 0.5*Waveguide::YWIDTH)};
      calAssembly->AddPlacedAssembly(wgAssembly, wgPos, 0);
      // (0,0,0) for this assembly should be on the open face of the waveguide

      // get tungsten to waveguide spacing
      G4double tSpacing{DetectorConstruction::sTSPACING};

      // place ACECal elements
      G4int copy{0};
      for (G4int layer=0; layer<DetectorConstruction::LAYERS; layer++) { // for each layer

	for (G4int n=0; n<DetectorConstruction::NELEMENTS; n++) { // for each element   

	  // set and place tungsten bar(s)
	  if (layer == 0) {
	    G4ThreeVector tPos{G4ThreeVector(-0.5*this->calXWORLD + 0.5*this->sTXWIDTH + n*xSpacing + this->OFFSET*( 1 + layer%2 ) + this->sTXWIDTH*((DetectorConstruction::NELEMENTS+1)%2), 
					     0, 
					     -0.5*this->calZWORLD + this->BEAMSPACE + 0.5*this->sTYWIDTH)};
	    auto physTungBar{new G4PVPlacement(rotTung,               // rotation
					       tPos,                  // translation
					       logicTungBar, "physTungBar",   // logic volume
					       this->logicWorld,      // mother volume
					       false, 0, true)};
	  }

	  // set waveguide position                 
	  G4ThreeVector calPos{G4ThreeVector(-0.5*this->calXWORLD + 0.5*this->sTXWIDTH + n*xSpacing + this->OFFSET*( 1 + layer%2 ) + this->sTXWIDTH*((DetectorConstruction::NELEMENTS+1)%2),
					     0,
					     -0.5*this->calZWORLD + this->BEAMSPACE + 0.5*this->sTYWIDTH + tSpacing - Waveguide::YWIDTH/2. + layer*layerSpacing) };  // stack along +z-axis

	  // insert (make imprints) waveguides into mother volume             
	  calAssembly->MakeImprint(this->logicWorld,   // mother volume              
				   calPos,             // translation                
				   0,                  // rotation                   
				   copy,               // copy number                
				   true);              // surf check
	
	} // END for (G4int layer...
      } // END for (G4int n...

    }
}


// construct the geometry
auto DetectorConstruction::Construct() -> G4VPhysicalVolume* {
  
    // create physical volume store to contain all physical volumes
    auto physVolStore{G4PhysicalVolumeStore::GetInstance()};
    this->pvStore = physVolStore;
    
    // create the world volume (contains the waveguides)
    this->createPhysWorld();

    // create the tungsten bar
    // this->createTungBar();

    // must return the physWorld
    return this->physWorld;
}


// create sensitive detectors and fields
auto DetectorConstruction::ConstructSDandField() -> void {

    // MUST IMPLEMENT

}
