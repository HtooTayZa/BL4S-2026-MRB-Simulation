#include "MRBRunAction.hh"
#include "G4AnalysisManager.hh"

MRBRunAction::MRBRunAction() : G4UserRunAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("csv");
    analysisManager->CreateNtuple("SpallationData", "MRB Shielding");
    analysisManager->CreateNtupleIColumn("EventID");       // 0
    analysisManager->CreateNtupleIColumn("DWC0_Hits");     // 1
    analysisManager->CreateNtupleIColumn("DWC1_Hits");     // 2
    analysisManager->CreateNtupleIColumn("DWC2_Hits");     // 3
    analysisManager->CreateNtupleDColumn("CaloEnergy_MeV");// 4
    analysisManager->CreateNtupleIColumn("WENDI_Neutrons");// 5
    analysisManager->FinishNtuple();
}

MRBRunAction::~MRBRunAction() {}
void MRBRunAction::BeginOfRunAction(const G4Run*) { G4AnalysisManager::Instance()->OpenFile(); }
void MRBRunAction::EndOfRunAction(const G4Run*) {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
