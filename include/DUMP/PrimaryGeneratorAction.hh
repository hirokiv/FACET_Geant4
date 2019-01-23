#ifndef PRIMARYGENERATORACTION_H
#define PRIMARYGENERATORACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class DetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);
private:
  G4GeneralParticleSource* fParticleGun;
};



#endif /* PRIMARYGENERATORACTION_H */
