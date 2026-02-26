#ifndef MRBDetectorConstruction_h
#define MRBDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4GenericMessenger.hh"

class MRBDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MRBDetectorConstruction();
    virtual ~MRBDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetShieldConfig(G4String materialName);

  private:
    G4LogicalVolume* fLogicMRB;
    G4Box* fSolidShield; 
    G4GenericMessenger* fMessenger;

    void DefineCommands();
};

#endif
