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
  G4UIcmdWithAString*          fDetectorMaterialCmd;
  G4UIcmdWithADoubleAndUnit*   fTargetXYSizeCmd;
  G4UIcmdWithADoubleAndUnit*   fTargetThicknessCmd;
};

#endif /* DETECTORMESSENGER_H */
