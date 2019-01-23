#include "SensitiveTarget.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "G4VProcess.hh"
#include "G4UnitsTable.hh"
#include "G4TouchableHandle.hh"

SensitiveTarget::SensitiveTarget(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

SensitiveTarget::~SensitiveTarget()
{}

void SensitiveTarget::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection
  fHitsCollection = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]);

  //Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool SensitiveTarget::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  TargetHit* newHit = new TargetHit();

  newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newHit->SetParentID(aStep->GetTrack()->GetParentID());
  newHit->SetEdep(aStep->GetTotalEnergyDeposit());
  newHit->SetPrePos(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetPostPos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetParticleTime(aStep->GetPreStepPoint()->GetGlobalTime());
  newHit->SetParticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  if (aStep->GetTrack()->GetCreatorProcess()) {
    newHit->SetCreatorProcessName(aStep->GetTrack()->GetCreatorProcess()->GetProcessName());
  }
  newHit->SetKineticEnergy(aStep->GetTrack()->GetKineticEnergy());
  newHit->SetMomentumDirection(aStep->GetPreStepPoint()->GetMomentumDirection());
  newHit->SetCharge(aStep->GetPreStepPoint()->GetCharge());
  newHit->SetCurrentStepNumber(aStep->GetTrack()->GetCurrentStepNumber());

  fHitsCollection->insert(newHit);
  //newHit->Print();

  return true;
}

void SensitiveTarget::EndOfEvent(G4HCofThisEvent*)
{
}
