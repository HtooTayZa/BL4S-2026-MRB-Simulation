#ifndef MRBScintillatorSD_h
#define MRBScintillatorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "MRBEventAction.hh"

class G4Step;
class G4HCofThisEvent;

class MRBScintillatorSD : public G4VSensitiveDetector
{
  public:
    // Constructor takes the SD name and a pointer to the EventAction
    MRBScintillatorSD(G4String name, MRBEventAction* eventAction);
    virtual ~MRBScintillatorSD();

    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    MRBEventAction* fEventAction;
};

#endif
