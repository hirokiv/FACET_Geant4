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
  G4RootAnalysisManager* rootAnalysisManager1 = G4RootAnalysisManager::Instance();
//  G4RootAnalysisManager* rootAnalysisManager2 = G4RootAnalysisManager::Instance();
  //rootAnalysisManager->SetFileName(fFileName);
  rootAnalysisManager1->SetVerboseLevel(1);
  rootAnalysisManager1->SetActivation(true);


  const G4int kMaxHisto = 6;
  const G4String id[] = {"dummy", "edep", "espec", "ang_yz", "sspec",
			"dummy", "space_gamma", "angle_gamma",
			"space_electron", "angle_electron",
			"space_positron", "angle_positron" 
			};
  const G4String title[] = {"dummy",
                            "Edep in target [MeV]",        //1
                            "Energy spectrum at detector", //2
                            "YZ direction",                //3
                            "Source spectra",              //4
			    "dummy",                       //
                            "Spatial 2D dist of Gamma-rays", //1 2D
                            "Angular 2D dist of Gamma-rays", //2 2D
                            "Spatial 2D dist of electrons",  //3 2D
                            "Angular 2D dist of electrons",  //4 2D
                            "Spatial 2D dist of positrons",  //5 2D
                            "Angular 2D dist of positrons",  //6 2D
			};            
  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated
  // as we have not yet set nbins, vmin, vmax
  G4int ih;
//  for (G4int k=0; k<kMaxHisto; k++) {
  for (G4int k=0; k<5; k++) {
    ih = rootAnalysisManager1->CreateH1(id[k], title[k], nbins, vmin, vmax);
    rootAnalysisManager1->SetH1Activation(ih, true);
  }

  for (G4int k=5; k<12; k++) {
    ih = rootAnalysisManager1->CreateH2(id[k], title[k], 
				nbins, -vmax, vmax,   // bins'number, xmin, xmax
				nbins, -vmax, vmax);  // bins'number, xmin, xmax
    rootAnalysisManager1->SetH2Activation(ih, true);
  }
//  rootAnalysisManager2->SetH2Activation(ih, true);

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

  csvAnalysisManager->CreateNtuple("02", "Electrons Output for QuickPIC");
  csvAnalysisManager->CreateNtupleDColumn("X [m]");
  csvAnalysisManager->CreateNtupleDColumn("Y [m]");
  csvAnalysisManager->CreateNtupleDColumn("px/pz");
  csvAnalysisManager->CreateNtupleDColumn("py/pz");
  csvAnalysisManager->CreateNtupleDColumn("CT-Z[m]");
  csvAnalysisManager->CreateNtupleDColumn("Gamma (Energy)");
  csvAnalysisManager->FinishNtuple();

  csvAnalysisManager->CreateNtuple("03", "Positrons Output for QuickPIC");
  csvAnalysisManager->CreateNtupleDColumn("X [m]");
  csvAnalysisManager->CreateNtupleDColumn("Y [m]");
  csvAnalysisManager->CreateNtupleDColumn("px/pz");
  csvAnalysisManager->CreateNtupleDColumn("py/pz");
  csvAnalysisManager->CreateNtupleDColumn("CT-Z[m]");
  csvAnalysisManager->CreateNtupleDColumn("Gamma (Energy)");
  csvAnalysisManager->FinishNtuple();
}


void AnalysisManager::PrintStatistic()
{
}
