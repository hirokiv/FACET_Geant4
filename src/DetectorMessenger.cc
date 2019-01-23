#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
  :G4UImessenger(), fDetector(detector), fDirectory(nullptr)
{
  fDirectory = new G4UIdirectory("/ELI-ALPS/");
  fDirectory->SetGuidance("Detector setup.");

  fDirectory = new G4UIdirectory("/ELI-ALPS/SYLOSeBeam/");
  fDirectory->SetGuidance("Detector setup.");

  fWorldMaterialCmd = new G4UIcmdWithAString("/ELI-ALPS/SYLOSeBeam/worldMaterial", this);
  fWorldMaterialCmd->SetGuidance("Set the world material.");
  fWorldMaterialCmd->SetParameterName("worldMat", false);
  fWorldMaterialCmd->SetDefaultValue("G4Galactic");
  fWorldMaterialCmd->AvailableForStates(G4State_PreInit);

  fTargetMaterialCmd = new G4UIcmdWithAString("/ELI-ALPS/SYLOSeBeam/targetMaterial", this);
  fTargetMaterialCmd->SetGuidance("Set the target material.");
  fTargetMaterialCmd->SetParameterName("targetMat", false);
  fTargetMaterialCmd->AvailableForStates(G4State_PreInit);

  fDetectorMaterialCmd = new G4UIcmdWithAString("/ELI-ALPS/SYLOSeBeam/detectorMaterial", this);
  fDetectorMaterialCmd->SetGuidance("Set the detector material.");
  fDetectorMaterialCmd->SetParameterName("detectorMat", false);
  fDetectorMaterialCmd->SetDefaultValue("G4_Galactic");
  fDetectorMaterialCmd->AvailableForStates(G4State_PreInit);

  fTargetXYSizeCmd = new G4UIcmdWithADoubleAndUnit("/ELI-ALPS/SYLOSeBeam/targetXYSize", this);
  fTargetXYSizeCmd->SetGuidance("Set the target size.");
  fTargetXYSizeCmd->SetParameterName("targetXYSize", false);
  fTargetXYSizeCmd->SetRange("targetXYSize>0");
  fTargetXYSizeCmd->SetUnitCategory("Length");
  fTargetXYSizeCmd->AvailableForStates(G4State_PreInit);
  fTargetXYSizeCmd->SetToBeBroadcasted(false);

  fTargetThicknessCmd = new G4UIcmdWithADoubleAndUnit("/ELI-ALPS/SYLOSeBeam/targetThickness", this);
  fTargetThicknessCmd->SetGuidance("Set the target thickness.");
  fTargetThicknessCmd->SetParameterName("targetThick", false);
  fTargetThicknessCmd->SetRange("targetThick>0");
  fTargetThicknessCmd->SetUnitCategory("Length");
  fTargetThicknessCmd->AvailableForStates(G4State_PreInit);
  fTargetThicknessCmd->SetToBeBroadcasted(false);
}

DetectorMessenger::~DetectorMessenger()
{
  delete fTargetThicknessCmd;
  delete fTargetXYSizeCmd;
  delete fDetectorMaterialCmd;
  delete fTargetMaterialCmd;
  delete fWorldMaterialCmd;
  delete fDirectory;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fWorldMaterialCmd) {
    fDetector->SetWorldMaterial(newValue);
  }
  if (command == fTargetMaterialCmd) {
    fDetector->SetTargetMaterial(newValue);
  }
  if (command == fTargetXYSizeCmd) {
    fDetector->SetTargetXYSize(fTargetXYSizeCmd->GetNewDoubleValue(newValue));
  }
  if (command == fTargetThicknessCmd) {
    fDetector->SetTargetThickness(fTargetThicknessCmd->GetNewDoubleValue(newValue));
  }
}
