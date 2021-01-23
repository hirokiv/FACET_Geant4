#ifndef BRACHYMATERIALMESSENGER_H
#define BRACHYMATERIALMESSENGER_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

class BrachyMaterial;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

class BrachyMaterialMessenger : public G4UImessenger
{
public:
  BrachyMaterialMessenger(BrachyMaterial*);
  virtual ~BrachyMaterialMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

private:
  BrachyMaterial* fMaterial;
  G4UIdirectory* fDirectory;
  G4UIcmdWithADoubleAndUnit*   fTargetXeDensityCmd;
};

#endif /* BRACHYMATERIALMESSENGER_H */
