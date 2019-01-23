#include "Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "DetectorHit.hh"
#include "TargetHit.hh"
#include "DetectorConstruction.hh"
//#include "AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include <cmath>

Run::Run()
  : G4Run(), fHCID1(-1), fHCID2(-1), fNumberOfEvents(0)
{
  rootAnalysisManager = G4RootAnalysisManager::Instance();
  csvAnalysisManager = G4CsvAnalysisManager::Instance();
}

Run::~Run()
{}

void Run::Merge(const G4Run* aRun)
{
  const Run* localRun = static_cast<const Run*>(aRun);
  fNumberOfEvents += localRun->fNumberOfEvents;

  G4Run::Merge(aRun);
}

void Run::RecordEvent(const G4Event* event)
{
  rootAnalysisManager->FillH1(4, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
  //G4cout << "Check!" << G4endl;
  fNumberOfEvents++;
  G4SDManager* sdm = G4SDManager::GetSDMpointer();
  //Sensitive detector
  if (fHCID1<0) {
    fHCID1 = sdm->GetCollectionID("Target/TargetHitsCollection");
  }
  G4HCofThisEvent* mHCE = event->GetHCofThisEvent();
  TargetHitsCollection* target = NULL;
    if (mHCE) {
    target = (TargetHitsCollection*)(mHCE->GetHC(fHCID1));
    //G4cout << detectorHC2 << G4endl;
  }
  if (target) {
    G4int n_hit = target->entries();
    //G4cout << n_hit << G4endl;
    for (G4int i = 0; i < n_hit; i++) {
      TargetHit* aHit = (*target)[i];
      //G4cout << G4BestUnit(edep, "Energy") << G4endl;
      rootAnalysisManager->FillH1(1, aHit->GetEdep());
      //Is Photon?
      if (aHit->GetParticleName() == "gamma") {
        if (aHit->GetCurrentStepNumber() == 1) {
          //Has kinetic energy?
          if (aHit->GetKineticEnergy()) {
            G4ThreeVector pos = aHit->GetPrePos();
            //G4ThreeVector postPos = aHit->GetPostPos();
            //G4ThreeVector pos = prePos + G4UniformRand()*(postPos-prePos);
            csvAnalysisManager->FillNtupleDColumn(0, 0, pos.x()/um);
            csvAnalysisManager->FillNtupleDColumn(0, 1, pos.y()/um);
            csvAnalysisManager->FillNtupleDColumn(0, 2, pos.z()/um);
            csvAnalysisManager->FillNtupleDColumn(0, 3, aHit->GetParticleTime()/ps);
            csvAnalysisManager->FillNtupleDColumn(0, 4, aHit->GetKineticEnergy()/MeV);
            G4ThreeVector direction = aHit->GetMomentumDirection();
            csvAnalysisManager->FillNtupleDColumn(0, 5, direction.x());
            csvAnalysisManager->FillNtupleDColumn(0, 6, direction.y());
            csvAnalysisManager->FillNtupleDColumn(0, 7, direction.z());
            if (aHit->GetCreatorProcessName()) {
              csvAnalysisManager->FillNtupleSColumn(0, 8, aHit->GetCreatorProcessName());
            }
           csvAnalysisManager->AddNtupleRow(0);
        }
        }
      }

    }
  }//End fHCID1

  //fHCID2
  if (fHCID2<0) {
    fHCID2 = sdm->GetCollectionID("Detector/DetectorHitsCollection");
  }

  G4HCofThisEvent* mHCE2 = event->GetHCofThisEvent();
  DetectorHitsCollection* detector = NULL;
  if (mHCE2) {
    detector = (DetectorHitsCollection*)(mHCE2->GetHC(fHCID2));
  }
  if (detector) {
    G4int n_hit = detector->entries();
    //Only the first entry is taken
    if (n_hit>0) {
      DetectorHit* aHit = (*detector)[0];
      //Is photon?
      if (aHit->GetParticleName() == "gamma") {
        G4ThreeVector prePos = aHit->GetPrePos();
        csvAnalysisManager->FillNtupleDColumn(1, 0, prePos.x()/cm);
        csvAnalysisManager->FillNtupleDColumn(1, 1, prePos.y()/cm);
        csvAnalysisManager->FillNtupleDColumn(1, 2, prePos.z()/cm);
        csvAnalysisManager->FillNtupleDColumn(1, 3, aHit->GetParticleTime()/ps);
        csvAnalysisManager->FillNtupleDColumn(1, 4, aHit->GetKineticEnergy()/MeV);
        G4ThreeVector direction = aHit->GetMomentumDirection();
        csvAnalysisManager->FillNtupleDColumn(1, 5, direction.x());
        csvAnalysisManager->FillNtupleDColumn(1, 6, direction.y());
        csvAnalysisManager->FillNtupleDColumn(1, 7, direction.z());
        rootAnalysisManager->FillH1(3,
                                    std::acos(direction.z()/std::sqrt(direction.x()*direction.x() +
                                                                      direction.y()*direction.y() +
                                                                      direction.z()*direction.z())));
        if (aHit->GetCreatorProcessName()) {
          csvAnalysisManager->FillNtupleSColumn(1, 8, aHit->GetCreatorProcessName());
        }
        csvAnalysisManager->AddNtupleRow(1);
        rootAnalysisManager->FillH1(2, aHit->GetKineticEnergy());
      }
    }
  }//End fHCID2

  G4Run::RecordEvent(event);
}
