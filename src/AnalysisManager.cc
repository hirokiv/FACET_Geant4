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


  const G4int kMaxH1 = 14;
  const G4int kMaxH2 = 13;
  const G4String id1[] = {"dummy", 
                        "edep",        // 1
                        "gspec",       // 2  Gamma irradiance flux density at the virtual detector MeV/cm^2
                        "ang_yz",      // 3
			"elec_spec",   // 4
			"posi_spec",   // 5
                        "Source_spec", // 6
                        "c_posi_spec", // 7  Positron Spectra at the collimator
                        "m_posi_spec", // 8  Positron Spectra at the magnetospectrometer 
                        "pri_espec",   // 9  Spectra of primary electron just behind the target
                        "eBrem",       // 10 Gamma by Bremsstlahlung at the collimator
                        "SynRad",      // 11 Synchrotron rad at the collimator
                        "emittance",   // 12 Electron Emittance at the collimator
                        "dummy"};
  const G4String id2[] = { "dummy", 
// At the virtual detector located at the exit of Collimator
                        "c_space_gamma", 
                        "c_angle_gamma",
			"c_space_electron", 
                        "c_angle_electron",
			"c_space_positron", 
                        "c_angle_positron", 
// At the virtual detector located at the magnetospectrometer
                        "m_space_gamma", 
                        "m_angle_gamma",
			"m_space_electron", 
                        "m_angle_electron",
			"m_space_positron", 
                        "m_angle_positron", 
                        "dummy" 
			};
  const G4String title1[] = {"dummy",
                            "Edep in target [MeV/cm^2]",        //1
                            "Gamma's energy at the collimator [Energy(arb.)/MeV]", //2
                            "YZ direction",                //3
                            "Electron spectra",              //4
                            "Positron spectra",              //5 
		            "Source spectra",              //6
		            "Collimator positron spectra",              //7
		            "Magnetospectrometer spectra",              //8
			    "Primary electron spectra",     //9
                            "Bremsstrahlung gamma's energy at the collimator [Energy(arb.)/MeV]",    //10
                            "SynchrotronRad gamma's energy at the collimator [Energy(arb.)/MeV]",    //11
                            "Emittance per energy at the collimator [Emittance(mm-mrad) within pm 5% Energy range]",    //12
                            "dummy"};                       //
  const G4String title2[] = {
                            "dummy", 
// At the virtual detector located at the exit of Collimator
                            "Spatial 2D dist of Gamma-rays at the collimator", //1 2D
                            "Angular 2D dist of Gamma-rays at the collimator", //2 2D
                            "Spatial 2D dist of electrons at the collimator",  //3 2D
                            "Angular 2D dist of electrons at the collimator",  //4 2D
                            "Spatial 2D dist of positrons at the collimator",  //5 2D
                            "Angular 2D dist of positrons at the collimator",  //6 2D
// At the virtual detector located at the magnetospectrometer
                            "Spatial 2D dist of Gamma-rays at the magnetospectrometer", //7 2D
                            "Angular 2D dist of Gamma-rays at the magnetospectrometer", //8 2D
                            "Spatial 2D dist of electrons at the magnetospectrometer",  //9 2D
                            "Angular 2D dist of electrons at the magnetospectrometer",  //10 2D
                            "Spatial 2D dist of positrons at the magnetospectrometer",  //11 2D
                            "Angular 2D dist of positrons at the magnetospectrometer",  //12 2D
// At the virtual detector located at the exit of Collimator
                            "Spatial 2D dist of gammas at the collimator",  //13 2D
                            "dummy" 
			};            
  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated
  // as we have not yet set nbins, vmin, vmax
  G4int ih;
//  for (G4int k=0; k<kMaxHisto; k++) {
  for (G4int k=0; k<kMaxH1; k++) {
    ih = rootAnalysisManager1->CreateH1(id1[k], title1[k], nbins, vmin, vmax);
    rootAnalysisManager1->SetH1Activation(ih, true);
  }

  for (G4int k=0; k<kMaxH2; k++) {
    ih = rootAnalysisManager1->CreateH2(id2[k], title2[k], 
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

  csvAnalysisManager->CreateNtuple("04", "Primary Electrons Output for QuickPIC");
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
