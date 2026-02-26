#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh" // High Precision Hadronic Physics List
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "MRBDetectorConstruction.hh"
#include "MRBActionInitialization.hh"

int main(int argc, char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Construct the default run manager
  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  
  // 1. Detector Construction: Your 10 mm Al Plate and 12.5 cm MRB Block
  runManager->SetUserInitialization(new MRBDetectorConstruction());

  // 2. Physics List: High precision tracking for hadronic cascades and secondary neutrons
  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  runManager->SetUserInitialization(physicsList);

  // 3. Action Initialization: Your 5 GeV Proton Beam
  runManager->SetUserInitialization(new MRBActionInitialization());

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if ( ! ui ) { 
    // Batch mode (running via script)
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // Interactive mode (GUI)
    // UImanager->ApplyCommand("/control/execute init_vis.mac"); 
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  delete visManager;
  delete runManager;
  
  return 0;
}
