#include "MRBRunAction.hh"
#include "G4AnalysisManager.hh"

MRBRunAction::MRBRunAction() : G4UserRunAction() {
    // Initialize the Geant4 Analysis Manager
    auto analysisManager = G4AnalysisManager::Instance();
    
    // UPDATED: Set the default file type to ROOT as specified in the report
    analysisManager->SetDefaultFileType("root");
    
    // Create the Ntuple (data table) to store our experimental outcomes
    analysisManager->CreateNtuple("SpallationData", "MRB Shielding Event Data");
    
    // Define the columns corresponding to our hybrid detector matrix
    analysisManager->CreateNtupleIColumn("EventID");       // Column 0
    analysisManager->CreateNtupleIColumn("DWC0_Hits");     // Column 1
    analysisManager->CreateNtupleIColumn("DWC1_Hits");     // Column 2
    analysisManager->CreateNtupleIColumn("DWC2_Hits");     // Column 3
    analysisManager->CreateNtupleDColumn("CaloEnergy_MeV");// Column 4
    analysisManager->CreateNtupleIColumn("WENDI_Neutrons");// Column 5
    analysisManager->CreateNtupleIColumn("T_charged");     // Column 6
    analysisManager->CreateNtupleIColumn("T_neutral");     // Column 7
    
    analysisManager->FinishNtuple();
}

MRBRunAction::~MRBRunAction() {}

void MRBRunAction::BeginOfRunAction(const G4Run*) { 
    // Open the output file at the start of the run.
    // Specifying a filename here automatically appends .root
    G4AnalysisManager::Instance()->OpenFile("MRB_Simulation_Data"); 
}

void MRBRunAction::EndOfRunAction(const G4Run*) {
    // Write accumulated data to the ROOT file and close it safely
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
