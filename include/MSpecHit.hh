#ifndef MSPECHIT_H
#define MSPECHIT_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class MSpecHit : public G4VHit
{
public:
  MSpecHit();
  MSpecHit(const MSpecHit&);
  virtual ~MSpecHit();

  //Operators
  const MSpecHit& operator=(const MSpecHit&);
  G4int operator==(const MSpecHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  //Methids from base class
  virtual void Print();

  //Set methods
  void SetTrackID(G4int id)           {fTrackID = id;}
  void SetParentID(G4int id)          {fParentID = id;}
  void SetEdep(G4double edep)         {fEdep = edep;}
  void SetPrePos(G4ThreeVector pos) {fPrePos = pos;}
  void SetPostPos(G4ThreeVector pos) {fPostPos = pos;}
  void SetParticleName(G4String name) {fParticle = name;}
  void SetParticleTime(G4double time) {fParticleTime = time;}
  void SetCreatorProcessName(G4String name) {fCreatorProcessName = name;}
  void SetKineticEnergy(G4double ene) {fKineticEnergy = ene;}
  void SetMomentumDirection(G4ThreeVector md) {fMomentumDirection = md;}
  void SetCharge(G4double ch)         {fCharge = ch;}
  void SetCurrentStepNumber(G4int val)     {fCurrentStepNumber = val;}

  //Get methods
  G4int         GetTrackID()            {return fTrackID;}
  G4int         GetParentID()           {return fParentID;}
  G4double      GetEdep() const         {return fEdep;}
  G4ThreeVector GetPrePos()           {return fPrePos;}
  G4ThreeVector GetPostPos()           {return fPostPos;}
  G4String      GetParticleName()       {return fParticle;}
  G4double      GetParticleTime()       {return fParticleTime;}
  G4String      GetCreatorProcessName() {return fCreatorProcessName;}
  G4double      GetKineticEnergy()      {return fKineticEnergy;}
  G4ThreeVector GetMomentumDirection()  {return fMomentumDirection;}
  G4double      GetCharge()             {return fCharge;}
  G4double      GetCurrentStepNumber()  {return fCurrentStepNumber;}


private:
  G4int              fTrackID;
  G4int              fParentID;
  G4double           fEdep;
  G4ThreeVector      fPrePos;
  G4ThreeVector      fPostPos;
  G4String           fParticle;
  G4double           fParticleTime;
  G4String           fCreatorProcessName;
  G4double           fKineticEnergy;
  G4ThreeVector      fMomentumDirection;
  G4double           fCharge;
  G4int              fCurrentStepNumber;
};

typedef G4THitsCollection<MSpecHit> MSpecHitsCollection;

extern G4ThreadLocal G4Allocator<MSpecHit>* MSpecHitAllocator;

inline void* MSpecHit::operator new(size_t)
{
  if (!MSpecHitAllocator) {
    MSpecHitAllocator = new G4Allocator<MSpecHit>;
  }
  return (void*) MSpecHitAllocator->MallocSingle();
}

inline void MSpecHit::operator delete(void *hit)
  {
    MSpecHitAllocator->FreeSingle((MSpecHit*) hit);
  }

#endif /* TARGETHIT_H */
