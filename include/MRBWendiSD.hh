#ifndef MRBWendiSD_h
#define MRBWendiSD_h 1

#include "G4VSensitiveDetector.hh"
#include "MRBWendiHit.hh" // Include the new hit class

class G4Step;
class G4HCofThisEvent;

class MRBWendiSD : public G4VSensitiveDetector
{
  public:
    MRBWendiSD(const G4String& name); // Removed eventAction pointer
    virtual ~MRBWendiSD();

    virtual void Initialize(G4HCofThisEvent* hitCollection) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection) override;

  private:
    MRBWendiHitsCollection* fHitsCollection;
};
#endif
