#ifndef DETECTORMESSENGER_H
#define DETECTORMESSENGER_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

class DetectorMessenger : public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction*);
  virtual ~DetectorMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  DetectorConstruction* fDetector;
  G4UIdirectory*        fDirectory;
  G4UIcmdWithAString*          fWorldMaterialCmd;
  G4UIcmdWithAString*          fTargetMaterialCmd;
  G4UIcmdWithAString*          fTubeMaterialCmd;
  G4UIcmdWithAString*          fDetectorMaterialCmd;
  G4UIcmdWithADoubleAndUnit*   fTargetXYSizeCmd;
  G4UIcmdWithADoubleAndUnit*   fTargetThicknessCmd;

  G4UIcmdWithADoubleAndUnit*   fFieldStrengthCmd;
  G4UIcmdWithADoubleAndUnit*   fFieldWavelengthCmd;

  G4UIcmdWithADoubleAndUnit*   fStepMaxCmd;

  G4UIcmdWithABool*   fInstabilityCmd;
  G4UIcmdWithAnInteger*   fnReplicas_zCmd;
};

#endif /* DETECTORMESSENGER_H */
