#include "MRBCaloSD.hh"
#include "MRBEventAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

MRBCaloSD::MRBCaloSD(G4String name) : G4VSensitiveDetector(name) 
{}

MRBCaloSD::~MRBCaloSD() {}

G4bool MRBCaloSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Get the energy deposited in this step
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    // Send the deposited energy to the Event Action to be tallied
    MRBEventAction* eventAction = (MRBEventAction*)G4RunManager::GetRunManager()->GetUserEventAction();
    if (eventAction) {
        eventAction->AddCaloEnergy(edep);
    }

    return true;
}
