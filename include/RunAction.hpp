#pragma once

#include "G4Run.hh"
#include "G4UserRunAction.hh"

#include "Analysis.hpp"


///
/// \brief A 1d histogram enum to index into the analysis manager.
///
enum Hist1D {
    HitTimesACE1 = 0,
    HitTimesACE2 = 1,
    HitTimesACE3 = 2
};

///
/// \brief A 2d histogram enum to index into the analysis manager.
///
/// This must exactly match the order in which the histograms are created.
///
enum Hist2D {
    TotalChargeACE1 = 0,
    ExcessChargeACE1 = 1,
    TotalChargeACE2 = 2,
    ExcessChargeACE2 = 3,
    TotalChargeACE3 = 4,
    ExcessChargeACE3 = 5,

};


///
/// \brief Primary Run Action
///
class RunAction final : public G4UserRunAction {

    ///
    /// \brief Create the NTuple to store charged tracks in the elements.
    ///
    auto CreateTrackNTuple() const -> void;

    ///
    /// \brief Create the 2D total and excess charge histograms.
    ///
    auto CreateChargedTrackHistograms() const -> void;

    ///
    /// \brief Create the histogram of time hits in the ACE elements.
    ///
    auto CreateTimeHistograms() const -> void;

    ///
    /// \brief Create the NTuple to store total kinetic energy in each ACE element.
    ///
    auto CreateEnergyNTuple() const -> void;
    
public:

    ///
    /// \brief Construct a new Run Action.
    ///
    RunAction() {};

    ///
    /// \brief Virtual destructor.
    ///
    ~RunAction() { delete G4AnalysisManager::Instance(); };

    ///
    /// \brief The beginning of a run.
    ///
    /// This creates the analysis manager, histograms,
    /// and Ntuples.
    ///
    auto BeginOfRunAction(const G4Run*) -> void;

    ///
    /// \brief The end of a run.
    ///
    /// This is responsible for writing/closing down
    /// the output files and destroying the analysis manager.
    ///
    auto EndOfRunAction(const G4Run*) -> void;
    
}; // END: class RunAction final

