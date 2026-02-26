#ifndef MRBPrimaryGeneratorAction_h
#define MRBPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4GenericMessenger.hh"

class G4Event;

class MRBPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MRBPrimaryGeneratorAction();    
    virtual ~MRBPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);         
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
  
  private:
    G4ParticleGun* fParticleGun;
    G4GenericMessenger* fMessenger; 
    G4double fBeamEnergy;           
    
    void DefineCommands();          
};

#endif
