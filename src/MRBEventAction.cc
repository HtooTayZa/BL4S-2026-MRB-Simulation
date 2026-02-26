#include "MRBEventAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

MRBEventAction::MRBEventAction()
: G4UserEventAction(), fHitCountDWC0(0), fHitCountDWC1(0), fHitCountDWC2(0), fTotalCaloEnergy(0.), fHitCountWendi(0)
{}

MRBEventAction::~MRBEventAction() {}

void MRBEventAction::BeginOfEventAction(const G4Event*)
{
    // Reset all detector tallies at the beginning of each new event (proton interaction)
    fHitCountDWC0 = 0;
    fHitCountDWC1 = 0;
    fHitCountDWC2 = 0;
    fTotalCaloEnergy = 0.;
    fHitCountWendi = 0;
}

void MRBEventAction::EndOfEventAction(const G4Event* event)
{
    G4int eventID = event->GetEventID();
    
    // Print progress to the console every 10,000 events
    if (eventID % 10000 == 0 && eventID != 0) {
        G4cout << ">>> Progress: " << eventID << " events simulated..." << G4endl;
    }

    // Only record the event if there was some interaction detected downstream.
    // This saves disk space by filtering out events where particles completely missed the detectors.
    if (fHitCountDWC0 > 0 || fHitCountDWC1 > 0 || fHitCountDWC2 > 0 || fTotalCaloEnergy > 0. || fHitCountWendi > 0) {
        auto analysisManager = G4AnalysisManager::Instance();
        
        // Populate the CSV columns with the collected data for this event
        analysisManager->FillNtupleIColumn(0, eventID);
        analysisManager->FillNtupleIColumn(1, fHitCountDWC0);
        analysisManager->FillNtupleIColumn(2, fHitCountDWC1);
        analysisManager->FillNtupleIColumn(3, fHitCountDWC2);
        analysisManager->FillNtupleDColumn(4, fTotalCaloEnergy / MeV); // Convert energy to MeV
        analysisManager->FillNtupleIColumn(5, fHitCountWendi); 
        
        // Commit the row to the file
        analysisManager->AddNtupleRow();
    }
}
