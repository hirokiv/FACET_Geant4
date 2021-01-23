#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
  :G4UImessenger(), fDetector(detector), fDirectory(nullptr)
{
  fDirectory = new G4UIdirectory("/ELI-ALPS/");
  fDirectory->SetGuidance("Detector setup.");

  fDirectory = new G4UIdirectory("/ELI-ALPS/SYLOSeBeam/");
  fDirectory->SetGuidance("Detector setup.");

  fDirectory = new G4UIdirectory("/FACET/");
  fDirectory->SetGuidance("FACET related setup.");

  fWorldMaterialCmd = new G4UIcmdWithAString("/ELI-ALPS/SYLOSeBeam/worldMaterial", this);
  fWorldMaterialCmd->SetGuidance("Set the world material.");
  fWorldMaterialCmd->SetParameterName("worldMat", false);
  fWorldMaterialCmd->SetDefaultValue("G4Galactic");
  fWorldMaterialCmd->AvailableForStates(G4State_PreInit);

  fTargetMaterialCmd = new G4UIcmdWithAString("/FACET/targetMaterial", this);
  fTargetMaterialCmd->SetGuidance("Set the target material.");
  fTargetMaterialCmd->SetParameterName("targetMat", true);
  fTargetMaterialCmd->AvailableForStates(G4State_PreInit);

  fTubeMaterialCmd = new G4UIcmdWithAString("/FACET/tubeMaterial", this);
  fTubeMaterialCmd->SetGuidance("Set the collimating tube material.");
  fTubeMaterialCmd->SetParameterName("tubeMat", true);
  fTubeMaterialCmd->AvailableForStates(G4State_PreInit);


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

  fTargetThicknessCmd = new G4UIcmdWithADoubleAndUnit("/FACET/targetThickness", this);
  fTargetThicknessCmd->SetGuidance("Set the target thickness.");
  fTargetThicknessCmd->SetParameterName("targetThick", true);
  fTargetThicknessCmd->SetRange("targetThick>0");
  fTargetThicknessCmd->SetUnitCategory("Length");
  fTargetThicknessCmd->SetDefaultValue(1.0*mm);
  fTargetThicknessCmd->SetDefaultUnit("mm");
//  fTargetThicknessCmd->AvailableForStates(G4State_PreInit);
//  fTargetThicknessCmd->SetToBeBroadcasted(false);
  fFieldStrengthCmd = new G4UIcmdWithADoubleAndUnit("/FACET/fieldStrength", this);
  fFieldStrengthCmd->SetGuidance("Set the magnetic field intensity.");
  fFieldStrengthCmd->SetParameterName("fieldStrength", true);
  fFieldStrengthCmd->SetRange("fieldStrength>0");
//  fFieldStrengthCmd->SetUnitCategory("Length");
  fFieldStrengthCmd->SetDefaultValue(1.0*tesla);
  fFieldStrengthCmd->SetDefaultUnit("tesla");

  fFieldWavelengthCmd = new G4UIcmdWithADoubleAndUnit("/FACET/fieldWavelength", this);
  fFieldWavelengthCmd->SetGuidance("Set the magnetic field wavelength.");
  fFieldWavelengthCmd->SetParameterName("fieldWavelength", true);
  fFieldWavelengthCmd->SetRange("fieldWavelength>0");
//  fFieldWavelengthCmd->SetUnitCategory("Length");
  fFieldWavelengthCmd->SetDefaultValue(2.0*um);
  fFieldWavelengthCmd->SetDefaultUnit("um");

  fInstabilityCmd = new G4UIcmdWithABool("/FACET/instability", this);
  fInstabilityCmd->SetGuidance("Set the target magneticfield.");
  fInstabilityCmd->SetParameterName("Instability", true);
//  fFieldWavelengthCmd->SetUnitCategory("Length");
  fInstabilityCmd->SetDefaultValue(false);

  fStepMaxCmd = new G4UIcmdWithADoubleAndUnit("/FACET/stepMax",this);
  fStepMaxCmd->SetGuidance("Define a step max");
  fStepMaxCmd->SetParameterName("stepMax", true);
  fStepMaxCmd->SetUnitCategory("Length");
//  fStepMaxCmd->AvailableForStates(G4State_Idle);

  fnReplicas_zCmd = new G4UIcmdWithAnInteger("/FACET/nReplicas_z",this);
  fnReplicas_zCmd->SetGuidance("Define a nomber of replicas inside the target");
  fnReplicas_zCmd->SetParameterName("NReplicas_z", true);
  fnReplicas_zCmd->SetDefaultValue(1);
//  fNReplicas_zCmd->SetUnitCategory("Length");
}

DetectorMessenger::~DetectorMessenger()
{
  delete fTargetThicknessCmd;
  delete fTargetXYSizeCmd;
  delete fDetectorMaterialCmd;
  delete fTargetMaterialCmd;
  delete fTubeMaterialCmd;
  delete fWorldMaterialCmd;
  delete fDirectory;

  delete fFieldStrengthCmd;
  delete fFieldWavelengthCmd;
  delete fInstabilityCmd;
  delete fStepMaxCmd;
  delete fnReplicas_zCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fWorldMaterialCmd) {
    fDetector->SetWorldMaterial(newValue);
  }
  if (command == fTargetMaterialCmd) {
    fDetector->SetTargetMaterial(newValue);
  }
  if (command == fTubeMaterialCmd) {
    fDetector->SetTubeMaterial(newValue);
  }
  if (command == fTargetXYSizeCmd) {
    fDetector->SetTargetXYSize(fTargetXYSizeCmd->GetNewDoubleValue(newValue));
  }
  if (command == fTargetThicknessCmd) {
    fDetector->SetTargetThickness(fTargetThicknessCmd->GetNewDoubleValue(newValue));
  }
  if (command == fFieldStrengthCmd) {
    fDetector->SetFieldStrength(fFieldStrengthCmd->GetNewDoubleValue(newValue));
 // yet undefined   fDetector->SetMagneticFieldInsideTarget();
  }
  if (command == fFieldWavelengthCmd) {
    fDetector->SetFieldWavelength(fFieldWavelengthCmd->GetNewDoubleValue(newValue));
 // yet undefined   fDetector->SetMagneticFieldInsideTarget();
  }
  if (command == fInstabilityCmd) {
    fDetector->SetInstability(fInstabilityCmd->GetNewBoolValue(newValue));
 // yet undefined   fDetector->SetMagneticFieldInsideTarget();
  }
  if (command == fStepMaxCmd) { 
    G4cout << "SetMax triggered" << G4endl;
    fDetector->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  }
  if (command == fnReplicas_zCmd) { 
    G4cout << "Set Nreplicas_z" << G4endl;
    fDetector->SetNReplicas_z(fnReplicas_zCmd->GetNewIntValue(newValue));
  }

}
