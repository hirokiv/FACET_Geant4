#include "Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "DetectorHit.hh"
#include "TargetHit.hh"
#include "VirtualHit.hh"
#include "DetectorConstruction.hh"
//#include "AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include <cmath>
#include <boost/format.hpp>

Run::Run()
  : G4Run(), fHCID1(-1), fHCID2(-1), fHCID3(-1), fNumberOfEvents(0)
{
  rootAnalysisManager1 = G4RootAnalysisManager::Instance();
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
  //  Fill fourth H1
  rootAnalysisManager1->FillH1(4, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());
  //G4cout << "Check!" << G4endl;
  fNumberOfEvents++;
  G4SDManager* sdm = G4SDManager::GetSDMpointer();


  // ===============================================================================
  //Sensitive detector fHCID1
  // ===============================================================================
  if (fHCID1<0) {
    fHCID1 = sdm->GetCollectionID("Target/TargetHitsCollection");
  }

  G4HCofThisEvent* mHCE1 = event->GetHCofThisEvent();
  TargetHitsCollection* target = NULL;
  if (mHCE1) {
    target = (TargetHitsCollection*)(mHCE1->GetHC(fHCID1));
    //G4cout << detectorHC2 << G4endl;
  }
  if (target) {
    G4int n_hit = target->entries();
    for (G4int i = 0; i < n_hit; i++) {
      TargetHit* aHit = (*target)[i];
    // Fill first H1
      rootAnalysisManager1->FillH1(1, aHit->GetEdep());
    // Is Photon?
//    G4cout << aHit->GetParticleName() << G4endl;
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

  // ===============================================================================
  // fHCID2
  // ===============================================================================
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
        G4ThreeVector prePos = aHit->GetPostPos();
        csvAnalysisManager->FillNtupleDColumn(1, 0, prePos.x()/cm);
        csvAnalysisManager->FillNtupleDColumn(1, 1, prePos.y()/cm);
        csvAnalysisManager->FillNtupleDColumn(1, 2, prePos.z()/cm);
        csvAnalysisManager->FillNtupleDColumn(1, 3, aHit->GetParticleTime()/ps);
        csvAnalysisManager->FillNtupleDColumn(1, 4, aHit->GetKineticEnergy()/MeV);
        G4ThreeVector direction = aHit->GetMomentumDirection();
        csvAnalysisManager->FillNtupleDColumn(1, 5, direction.x());
        csvAnalysisManager->FillNtupleDColumn(1, 6, direction.y());
        csvAnalysisManager->FillNtupleDColumn(1, 7, direction.z());
  //  Fill third H1
        rootAnalysisManager1->FillH1(3,
                                    std::acos(direction.z()/std::sqrt(direction.x()*direction.x() +
                                                                      direction.y()*direction.y() +
                                                                      direction.z()*direction.z())));
        if (aHit->GetCreatorProcessName()) {
          csvAnalysisManager->FillNtupleSColumn(1, 8, aHit->GetCreatorProcessName());
        }
        csvAnalysisManager->AddNtupleRow(1);
  //  Fill second H1
        rootAnalysisManager1->FillH1(2, aHit->GetKineticEnergy());
      }
      // Is electron? for the QuickPIC output
      if (aHit->GetParticleName() == "e-") {
        G4ThreeVector postPos = aHit->GetPostPos();
        G4ThreeVector mom_dir = aHit->GetMomentumDirection();
        G4double time = aHit->GetParticleTime();
        G4cout <<  "### Electron Current time : " << time << G4endl;
        G4cout <<  "### Electron Position : " << postPos.z()/m << G4endl;
        G4double c_light = 3e-1;   // speed of light in [m/ns]
        G4double z_init = - 50e-6; // Always start simulation at z=-50 um
        G4double gamma = aHit->GetKineticEnergy() / 0.511;
        G4double l = std::sqrt(mom_dir.x()*mom_dir.x()+mom_dir.y()*mom_dir.y()+mom_dir.z()*mom_dir.z());
        csvAnalysisManager->FillNtupleDColumn(2, 0, (postPos.x()/m)                      );
        csvAnalysisManager->FillNtupleDColumn(2, 1, (postPos.y()/m)                      );
        csvAnalysisManager->FillNtupleDColumn(2, 2, (mom_dir.x()/l)                     );
        csvAnalysisManager->FillNtupleDColumn(2, 3, (mom_dir.y()/l)                     );
        csvAnalysisManager->FillNtupleDColumn(2, 4, (c_light*time-postPos.z()/m+z_init));
        csvAnalysisManager->FillNtupleDColumn(2, 5, (gamma)                             );
        csvAnalysisManager->AddNtupleRow(2);
      }

      // Is positron? for the QuickPIC output
      if (aHit->GetParticleName() == "e+") {
        G4ThreeVector postPos = aHit->GetPostPos();
        G4ThreeVector mom_dir = aHit->GetMomentumDirection();
        G4double time = aHit->GetParticleTime();   // nano second [ns]
        G4double c_light =   3.e-1;   // speed of light in [m/ns]
        G4double z_init = - 50.e-6; // Always start simulation at z=-50 um
	G4cout <<  "### Positron Current time : " << time << G4endl;
	G4cout <<  "### Positron Position : " << postPos.z()/m << G4endl;
        G4double gamma = aHit->GetKineticEnergy() / 0.511;
        G4double l = std::sqrt(mom_dir.x()*mom_dir.x()+mom_dir.y()*mom_dir.y()+mom_dir.z()*mom_dir.z());
        csvAnalysisManager->FillNtupleDColumn(3, 0, postPos.x()/m);
        csvAnalysisManager->FillNtupleDColumn(3, 1, postPos.y()/m);
        csvAnalysisManager->FillNtupleDColumn(3, 2, mom_dir.x()/l);
        csvAnalysisManager->FillNtupleDColumn(3, 3, mom_dir.y()/l);
        csvAnalysisManager->FillNtupleDColumn(3, 4, c_light*time-postPos.z()/m+z_init);
        csvAnalysisManager->FillNtupleDColumn(3, 5, gamma);
        csvAnalysisManager->AddNtupleRow(3);
      }
    }
  }//End fHCID2

  // ===============================================================================
  // fHCID3
  // ===============================================================================
  if (fHCID3<0) {
    fHCID3 = sdm->GetCollectionID("Virtual/VirtualHitsCollection");
  }

  G4HCofThisEvent* mHCE3 = event->GetHCofThisEvent();
  VirtualHitsCollection* Virtual = NULL;
  if (mHCE3) {
    Virtual = (VirtualHitsCollection*)(mHCE3->GetHC(fHCID3));
  }

  if (Virtual) { 
    G4int n_hit = Virtual->entries();
// Only the first entry is taken
    if (n_hit>0) {
      VirtualHit* aHit = (*Virtual)[0];
      // Is gamma? 
      if (aHit->GetParticleName() == "gamma") {
      // Generate 2D histogram
        G4ThreeVector prePos = aHit->GetPrePos();
        G4ThreeVector mom_dir = aHit->GetMomentumDirection();
        rootAnalysisManager1->FillH2(1, prePos.x(), prePos.y());
        rootAnalysisManager1->FillH2(2, std::asin(mom_dir.x()), std::asin(mom_dir.y()));
      }
      // Is electron?
      if (aHit->GetParticleName() == "e-"   ) {
      // Generate 2D histogram
        G4ThreeVector prePos = aHit->GetPrePos();
        G4ThreeVector mom_dir = aHit->GetMomentumDirection();
        rootAnalysisManager1->FillH2(3, prePos.x(), prePos.y());
        rootAnalysisManager1->FillH2(4, std::asin(mom_dir.x()), std::asin(mom_dir.y()));
      }
      // Is positron?
      if (aHit->GetParticleName() == "e+"   ) {
      // Generate 2D histogram
        G4ThreeVector prePos = aHit->GetPrePos();
        G4ThreeVector mom_dir = aHit->GetMomentumDirection();
        rootAnalysisManager1->FillH2(5, prePos.x(), prePos.y());
        rootAnalysisManager1->FillH2(6, std::asin(mom_dir.x()), std::asin(mom_dir.y()));
      }
    }
  }

  G4Run::RecordEvent(event);
}
