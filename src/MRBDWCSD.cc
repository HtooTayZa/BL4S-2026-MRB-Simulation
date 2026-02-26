#include "MRBDWCSD.hh"
#include "MRBEventAction.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

MRBDWCSD::MRBDWCSD(G4String name) : G4VSensitiveDetector(name) 
{}

MRBDWCSD::~MRBDWCSD() {}

G4bool MRBDWCSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Check if the particle deposited energy in the gas (ionisation)
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false; 

    // Get the copy number of the DWC volume (0 = DWC0, 1 = DWC1, 2 = DWC2)
    G4int copyNo = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();

    // Retrieve the current EventAction to update the counters
    MRBEventAction* eventAction = (MRBEventAction*)G4RunManager::GetRunManager()->GetUserEventAction();

    // Route the hit to the correct counter
    if (eventAction) {
        if (copyNo == 0) {
            eventAction->AddHitDWC0();
        } else if (copyNo == 1) {
            eventAction->AddHitDWC1();
        } else if (copyNo == 2) {
            eventAction->AddHitDWC2();
        }
    }

    return true;
}
