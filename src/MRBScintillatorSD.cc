#include "MRBScintillatorSD.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

MRBScintillatorSD::MRBScintillatorSD(G4String name, MRBEventAction* eventAction)
: G4VSensitiveDetector(name), fEventAction(eventAction)
{}

MRBScintillatorSD::~MRBScintillatorSD() {}

void MRBScintillatorSD::Initialize(G4HCofThisEvent*) {}

G4bool MRBScintillatorSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Get the energy deposited in this step
    G4double edep = aStep->GetTotalEnergyDeposit();
    
    // If no energy is deposited, we don't count it as a valid optical pulse
    if (edep == 0.) return false;

    // Retrieve the global time of the hit (nanosecond-precision timestamp)
    G4double hitTime = aStep->GetPostStepPoint()->GetGlobalTime();
    
    // Identify which scintillator was hit based on the detector's assigned name
    // and pass the timestamp to the Event Action for coincidence logic
    if (SensitiveDetectorName == "Sin_SD") {
        fEventAction->RegisterSinHit(hitTime);
    } 
    else if (SensitiveDetectorName == "Veto_SD") {
        fEventAction->RegisterVetoHit(hitTime);
    }

    return true;
}

void MRBScintillatorSD::EndOfEvent(G4HCofThisEvent*) {}
