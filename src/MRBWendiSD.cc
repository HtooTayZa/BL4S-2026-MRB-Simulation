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
    if (step->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition()) {
        if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
            fEventAction->AddWendiNeutron();
        }
    }
    return true;
}
void MRBWendiSD::EndOfEvent(G4HCofThisEvent*) {}
