#include "MRBRunAction.hh"
#include "G4AnalysisManager.hh"

MRBRunAction::MRBRunAction() : G4UserRunAction() {
    // Initialize the Geant4 Analysis Manager to handle data acquisition
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("csv");
    
    // Create the Ntuple (data table) to store our experimental outcomes
    analysisManager->CreateNtuple("SpallationData", "MRB Shielding");
    
    // Define the columns corresponding to our hybrid detector matrix
    analysisManager->CreateNtupleIColumn("EventID");       // Column 0: Unique ID for the event
    analysisManager->CreateNtupleIColumn("DWC0_Hits");     // Column 1: Charged particle multiplicity at DWC0
    analysisManager->CreateNtupleIColumn("DWC1_Hits");     // Column 2: Charged particle multiplicity at DWC1
    analysisManager->CreateNtupleIColumn("DWC2_Hits");     // Column 3: Charged particle multiplicity at DWC2
    analysisManager->CreateNtupleDColumn("CaloEnergy_MeV");// Column 4: Total energy deposited in Calorimeter
    analysisManager->CreateNtupleIColumn("WENDI_Neutrons");// Column 5: Secondary neutron counts off-axis
    analysisManager->FinishNtuple();
}

MRBRunAction::~MRBRunAction() {}

void MRBRunAction::BeginOfRunAction(const G4Run*) { 
    // Open the output file at the start of the run. 
    // The filename is typically determined by the UI manager/macro.
    G4AnalysisManager::Instance()->OpenFile(); 
}

void MRBRunAction::EndOfRunAction(const G4Run*) {
    // Write accumulated data to the CSV and close the file safely
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
