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

    // --- Existing Functionality: Data Collection ---
    void AddHitDWC0() { fHitCountDWC0++; }
    void AddHitDWC1() { fHitCountDWC1++; }
    void AddHitDWC2() { fHitCountDWC2++; }
    void AddCaloEnergy(G4double edep) { fTotalCaloEnergy += edep; }
   
    // --- New Functionality: Hardware Trigger Logic ---
    void RegisterSinHit(G4double time);
    void RegisterVetoHit(G4double time);
    void SetCalHit(G4bool hit) { fCalHit = hit; }

  private:

    // --- Existing Variables ---
    G4int fHitCountDWC0;
    G4int fHitCountDWC1;
    G4int fHitCountDWC2;
    G4double fTotalCaloEnergy;
    G4int fHitCountWendi; 

    // --- New Variables ---
    G4double fSinTime;
    G4double fVetoTime;
    G4bool   fCalHit;
    G4int fWendiHCID;
};

#endif
