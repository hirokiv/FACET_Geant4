#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction()
{
  G4cout << "Primary Generator Construction" << G4endl;
  fParticleGun = new G4GeneralParticleSource();
  G4cout << "Primary generator is constructed" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  fParticleGun->GeneratePrimaryVertex(event);
}
