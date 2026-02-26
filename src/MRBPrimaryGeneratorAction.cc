#include "MRBPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MRBPrimaryGeneratorAction::MRBPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fParticleGun(0), fMessenger(0), fBeamEnergy(5.0*GeV)
{
  fParticleGun  = new G4ParticleGun(1);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  fParticleGun->SetParticleDefinition(particleTable->FindParticle("proton"));
  DefineCommands(); 
}

MRBPrimaryGeneratorAction::~MRBPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

void MRBPrimaryGeneratorAction::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this, "/MRB/gun/", "Primary generator control");
  auto& energyCmd = fMessenger->DeclarePropertyWithUnit("energy", "GeV", fBeamEnergy, "Set mean beam energy.");
  energyCmd.SetParameterName("Energy", true);
  energyCmd.SetDefaultValue("5.0");
}

void MRBPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // 2cm diameter beam spot (sigma = 0.5cm)
  G4double beam_sigma_pos = 0.5 * cm; 
  G4double x0 = G4RandGauss::shoot(0.0, beam_sigma_pos);
  G4double y0 = G4RandGauss::shoot(0.0, beam_sigma_pos);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, -100.*cm));

  // 2% Energy Spread
  G4double energy_spread = fBeamEnergy * 0.02; 
  fParticleGun->SetParticleEnergy(G4RandGauss::shoot(fBeamEnergy, energy_spread));

  // 2 milliradian divergence
  G4double divergence = 0.002 * rad;
  G4double angle_x = G4RandGauss::shoot(0.0, divergence);
  G4double angle_y = G4RandGauss::shoot(0.0, divergence);
  G4ThreeVector direction(std::tan(angle_x), std::tan(angle_y), 1.0);
  fParticleGun->SetParticleMomentumDirection(direction.unit());

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
