#pragma once

#include "G4VUserActionInitialization.hh"

#include "DetectorConstruction.hpp"

///
/// \brief Initialize actions for worker and master threads.
///
class ActionInitialization final : public G4VUserActionInitialization {

    ///
    /// \brief The initialized DetectorConstruction instance.
    ///
    const DetectorConstruction* detector;

public:

    ///
    /// \brief Construct an action initialization.
    ///
    ActionInitialization(const DetectorConstruction* detectorConstruct) :
	detector(detectorConstruct) {};

    ///
    /// \brief Create actions for the master thread.
    ///
    auto BuildForMaster() const -> void;

    ///
    /// \brief Create actions for all worker threads.
    ///
    auto Build() const -> void;

};

