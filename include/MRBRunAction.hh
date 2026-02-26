#ifndef MRBRunAction_h
#define MRBRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class MRBRunAction : public G4UserRunAction
{
  public:
    MRBRunAction();
    virtual ~MRBRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

#endif
