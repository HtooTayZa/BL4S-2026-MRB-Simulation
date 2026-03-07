#include "MRBWendiSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Neutron.hh"

G4ThreadLocal G4Allocator<MRBWendiHit>* MRBWendiHitAllocator = nullptr;
MRBWendiSD::MRBWendiSD(const G4String& name)
: G4VSensitiveDetector(name), fHitsCollection(nullptr) 
{
    // Register the hits collection name with Geant4
    collectionName.insert("WendiHitsCollection");
}

MRBWendiSD::~MRBWendiSD() {}

void MRBWendiSD::Initialize(G4HCofThisEvent* hce) 
{
    // Create the hits collection at the start of each event
    fHitsCollection = new MRBWendiHitsCollection(SensitiveDetectorName, collectionName[0]);
    
    // Add it to the event's overall hit collection map
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool MRBWendiSD::ProcessHits(G4Step* step, G4TouchableHistory*) 
{
    // Filter specifically for neutrons entering the WENDI detector
    if (step->GetTrack()->GetDefinition() == G4Neutron::NeutronDefinition()) {
        if (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
            
            // Create a new hit and add it to the collection
            MRBWendiHit* newHit = new MRBWendiHit();
            fHitsCollection->insert(newHit);
        }
    }
    return true;
}

void MRBWendiSD::EndOfEvent(G4HCofThisEvent*) {}
