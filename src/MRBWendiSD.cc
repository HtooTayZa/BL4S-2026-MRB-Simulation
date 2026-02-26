#include "MRBWendiSD.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"

MRBWendiSD::MRBWendiSD(const G4String& name, MRBEventAction* eventAction)
: G4VSensitiveDetector(name), fEventAction(eventAction) {}

MRBWendiSD::~MRBWendiSD() {}
void MRBWendiSD::Initialize(G4HCofThisEvent*) {}

G4bool MRBWendiSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    // Filter specifically for neutrons entering the WENDI detector
    if (step->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition()) {
        
        // We only want to count the neutron once when it enters the detector volume (crosses the boundary)
        // to prevent overcounting from multiple steps taken by the same neutron inside the gas.
        if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
            fEventAction->AddWendiNeutron();
        }
    }
    return true;
}
void MRBWendiSD::EndOfEvent(G4HCofThisEvent*) {}
