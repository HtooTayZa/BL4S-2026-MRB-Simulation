#include "MRBPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

MRBPrimaryGeneratorAction::MRBPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fParticleGun(0), fMessenger(0), fBeamMomentum(5.0*GeV)
{
  // Initialize the particle gun to shoot 1 particle per event
  fParticleGun  = new G4ParticleGun(1);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
  // Set the primary particle to a proton (simulating Galactic Cosmic Rays)
  fParticleGun->SetParticleDefinition(particleTable->FindParticle("proton"));
  
  // Initialize the UI command /MRB/gun/momentum
  DefineCommands(); 
}

MRBPrimaryGeneratorAction::~MRBPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

void MRBPrimaryGeneratorAction::DefineCommands()
{
  // Allow dynamic adjustment of the primary beam momentum from macros
  fMessenger = new G4GenericMessenger(this, "/MRB/gun/", "Primary generator control");
  auto& momCmd = fMessenger->DeclarePropertyWithUnit("momentum", "GeV", fBeamMomentum, "Set mean beam momentum.");
  momCmd.SetParameterName("Momentum", true);
  momCmd.SetDefaultValue("5.0");
}

void MRBPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // --- Beam Realism Setup ---

  // 1. Spot Size: 2 mm diameter Gaussian distribution (sigma = 1.0 mm)
  G4double beam_sigma_pos = 1.0 * mm; 
  G4double x0 = G4RandGauss::shoot(0.0, beam_sigma_pos);
  G4double y0 = G4RandGauss::shoot(0.0, beam_sigma_pos);
  
  // Start the beam 150 cm upstream of the target origin to clear the S_in scintillator at Z = -1.00 m
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, -150.*cm));

  // 2. Momentum Setup
  // We use SetParticleMomentum to accurately reflect the 5.0 GeV/c beam.
  // Geant4 will automatically calculate the correct 4.15 GeV kinetic energy.
  // Energy spread is removed to simulate a monoenergetic beam per the technical report.
  fParticleGun->SetParticleMomentum(fBeamMomentum); 

  // 3. Beam Divergence: 2 milliradians applied via Gaussian scattering on X and Y angles
  G4double divergence = 0.002 * rad;
  G4double angle_x = G4RandGauss::shoot(0.0, divergence);
  G4double angle_y = G4RandGauss::shoot(0.0, divergence);
  
  // Calculate and apply the momentum direction vector based on the divergence angles
  G4ThreeVector direction(std::tan(angle_x), std::tan(angle_y), 1.0);
  fParticleGun->SetParticleMomentumDirection(direction.unit());

  // Generate the vertex for this specific event
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
