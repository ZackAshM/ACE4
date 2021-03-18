#include "ActionInitialization.hpp"

#include "PrimaryGeneratorAction.hpp"
#include "RunAction.hpp"
#include "EventAction.hpp"
#include "SteppingAction.hpp"
#include "TrackingAction.hpp"

// initialize actions for the master thread
auto ActionInitialization::BuildForMaster() const -> void {
    SetUserAction(new RunAction);
}

// initialize actions for worker threads
auto ActionInitialization::Build() const -> void {
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction);
    SetUserAction(new EventAction);
    SetUserAction(new SteppingAction(this->detector));
    SetUserAction(new TrackingAction);
}
