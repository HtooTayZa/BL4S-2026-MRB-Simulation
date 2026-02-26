#ifndef MRBActionInitialization_h
#define MRBActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class MRBActionInitialization : public G4VUserActionInitialization
{
  public:
    MRBActionInitialization();
    virtual ~MRBActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
