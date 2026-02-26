#ifndef MRBDWCSD_h
#define MRBDWCSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class G4Step;

class MRBDWCSD : public G4VSensitiveDetector
{
public:
    MRBDWCSD(G4String name);
    virtual ~MRBDWCSD();

    // This is called every time a particle "steps" inside the DWC gas
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
};

#endif
