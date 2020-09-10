#include "Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "DetectorHit.hh"
#include "TargetHit.hh"
#include "VirtualHit.hh"
#include "MSpecHit.hh"
#include "DetectorConstruction.hh"
//#include "AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include <cmath>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>

Run::Run()
  : G4Run(), fHCID1(-1), fHCID2(-1), fHCID3(-1), fHCID4(-1), fNumberOfEvents(0), 
    emitt_N(20), E_N(10*GeV), E_range(0.05), 
    xx2(std::vector<G4double>(emitt_N)),  vx2(std::vector<G4double>(emitt_N)), 
     xvx(std::vector<G4double>(emitt_N)), E_count(std::vector<G4int>(emitt_N)), E_sum(std::vector<G4double>(emitt_N))
{
  rootAnalysisManager1 = G4RootAnalysisManager::Instance();
  csvAnalysisManager = G4CsvAnalysisManager::Instance();

//  emitt_N = 20;
//  E_N = 10*GeV; //GeV
//  E_range = 0.05; // \pm 5%

  
//  xx2(std::vector<G4double>(emitt_N));          // um
//  vx2(std::vector<G4double>(emitt_N));        // rad
//  xvx(std::vector<G4double>(emitt_N)); // mm-mrad
//  E_count[emitt_N] = {0}; // Number of electrons used for the calculation
 
}

Run::~Run()
{
//  WriteElectronsInfo();
  WriteParticlesInfo();
}

void Run::WriteParticlesInfo()
{
  std::ofstream outputfile("BeamParamsElectron.txt", std::ios::app);
  outputfile << 
"Energy [MeV], Mean energy [MeV], # of sampled particles, Sigma_x [um], Sigma_vx [urad], Epsilon_N [mm-mrad]"
 << G4endl;

  for (G4int emitt_idx = 0 ; emitt_idx < emitt_N; emitt_idx++){
    G4double E_c_temp = (G4double)E_count[emitt_idx]; // # of electrons in pm 5 percent range
    G4double E_temp = (G4double)(emitt_idx+1)/(G4double)emitt_N * E_N;  // in MeV
    G4double E_ave = E_sum[emitt_idx] / E_c_temp; 
    G4double emittance_n = std::sqrt(xx2[emitt_idx]*vx2[emitt_idx]/(E_c_temp*E_c_temp) - pow(xvx[emitt_idx]/E_c_temp,2.0)) * (E_ave/0.511+1);

    outputfile << E_temp << "     ";
    outputfile << E_ave / MeV << "     ";
    outputfile << E_c_temp << "     ";
    outputfile << std::sqrt(xx2[emitt_idx]/E_c_temp) << "     "; //um
    outputfile << std::sqrt(vx2[emitt_idx]/E_c_temp)*1000. << "     "; // mrad
    outputfile << emittance_n << G4endl; //mm-mrad
  }
  outputfile << G4endl;
  outputfile.close();

}
//void Run::WritePositronsInfo()
//{
//  std::ofstream outputfile("BeamParams.txt", std::ios::app);
//  outputfile << 
//"Energy [MeV], Mean energy [MeV], # of sampled particles, Sigma_x [um], Sigma_vx [urad], Epsilon_N [mm-mrad]"
// << G4endl;
//
//  for (G4int emitt_idx = 0 ; emitt_idx < emitt_N; emitt_idx++){
//    G4double E_c_temp = (G4double)E_count[emitt_idx]; // # of electrons in pm 5 percent range
//    G4double E_temp = (G4double)(emitt_idx+1)/(G4double)emitt_N * E_N;  // in MeV
//    G4double E_ave = E_sum[emitt_idx] / E_c_temp; 
//    G4double emittance_n = std::sqrt(xx2[emitt_idx]*vx2[emitt_idx]/(E_c_temp*E_c_temp) - pow(xvx[emitt_idx]/E_c_temp,2.0)) * (E_ave/0.511+1);
//
//    outputfile << E_temp << "     ";
//    outputfile << E_ave / MeV << "     ";
//    outputfile << E_c_temp << "     ";
//    outputfile << std::sqrt(xx2[emitt_idx]/E_c_temp) << "     "; //um
//    outputfile << std::sqrt(vx2[emitt_idx]/E_c_temp)*1000. << "     "; // mrad
//    outputfile << emittance_n << G4endl; //mm-mrad
//  }
//  outputfile << G4endl;
//  outputfile.close();
//
//}



void Run::Merge(const G4Run* aRun)
{
  const Run* localRun = static_cast<const Run*>(aRun);
  fNumberOfEvents += localRun->fNumberOfEvents;

  G4Run::Merge(aRun);
}

void Run::RecordEvent(const G4Event* event)
{
  //G4cout << "Check!" << G4endl;
  fNumberOfEvents++;
  G4SDManager* sdm = G4SDManager::GetSDMpointer();

  // ===============================================================================
  // Show initial source histogram and phase space
  //  Fill sixth H1
  // ===============================================================================
  rootAnalysisManager1->FillH1(6, event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy());

  rootAnalysisManager1->FillH2(13, event->GetPrimaryVertex()->GetX0(), std::asin( event->GetPrimaryVertex()->GetPrimary()->GetMomentumDirection().x() ));

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
    if (n_hit>0) {
      for (G4int i = 0; i < n_hit; i++) {
        TargetHit* aHit = (*target)[i];
      // Fill first H1
        rootAnalysisManager1->FillH1(1, aHit->GetEdep());
      // Is Photon?
        G4cout << aHit->GetCreatorProcessName() << G4endl;
        G4cout << aHit->GetParticleName() << G4endl;
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
      for (G4int i = 0; i < n_hit; i++) {
        DetectorHit* aHit = (*detector)[i];
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

    //  Fill third H1
          rootAnalysisManager1->FillH1(3,
                                      std::acos(direction.z()/std::sqrt(direction.x()*direction.x() +
                                                                        direction.y()*direction.y() +
                                                                        direction.z()*direction.z())));
          if (aHit->GetCreatorProcessName()) {
            csvAnalysisManager->FillNtupleSColumn(1, 8, aHit->GetCreatorProcessName());
          }
          csvAnalysisManager->AddNtupleRow(1);

//          G4ThreeVector prePos = aHit->GetPrePos();
//          G4ThreeVector mom_dir = aHit->GetMomentumDirection();
//          rootAnalysisManager1->FillH2(13, prePos.x(), prePos.y(), aHit->GetKineticEnergy()/MeV );
        }

// For QuickPIC output
        G4double E_gamma = aHit->GetKineticEnergy() / 0.511 + 1;
        if (E_gamma > 1.5) {
          // Is electron? for the QuickPIC output
          if (aHit->GetParticleName() == "e-") {
            G4ThreeVector prePos = aHit->GetPrePos();
            G4ThreeVector mom_dir = aHit->GetMomentumDirection();
            G4double time = aHit->GetParticleTime();
            G4double c_light = 3e-1;   // speed of light in [m/ns]
            G4double z_init = - 50e-6; // Always start simulation at z=-50 um
//            G4double E_gamma = aHit->GetKineticEnergy() / 0.511 + 1;
//            G4double l = std::sqrt(mom_dir.x()*mom_dir.x()+mom_dir.y()*mom_dir.y()+mom_dir.z()*mom_dir.z());
            csvAnalysisManager->FillNtupleDColumn(2, 0, (prePos.x()/m)            );
            csvAnalysisManager->FillNtupleDColumn(2, 1, (prePos.y()/m)             );
            csvAnalysisManager->FillNtupleDColumn(2, 2, (mom_dir.x())                     );
            csvAnalysisManager->FillNtupleDColumn(2, 3, (mom_dir.y())                     );
            csvAnalysisManager->FillNtupleDColumn(2, 4, (c_light*time-prePos.z()/m+z_init));
            csvAnalysisManager->FillNtupleDColumn(2, 5, (E_gamma)                         );
            csvAnalysisManager->AddNtupleRow(2);
            //  Fill fourth H1
  
            rootAnalysisManager1->FillH1(4, aHit->GetKineticEnergy());
// Filter out only primary electrons
            if (aHit->GetTrackID()==1) {
              csvAnalysisManager->FillNtupleDColumn(4, 0, (prePos.x()/m)       );
              csvAnalysisManager->FillNtupleDColumn(4, 1, (prePos.y()/m)          );
              csvAnalysisManager->FillNtupleDColumn(4, 2, (mom_dir.x())           );
              csvAnalysisManager->FillNtupleDColumn(4, 3, (mom_dir.y())              );
              csvAnalysisManager->FillNtupleDColumn(4, 4, (c_light*time-prePos.z()/m+z_init));
              csvAnalysisManager->FillNtupleDColumn(4, 5, (E_gamma)               );
              csvAnalysisManager->AddNtupleRow(4);
	      //csvAnalysisManager->FillNtupleSColumn(4, 8, aHit->GetCreatorProcessName());
	      //csvAnalysisManager->AddNtupleRow(4);
              rootAnalysisManager1->FillH1(9, aHit->GetKineticEnergy());
            }

//  Some new features such as emittance calculations
          // Calculate emittances
            for (G4int emitt_idx = 0; emitt_idx<emitt_N ; emitt_idx++ ){
              G4double E_temp = (G4double)(emitt_idx+1)/(G4double)emitt_N * E_N;  // in GeV
              if ( (E_temp * (1+E_range) > aHit->GetKineticEnergy() ) && ( E_temp * (1-E_range) <  aHit->GetKineticEnergy() ) ){
                xx2[emitt_idx] = xx2[emitt_idx] + pow(prePos.x()/um,2.0) ;          // um
                vx2[emitt_idx] = vx2[emitt_idx] + pow(std::asin(mom_dir.x())/rad,2.0) ;        // rad
                xvx[emitt_idx] = xvx[emitt_idx] + prePos.x()/um * std::asin(mom_dir.x())/rad; // mm-mrad
                E_count[emitt_idx] = E_count[emitt_idx] + 1;
                E_sum[emitt_idx] = E_sum[emitt_idx] + aHit->GetKineticEnergy()/MeV;
  //		G4cout<< emitt_idx << G4endl;
  //		G4cout<< E_count[emitt_idx] << G4endl;
  //		G4cout<< xx2[emitt_idx] << G4endl;  // um*um
  //		G4cout<< vx2[emitt_idx] << G4endl;  // rad*rad
  //		G4cout<< xvx[emitt_idx] << G4endl;  // um*rad
  //              G4cout << "Pos X : " << prePos.x()/um << G4endl;
  //              G4cout << "Angle : " << std::asin(mom_dir.x())/rad << G4endl;
              }
              if (emitt_idx == emitt_N-1) {
                rootAnalysisManager1->FillH2(14, prePos.x(), std::asin(mom_dir.x()));
              }
            }
	//    rootAnalysisManager->FillH1(2, aHit->GetKineticEnergy());
          }
    
    //        // Is positron? for the QuickPIC output
          if (aHit->GetParticleName() == "e+") {
            G4ThreeVector prePos = aHit->GetPrePos();
            G4ThreeVector mom_dir = aHit->GetMomentumDirection();
            G4double time = aHit->GetParticleTime();   // nano second [ns]
            G4double c_light =    3.e-1;   // speed of light in [m/ns]
            G4double z_init  = - 100.e-6; // Always start simulation at z=-50 um
            csvAnalysisManager->FillNtupleDColumn(3, 0, prePos.x()/m                    );
            csvAnalysisManager->FillNtupleDColumn(3, 1, prePos.y()/m                    );
            csvAnalysisManager->FillNtupleDColumn(3, 2, mom_dir.x()                     );
            csvAnalysisManager->FillNtupleDColumn(3, 3, mom_dir.y()                     );
            csvAnalysisManager->FillNtupleDColumn(3, 4, c_light*time-prePos.z()/m+z_init);
            csvAnalysisManager->FillNtupleDColumn(3, 5, E_gamma                           );
            csvAnalysisManager->AddNtupleRow(3);
            rootAnalysisManager1->FillH1(5, aHit->GetKineticEnergy());
          }
    
      // Is muon? for the QuickPIC output
          if (aHit->GetParticleName() == "mu-") {
            G4ThreeVector prePos = aHit->GetPrePos();
            G4ThreeVector mom_dir = aHit->GetMomentumDirection();
            G4double time = aHit->GetParticleTime();   // nano second [ns]
            G4double c_light =    3.e-1;   // speed of light in [m/ns]
            G4double z_init  = - 50.e-6; // Always start simulation at z=-50 um
            csvAnalysisManager->FillNtupleDColumn(5, 0, prePos.x()/m                    );
            csvAnalysisManager->FillNtupleDColumn(5, 1, prePos.y()/m                    );
            csvAnalysisManager->FillNtupleDColumn(5, 2, mom_dir.x()                     );
            csvAnalysisManager->FillNtupleDColumn(5, 3, mom_dir.y()                     );
            csvAnalysisManager->FillNtupleDColumn(5, 4, c_light*time-prePos.z()/m+z_init);
            csvAnalysisManager->FillNtupleDColumn(5, 5, E_gamma                           );
            csvAnalysisManager->AddNtupleRow(5);
            rootAnalysisManager1->FillH1(13, aHit->GetKineticEnergy());
          }
      // Is muon? for the QuickPIC output
          if (aHit->GetParticleName() == "mu+") {
            G4ThreeVector prePos = aHit->GetPrePos();
            G4ThreeVector mom_dir = aHit->GetMomentumDirection();
            G4double time = aHit->GetParticleTime();   // nano second [ns]
            G4double c_light =    3.e-1;   // speed of light in [m/ns]
            G4double z_init  = - 50.e-6; // Always start simulation at z=-50 um
            csvAnalysisManager->FillNtupleDColumn(6, 0, prePos.x()/m                    );
            csvAnalysisManager->FillNtupleDColumn(6, 1, prePos.y()/m                    );
            csvAnalysisManager->FillNtupleDColumn(6, 2, mom_dir.x()                     );
            csvAnalysisManager->FillNtupleDColumn(6, 3, mom_dir.y()                     );
            csvAnalysisManager->FillNtupleDColumn(6, 4, c_light*time-prePos.z()/m+z_init);
            csvAnalysisManager->FillNtupleDColumn(6, 5, E_gamma                           );
            csvAnalysisManager->AddNtupleRow(6);
            rootAnalysisManager1->FillH1(14, aHit->GetKineticEnergy());
          }

      // Is pion? 
          if (aHit->GetParticleName() == "pi-") {
            G4ThreeVector prePos = aHit->GetPrePos();
            G4ThreeVector mom_dir = aHit->GetMomentumDirection();
//            G4double time = aHit->GetParticleTime();   // nano second [ns]
            rootAnalysisManager1->FillH1(15, aHit->GetKineticEnergy());
          }
      // Is muon? for the QuickPIC output
          if (aHit->GetParticleName() == "pi+") {
            G4ThreeVector prePos = aHit->GetPrePos();
            G4ThreeVector mom_dir = aHit->GetMomentumDirection();
//            G4double time = aHit->GetParticleTime();   // nano second [ns]
            rootAnalysisManager1->FillH1(16, aHit->GetKineticEnergy());
          }
        }
      }
    }
  }//End fHCID2

  // ===============================================================================
  // fHCID3  At the collimator
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
      for (G4int i = 0; i < n_hit; i++) {
        VirtualHit* aHit = (*Virtual)[i];
        // Is gamma? 
        if (aHit->GetParticleName() == "gamma") {
        // Generate 2D histogram
          G4ThreeVector prePos = aHit->GetPrePos();
          G4ThreeVector mom_dir = aHit->GetMomentumDirection();
          rootAnalysisManager1->FillH2(1, prePos.x(), prePos.y(), aHit->GetKineticEnergy()/MeV);
          rootAnalysisManager1->FillH2(2, std::asin(mom_dir.x()), std::asin(mom_dir.y()), aHit->GetKineticEnergy()/MeV);


    //  Fill second H1, radiated energy per 1 cm2
          if ( (abs(prePos.x())/mm < 5) && (abs(prePos.y())/mm < 5))
          {
              rootAnalysisManager1->FillH1(2, aHit->GetKineticEnergy(), aHit->GetKineticEnergy()/MeV);
    // Divide bremsstrahlung and synchrotron radiation
          if (aHit->GetCreatorProcessName() == "eBrem")
            rootAnalysisManager1->FillH1(10, aHit->GetKineticEnergy(), aHit->GetKineticEnergy()/MeV);
          if (aHit->GetCreatorProcessName() == "SynchrotronRadiation")
            rootAnalysisManager1->FillH1(11, aHit->GetKineticEnergy(), aHit->GetKineticEnergy()/MeV);
          }
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
          rootAnalysisManager1->FillH1(7, aHit->GetKineticEnergy());
        }
      }
    }
  }

  // ===============================================================================
  // fHCID4
  // ===============================================================================
  if (fHCID4<0) {
    fHCID4 = sdm->GetCollectionID("MSpec/MSpecHitsCollection");
  }

  G4HCofThisEvent* mHCE4 = event->GetHCofThisEvent();
  MSpecHitsCollection* MSpec = NULL;
  if (mHCE4) {
    MSpec = (MSpecHitsCollection*)(mHCE4->GetHC(fHCID4));
  }

  if (MSpec) { 
    G4int n_hit = MSpec->entries();
// Only the first entry is taken
    if (n_hit>0) {
      for (G4int i = 0; i < n_hit; i++) {
        MSpecHit* aHit = (*MSpec)[i];
        // Is gamma? 
        if (aHit->GetParticleName() == "gamma") {
        // Generate 2D histogram
          G4ThreeVector prePos = aHit->GetPrePos();
          G4ThreeVector mom_dir = aHit->GetMomentumDirection();
          rootAnalysisManager1->FillH2(7, prePos.x(), prePos.y());
          rootAnalysisManager1->FillH2(8, std::asin(mom_dir.x()), std::asin(mom_dir.y()));
        }
        // Is electron?
        if (aHit->GetParticleName() == "e-"   ) {
        // Generate 2D histogram
          G4ThreeVector prePos = aHit->GetPrePos();
          G4ThreeVector mom_dir = aHit->GetMomentumDirection();
          rootAnalysisManager1->FillH2(9, prePos.x(), prePos.y());
          rootAnalysisManager1->FillH2(10, std::asin(mom_dir.x()), std::asin(mom_dir.y()));
        }
        // Is positron?
        if (aHit->GetParticleName() == "e+"   ) {
        // Generate 2D histogram
          G4ThreeVector prePos = aHit->GetPrePos();
          G4ThreeVector mom_dir = aHit->GetMomentumDirection();
          rootAnalysisManager1->FillH2(11, prePos.x(), prePos.y());
          rootAnalysisManager1->FillH2(12, std::asin(mom_dir.x()), std::asin(mom_dir.y()));
          rootAnalysisManager1->FillH1(8, aHit->GetKineticEnergy());
        }
      }
    }
  }



  G4Run::RecordEvent(event);
}
