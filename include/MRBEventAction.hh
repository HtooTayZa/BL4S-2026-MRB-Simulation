#ifndef MRBEventAction_h
#define MRBEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

class MRBEventAction : public G4UserEventAction
{
  public:
    MRBEventAction();
    virtual ~MRBEventAction(); 

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddHitDWC0() { fHitCountDWC0++; }
    void AddHitDWC1() { fHitCountDWC1++; }
    void AddHitDWC2() { fHitCountDWC2++; }
    void AddCaloEnergy(G4double edep) { fTotalCaloEnergy += edep; }
    void AddWendiNeutron() { fHitCountWendi++; }

  private:
    G4int fHitCountDWC0;
    G4int fHitCountDWC1;
    G4int fHitCountDWC2;
    G4double fTotalCaloEnergy;
    G4int fHitCountWendi; 
};

#endif
