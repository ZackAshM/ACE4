#pragma once

#include "G4UserSteppingAction.hh"

#include "RunAction.hpp"
#include "DetectorConstruction.hpp"

///
/// \brief Particle Stepping Action
///
/// This currently only handles checking for charged tracks
/// in ACE waveguides.
///
class SteppingAction final : public G4UserSteppingAction {

    ///
    /// \brief The initialized detector construction.
    ///
    const DetectorConstruction* detector;

    ///
    /// \brief Save this track if it intersects an ACE element.
    ///
    auto CheckForACETracks(const G4Step* step) -> void;

  public:

    ///
    /// \brief Create a new stepping action.
    ///
    /// @param detector   The initialized detector construction.
    ///
    SteppingAction(const DetectorConstruction* detectorConstruct) : detector(detectorConstruct) {};

    ///
    /// \brief Destructor.
    ///
    virtual ~SteppingAction() {};

    ///
    /// \brief The nitty-gritty stepping action.
    ///
    /// This currently checks if each step is inside
    /// an ACE element and if so saves it to the
    /// appropriate event collection.
    ///
    auto UserSteppingAction(const G4Step*) -> void;

  ///
  /// \brief Keeps track of the total energy.
  ///
  G4double totalEnergy{0*CLHEP::TeV};

};

