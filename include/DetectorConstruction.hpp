#pragma once

#include <map>
#include "Waveguide.hpp"
#include "G4SDManager.hh"
#include "G4AssemblyVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VUserDetectorConstruction.hh"

using CLHEP::eV;

///
/// \brief Create the detector geometry.
///
class DetectorConstruction final : public G4VUserDetectorConstruction {

    ///
    /// \brief The minimum and maximum optical photon energies (eV).
    ///
    const G4double photonEnergyRange[2]{(1240.7*eV)/320., (1240.7*eV)/1000.};

    ///
    /// \brief The sampled optical photon energy spectrum (eV).
    ///
    const G4double energySpan[35]{(1240.7*eV)/320., (1240.7*eV)/340., (1240.7*eV)/360., (1240.7*eV)/380.,
	    (1240.7*eV)/400., (1240.7*eV)/420., (1240.7*eV)/440., (1240.7*eV)/460., (1240.7*eV)/480.,
	    (1240.7*eV)/500., (1240.7*eV)/520., (1240.7*eV)/540., (1240.7*eV)/560., (1240.7*eV)/580.,
	    (1240.7*eV)/600., (1240.7*eV)/620., (1240.7*eV)/640., (1240.7*eV)/660., (1240.7*eV)/680.,
	    (1240.7*eV)/700., (1240.7*eV)/720., (1240.7*eV)/740., (1240.7*eV)/760., (1240.7*eV)/780.,
	    (1240.7*eV)/800., (1240.7*eV)/820., (1240.7*eV)/840., (1240.7*eV)/860., (1240.7*eV)/880.,
	    (1240.7*eV)/900., (1240.7*eV)/920., (1240.7*eV)/940., (1240.7*eV)/960., (1240.7*eV)/980.,
	    (1240.7*eV)/1000.}; // in eV

    // world full-sizes (NOT half-sizes)
    static constexpr G4double XWORLD{0.035*CLHEP::m}; ///< The full-width of the x-dimension world.
    static constexpr G4double YWORLD{0.9145*CLHEP::m}; ///< The full-width of the y-dimension world.
    static constexpr G4double ZWORLD{0.25*CLHEP::m}; ///< The full-width of the z-dimension world.

    ///
    /// \brief the logical world containing the detector geometry.
    ///
    G4LogicalVolume* logicWorld;
  
    ///
    /// \brief The physical world containing the detector geometry.
    ///
    G4VPhysicalVolume* physWorld;

    ///
    /// \brief Construct the solid, logic, and physical worlds.
    ///
    /// This uses the full sizes XWORLD, YWORLD, ZWORLD for dimensions.
    ///
    auto createPhysWorld() -> void;

    ///
    /// \brief Construct the waveguide assembly.
    ///
    auto createWaveguides() -> void;

    ///
    /// \brief Construct tungsten bar to scatter e-
    ///
    auto createTungBar() -> void;

  ///
  /// \brief Construct a calorimeter made from ACE elements.
  ///
  auto createACECal() -> void;
    
public:

    // multiple copy properties (assuming copies along z-axis)
    // static constexpr G4int NWAVEGUIDES{3}; ///< Total number of waveguides to be created
    // static constexpr G4double AIRGAP{1.27*CLHEP::mm}; ///< The airgap between waveguides
    // static constexpr G4double SPACING{AIRGAP + Waveguide::YWIDTH}; ///< Spacing between each waveguide center

  ///
  /// \brief ACECal properties
  ///
  /// Each layer of the calorimeter will consist of a layer
  /// of tungsten elements followed by a layer of waveguide
  /// elements (unless SINGLETUNG==true). Each ACECal layer 
  /// can be offset from each other by specifying OFFSET
  static constexpr G4int LAYERS{3}; ///< The number of layers in the ACECal to be created.
  static constexpr G4int NELEMENTS{1}; ///< The number of elements in each layer.
  static constexpr G4bool SINGLETUNG{true}; ///< If true, only one total layer of tungsten is placed - before the first ACE element
  static constexpr G4double AIRGAP{1.27*CLHEP::mm}; ///< The airgap between waveguides - larger airgaps are in units of this defined airgap
  static constexpr G4double LAYERGAP{5*AIRGAP}; ///< The airgap between layers in the ACECal.
  
  static constexpr G4double XGAP{AIRGAP}; ///< The x airgap between each element in a layer of the ACECal.
  static constexpr G4double EXCESS{XGAP}; ///< The excess x-width of the tungsten over the ACE element; must be <= XGAP to avoid overlap. 
  static constexpr G4double calTXWIDTH{Waveguide::XWIDTH + EXCESS}; ///< The full x-width of the tungsten elements in the ACECal
  static constexpr G4double calTYWIDTH{9.946*CLHEP::cm}; ///< The full y-width or depth of the tungsten elements in the ACECal, 9.946cm=1nucl.interaction.len., 3.5mm=1rad.len.
  static constexpr G4double ELXWIDTH{Waveguide::XWIDTH}; ///< The x width of an element in a layer.
  static constexpr G4double T2WGGAP{AIRGAP}; ///< The airgap between tungsten and waveguide elements in a layer.
  static constexpr G4double T2WGSPACING{0.5*calTYWIDTH + T2WGGAP + 0.5*Waveguide::YWIDTH};
  static constexpr G4double LAYERSPACING{int(!SINGLETUNG) * (calTYWIDTH + T2WGGAP) + Waveguide::YWIDTH + LAYERGAP}; ///< The spacing between each center of a layer in the calorimeter.
  static constexpr G4double XSPACING{XGAP + ELXWIDTH}; ///< The x spacing between each center of elements in an ACECal layer.
  static constexpr G4double OFFSET{0*ELXWIDTH}; ///< The offset of layers in the ACECal.
  static constexpr G4double BEAMSPACE{5*CLHEP::cm}; //< The full spacing between the edge of the world and the face of the first ACECal layer.

  ///
  /// \brief Tungsten bar properties, when SINGLETUNG == true
  ///
  static constexpr G4double sTXWIDTH{Waveguide::XWIDTH}; ///< The full-width of the tungsten bar in the x-direction
  static constexpr G4double sTYWIDTH{9.946*CLHEP::cm}; ///< The full-width of the tungsten bar in the y-direction, 9.946cm=1nucl.interaction.len., 3.5mm=1rad.len.
  static constexpr G4double sTZWIDTH{50*CLHEP::cm}; ///< The full-width of the tungsten bar in the z-direction
  static constexpr G4double sTAIRGAP{5*AIRGAP}; ///< The length of the gap between the tungsten bar and first waveguide element
  static constexpr G4double sTSPACING{sTYWIDTH/2. + Waveguide::YWIDTH/2. + sTAIRGAP}; ///< The full distance of the center of the tungsten bar from the center of the first waveguide element

  ///
  /// \brief The dimensions of the world
  ///
  static constexpr G4double calXWORLD{NELEMENTS * (ELXWIDTH + XGAP) - XGAP + 2*OFFSET + EXCESS + calTXWIDTH*((NELEMENTS+1)%2)}; ///< The full-width of the x-dimension world; fit to ACECal.
  static constexpr G4double calYWORLD{1.05*Waveguide::LENGTH}; ///< The full-width of the y-dimension world; fit to ACECal.
  static constexpr G4double calZWORLD{BEAMSPACE + LAYERS * (Waveguide::YWIDTH + int(!SINGLETUNG) * (T2WGGAP + calTYWIDTH) + LAYERGAP) 
      - LAYERGAP + int(SINGLETUNG) * (sTAIRGAP + sTYWIDTH)}; ///< The full-width of the z-dimension world; fit to ACECal.

  
    ///
    /// \brief A physical volume storage to contain all physical volumes
    ///
    G4PhysicalVolumeStore* pvStore;
  
    ///
    /// \brief A map from ACE ID to G4PhysicalVolume
    ///
    std::map<int, G4VPhysicalVolume*> ACE_PVs;

    ///
    /// \brief Construct the geometry.
    ///
    /// This returns the physical world volume.
    ///
    auto Construct() -> G4VPhysicalVolume*;

    ///
    /// \brief Construct all sensitive detectors and EM fields.
    ///
    auto ConstructSDandField() -> void;

};

