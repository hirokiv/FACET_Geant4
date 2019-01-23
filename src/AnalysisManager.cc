#include "AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisManager.hh"
#include "G4CsvAnalysisManager.hh"

AnalysisManager::AnalysisManager()
  : fFactoryOn(false), fFileName("histograms")
{
  G4cout << "AnalysisManager constructor" << G4endl;
  Book();
  G4cout << "Booking" << G4endl;
}

AnalysisManager::~AnalysisManager()
{
  delete G4RootAnalysisManager::Instance();
  delete G4CsvAnalysisManager::Instance();
}

void AnalysisManager::Book()
{
  G4RootAnalysisManager* rootAnalysisManager = G4RootAnalysisManager::Instance();
  //rootAnalysisManager->SetFileName(fFileName);
  rootAnalysisManager->SetVerboseLevel(1);
  rootAnalysisManager->SetActivation(true);

  const G4int kMaxHisto = 6;
  const G4String id[] = {"dummy", "edep", "espec", "ang_yz", "sspec", "dummy"};
  const G4String title[] = {"dummy",
                            "Edep in target [MeV]",        //1
                            "Energy spectrum at detector", //2
                            "YZ direction",                //3
                            "Source spectra",              //4
                            "dummy"};
  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = rootAnalysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    rootAnalysisManager->SetH1Activation(ih, true);
  }

  G4CsvAnalysisManager* csvAnalysisManager = G4CsvAnalysisManager::Instance();
  csvAnalysisManager->SetFileName("ntuple");
  csvAnalysisManager->SetVerboseLevel(1);
  csvAnalysisManager->SetActivation(true);

  csvAnalysisManager->CreateNtuple("00", "Position-time-kinetic-energy-momentum direction and creator process of created gamma");
  csvAnalysisManager->CreateNtupleDColumn("X [um]");
  csvAnalysisManager->CreateNtupleDColumn("Y [um]");
  csvAnalysisManager->CreateNtupleDColumn("Z [um]");
  csvAnalysisManager->CreateNtupleDColumn("t [ps]");
  csvAnalysisManager->CreateNtupleDColumn("KineticEnergy [keV]");
  csvAnalysisManager->CreateNtupleDColumn("MomentumDirection_X");
  csvAnalysisManager->CreateNtupleDColumn("MomentumDirection_Y");
  csvAnalysisManager->CreateNtupleDColumn("MomentumDirection_Z");
  csvAnalysisManager->CreateNtupleSColumn("Creator process name");
  csvAnalysisManager->FinishNtuple();

  csvAnalysisManager->CreateNtuple("01", "Position-time-energy-momentum direction and creator process of outgoing gamma");
  csvAnalysisManager->CreateNtupleDColumn("X [um]");
  csvAnalysisManager->CreateNtupleDColumn("Y [um]");
  csvAnalysisManager->CreateNtupleDColumn("Z [um]");
  csvAnalysisManager->CreateNtupleDColumn("t [ps]");
  csvAnalysisManager->CreateNtupleDColumn("KineticEnergy [keV]");
  csvAnalysisManager->CreateNtupleDColumn("MomentumDirection_X");
  csvAnalysisManager->CreateNtupleDColumn("MomentumDirection_Y");
  csvAnalysisManager->CreateNtupleDColumn("MomentumDirection_Z");
  csvAnalysisManager->CreateNtupleSColumn("Creator process name");
  csvAnalysisManager->FinishNtuple();
}


void AnalysisManager::PrintStatistic()
{
}
