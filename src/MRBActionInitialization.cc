#include "MRBActionInitialization.hh"
#include "MRBPrimaryGeneratorAction.hh"
#include "MRBEventAction.hh"
#include "MRBRunAction.hh"

MRBActionInitialization::MRBActionInitialization()
 : G4VUserActionInitialization()
{}

MRBActionInitialization::~MRBActionInitialization()
{}

void MRBActionInitialization::BuildForMaster() const
{
  // Required to properly initialize the Analysis Manager in multi-threaded mode
  SetUserAction(new MRBRunAction());
}

void MRBActionInitialization::Build() const
{
  // 1. Register the 5 GeV Proton Beam
  SetUserAction(new MRBPrimaryGeneratorAction());
  
  // 2. Register the Run Action (Creates and saves the CSV file)
  SetUserAction(new MRBRunAction());
  
  // 3. Register the Event Action (Populates the CSV file with multiplicity/energy data)
  SetUserAction(new MRBEventAction());
}
