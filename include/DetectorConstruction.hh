#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H 1


#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4MagneticField.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolumeStore.hh"
#include "BrachyMaterial.hh"
#include "G4Cache.hh"

class G4VPhysicalVolume;
class DetectorMessenger;
class G4LogicalVolume;
class G4Material;
class G4MagneticField;
class BrachyMaterial;
class F03FieldSetup;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void SetWorldMaterial(G4String);
  void SetTargetMaterial(G4String);
  void SetVirtualMaterial(G4String);
  void SetDetectorMaterial(G4String);
  void SetMagnetMaterial(G4String);
  void SetTubeMaterial(G4String);
  void SetTargetThickness(G4double);

  void SetFieldStrength(G4double);
  void SetFieldWavelength(G4double);

  void SetTargetXYSize(G4double size) {fTargetXYSize = size; ComputeParameters();}
  G4Material* SetMaterial(G4String);

  void SetInstability(G4bool);
  void SetMagneticFieldInsideTarget();
  void SetMaxStep(G4double);
  void SetNReplicas_z(G4int);

private:
  void DefineMaterials();
  void ComputeParameters();
  G4Cache<F03FieldSetup*>  fEmFieldSetup;
  

  G4bool fCheckOverlaps;

  G4VPhysicalVolume* fWorldVolume;
  G4LogicalVolume* fTargetLogicalVolume;
  G4LogicalVolume* fDetectorLogicalVolume;
  G4LogicalVolume* fVirtualLogicalVolume;
  G4LogicalVolume* fMagnetLogicalVolume;
  G4LogicalVolume* fTubeLogicalVolume;
  G4LogicalVolume* fDTiltLogicalVolume;
  G4LogicalVolume* fMFieldLogicalVolume;

//  G4LogicalVolume* fQPICLogicalVolume;
  BrachyMaterial* pMat;

  G4Material* fWorldMaterial;
  G4Material* fTargetMaterial;
  G4Material* fVirtualMaterial;
  G4Material* fDetectorMaterial;
  G4Material* fMagnetMaterial;
  G4Material* fTubeMaterial;

  G4double fTargetXYSize;
  G4double fTargetThickness;
  G4double fVirtualThickness;
  G4double fVirtualMargin;
  G4double fWorldRadius;
  G4double fDetectorRadius;
  DetectorMessenger* fDetectorMessenger;

  // Sinosoidal magnetic field characteristics
  G4double fAmp;
  G4double fLambda;
  G4bool fInstability; // Switch if we turn on the field inside the target
  G4UserLimits* fStepLimit;

  G4int fnReplicas_z;

};

#endif // DETECTORCONSTRUCTION_H
