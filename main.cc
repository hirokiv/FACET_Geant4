#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include "G4PhysListFactory.hh"

#define G4UI_USE
#define G4VIS_USE

int main(int argc, char *argv[])
{
  //Chose the random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  //Construct the default run manager
  #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(<number_of_threads>);
    G4cout << "MT Run Manager" << G4endl;
  #else
    G4RunManager* runManager = new G4RunManager;
    G4cout << "Single-thread Run Manager" << G4endl;
  #endif

  //Set initialization classes
  DetectorConstruction* detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);
  G4cout << "DetectorConstruction.hh" << G4endl;

  //The PhisicsList
  G4bool USERDEFINED = 0;
  if (USERDEFINED == 1)
  {
    PhysicsList* physics = new PhysicsList();
    runManager->SetUserInitialization(physics);
    G4cout << "Physics User defined" << G4endl;
  } else {
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    G4VUserPhysicsList *physics = physListFactory->GetReferencePhysList("QGSP_BERT");
    runManager->SetUserInitialization(physics);
    G4cout << "Physics QGSP_BERT" << G4endl;
  }

  //User action initialization classes
  runManager->SetUserInitialization(new ActionInitialization(detector));
  G4cout << "ActionInitialization.hh" << G4endl;

  //Initialize visualization
  #ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
  #endif

  //User interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  UImanager->ApplyCommand("/control/execute ../util/FACET_initial.mac");
  UImanager->ApplyCommand("/control/execute ../util/FACET_Analysis.mac");
  if (argc!=1)
    {
      //batch mode
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    } else
    {
      runManager->Initialize();
      //define UI session
      #ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      UImanager->ApplyCommand("/control/macroPath ../macro");
      #ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init_vis.mac");
      #else
      UImanager->ApplyCommand("/control/execute init.mac");
      #endif
      ui->SessionStart();
      delete ui;
      #endif
    }

  //Job termination
  #ifdef G4VIS_USE

    delete visManager;

  #endif // G4VIS_USE
  delete runManager;
  return 0;
}

