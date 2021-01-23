#include "BrachyMaterialMessenger.hh"
#include "BrachyMaterial.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"

BrachyMaterialMessenger::BrachyMaterialMessenger(BrachyMaterial* material)
  :G4UImessenger(), fMaterial(material), fDirectory(nullptr)
{
  fTargetXeDensityCmd = new G4UIcmdWithADoubleAndUnit("/FACET/XeDensity", this);
  fTargetXeDensityCmd->SetGuidance("Set Target Xe Density.");
  fTargetXeDensityCmd->SetParameterName("XeDensity", true);
  fTargetXeDensityCmd->SetRange("XeDensity>0");
  fTargetXeDensityCmd->SetUnitCategory("Xe Density");
  fTargetXeDensityCmd->SetDefaultValue(2.953*g/cm3);
  fTargetXeDensityCmd->SetDefaultUnit("g/cm3");
  fTargetXeDensityCmd->SetToBeBroadcasted(false);
}

BrachyMaterialMessenger::~BrachyMaterialMessenger()
{
  delete fTargetXeDensityCmd;
}

void BrachyMaterialMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == fTargetXeDensityCmd) {
    G4cout << "Set target density triggered" << G4endl;
    G4cout << "Only works with TargetMaterial=liquidXe" << G4endl;
    fMaterial->SetTargetXeDensity(fTargetXeDensityCmd->GetNewDoubleValue(newValue));
  }
}
