#ifndef MRBCaloSD_h
#define MRBCaloSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class G4Step;

class MRBCaloSD : public G4VSensitiveDetector
{
public:
    MRBCaloSD(G4String name);
    virtual ~MRBCaloSD();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
};

#endif
