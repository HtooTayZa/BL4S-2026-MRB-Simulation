#include "MRBActionInitialization.hh"
#include "MRBPrimaryGeneratorAction.hh"
#include "MRBEventAction.hh"
#include "MRBRunAction.hh"

MRBActionInitialization::MRBActionInitialization()
 : G4VUserActionInitialization()
{}

MRBActionInitialization::~MRBActionInitialization()
{}

// This method is called in multi-threaded mode for the master thread.
// It ensures that the RunAction (which handles the output CSV file) 
// is correctly initialized to aggregate data from all worker threads.
void MRBActionInitialization::BuildForMaster() const
{
  SetUserAction(new MRBRunAction());
}

// This method is called for each worker thread (or the main thread in sequential mode).
// It registers the core action classes that dictate the simulation's behavior.
void MRBActionInitialization::Build() const
{
  // 1. Primary Generator: Fires the 5 GeV Proton Beam
  SetUserAction(new MRBPrimaryGeneratorAction());
  
  // 2. Run Action: Manages the lifecycle of the simulation run and the output CSV file
  SetUserAction(new MRBRunAction());
  
  // 3. Event Action: Collects detector hits (multiplicity/energy) per event and writes to the CSV
  SetUserAction(new MRBEventAction());
}
