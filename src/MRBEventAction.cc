#include "MRBEventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4ios.hh"

MRBEventAction::MRBEventAction()
: G4UserEventAction(), 
  fHitCountDWC0(0), fHitCountDWC1(0), fHitCountDWC2(0), 
  fTotalCaloEnergy(0.), fHitCountWendi(0),
  fSinTime(-1.0), fVetoTime(-1.0), fCalHit(false)
{}

MRBEventAction::~MRBEventAction() {}

void MRBEventAction::BeginOfEventAction(const G4Event*)
{
    // Reset all original detector tallies
    fHitCountDWC0 = 0;
    fHitCountDWC1 = 0;
    fHitCountDWC2 = 0;
    fTotalCaloEnergy = 0.;
    fHitCountWendi = 0;

    // Reset the new hardware trigger timestamps
    fSinTime  = -1.0;
    fVetoTime = -1.0;
    fCalHit   = false;
}

// --- New Scintillator Timing Methods ---
void MRBEventAction::RegisterSinHit(G4double time)
{
    if (fSinTime < 0.0) { fSinTime = time; }
}

void MRBEventAction::RegisterVetoHit(G4double time)
{
    if (fVetoTime < 0.0) { fVetoTime = time; }
}

void MRBEventAction::EndOfEventAction(const G4Event* event)
{
    G4int eventID = event->GetEventID();
    
    // Print progress to the console every 10,000 events (Restored)
    if (eventID % 10000 == 0 && eventID != 0) {
        G4cout << ">>> Progress: " << eventID << " events simulated..." << G4endl;
    }

    // --- Evaluate Hardware NIM Trigger Logic ---
    G4int T_charged = 0; // Using integer 0 or 1 for easy ROOT Ntuple storage
    G4int T_neutral = 0;

    // A valid event requires a primary proton (S_in) and an energy dump (Cal)
    if (fSinTime >= 0.0 && fCalHit) {
        // Evaluate the 50 ns Time-of-Flight window
        if (fVetoTime >= 0.0 && (fVetoTime - fSinTime) <= 50.0 * ns) {
            T_charged = 1; 
        } else {
            T_neutral = 1;
        }
    }

// --- Original Data Recording Logic (Restored) ---
    // Only record the event if there was some interaction detected downstream.
    if (fHitCountDWC0 > 0 || fHitCountDWC1 > 0 || fHitCountDWC2 > 0 || fTotalCaloEnergy > 0. || fHitCountWendi > 0) {
        auto analysisManager = G4AnalysisManager::Instance();
        
        // Populate the exact original columns
        analysisManager->FillNtupleIColumn(0, eventID);
        analysisManager->FillNtupleIColumn(1, fHitCountDWC0);
        analysisManager->FillNtupleIColumn(2, fHitCountDWC1);
        analysisManager->FillNtupleIColumn(3, fHitCountDWC2);
        analysisManager->FillNtupleDColumn(4, fTotalCaloEnergy / MeV);
        analysisManager->FillNtupleIColumn(5, fHitCountWendi); 
        analysisManager->FillNtupleIColumn(6, T_charged);
        analysisManager->FillNtupleIColumn(7, T_neutral);
        
        // Commit the row to the file
        analysisManager->AddNtupleRow();
    }
}
