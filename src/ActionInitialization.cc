#include "ActionInitialization.hh"
#include "PrimaryGenerator.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "AnalysisManager.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* det)
  : G4VUserActionInitialization(), fDetector(det)
{
  G4cout << "Action Initialization Constructor" << G4endl;
}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  G4cout << "BuildForMaster" << G4endl;
  SetUserAction(new RunAction(fDetector));
  G4cout << "BuildForMaster end" << G4endl;
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGenerator());
  G4cout << "Build PrimaryGenerator.hh" << G4endl;
  SetUserAction(new RunAction(fDetector));
  G4cout << "Build RunAction.hh" << G4endl;
}
