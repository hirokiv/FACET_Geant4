#include "MSpecHit.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>

G4ThreadLocal G4Allocator<MSpecHit>* MSpecHitAllocator = 0;

MSpecHit::MSpecHit()
  : G4VHit(), fTrackID(0), fParentID(0), fEdep(0.), fPrePos(G4ThreeVector()), fPostPos(G4ThreeVector()),
    fParticle(""), fParticleTime(0.), fCreatorProcessName(""),  fKineticEnergy(0.0),
    fMomentumDirection(G4ThreeVector()), fCharge(0.0), fCurrentStepNumber(-1)
{}

MSpecHit::~MSpecHit()
{}

MSpecHit::MSpecHit(const MSpecHit &right)  : G4VHit()
{
  fTrackID  = right.fTrackID;
  fParentID = right.fParentID;
  fEdep     = right.fEdep;
  fPrePos = right.fPrePos;
  fPostPos = right.fPostPos;
  fParticle = right.fParticle;
  fParticleTime = right.fParticleTime;
  fCreatorProcessName = right.fCreatorProcessName;
  fKineticEnergy   = right.fKineticEnergy;
  fMomentumDirection = right.fMomentumDirection;
  fCharge     = right.fCharge;
  fCurrentStepNumber= right.fCurrentStepNumber;
}

const MSpecHit& MSpecHit::operator=(const MSpecHit &right)
{
  fTrackID  = right.fTrackID;
  fParentID = right.fParentID;
  fEdep     = right.fEdep;
  fPrePos = right.fPrePos;
  fPostPos = right.fPostPos;
  fParticle = right.fParticle;
  fParticleTime = right.fParticleTime;
  fCreatorProcessName = right.fCreatorProcessName;
  fKineticEnergy   = right.fKineticEnergy;
  fMomentumDirection = right.fMomentumDirection;
  fCharge     = right.fCharge;
  fCurrentStepNumber= right.fCurrentStepNumber;

  return *this;
}

G4int MSpecHit::operator==(const MSpecHit &right) const
{
  return (this == &right) ? 1 : 0;
}

void MSpecHit::Print()
{
  new G4UnitDefinition ( "meter/s", "m/s", "Speed", m/s );
  new G4UnitDefinition ( "kg*meter/s", "kg*m/s", "Momentum",kg*m/s );
  G4cout << "\n New Hit ==> "
         << "                \nTrackID = " << fTrackID
         << "                \nParentID = " << fParentID
         << "                \nEdep = " << std::setw(7) << G4BestUnit(fEdep, "Energy")
         << "                \nParticle name = " << fParticle
         << "                \nParticle time = " << G4BestUnit(fParticleTime, "Time")
         << "                \nCreator process name = " << fCreatorProcessName
         << "                \nKinetic Energy = " << G4BestUnit(fKineticEnergy, "Energy")
         << "                \nCharge = " << fCharge
         << G4endl;
}
