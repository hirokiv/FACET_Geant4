#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SensitiveTarget.hh"
#include "SensitiveDetector.hh"
#include "SensitiveVirtual.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

DetectorConstruction::DetectorConstruction()
  :G4VUserDetectorConstruction(), fCheckOverlaps(true), fWorldVolume(nullptr),
   fTargetLogicalVolume(nullptr), fDetectorLogicalVolume(nullptr)
{
  G4cout << "DetectorConstruction Constructor" << G4endl;
  fTargetXYSize = 10*cm;
  fTargetThickness = 1*mm;
  G4cout << "ComputeParameters" << G4endl;
  ComputeParameters();
  SetWorldMaterial("G4_Galactic");
  SetTargetMaterial("G4_W");
  SetDetectorMaterial("G4_Galactic");
  SetVirtualMaterial("G4_Galactic");
//  SetPlateMaterial("G4_P");
  fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction() {
  delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // ==========================================================================
  //The world
  // ==========================================================================
  G4Orb* solidWorld = new G4Orb("World", fWorldRadius);
  G4LogicalVolume* lWorld = new G4LogicalVolume(solidWorld, fWorldMaterial, "World");
  fWorldVolume = new G4PVPlacement(0,
                                   G4ThreeVector(0, 0, 0),
                                   lWorld,
                                   "World",
                                   0,
                                   false,
                                   0,
                                   fCheckOverlaps);
  lWorld->SetVisAttributes(G4VisAttributes::Invisible);

  // ==========================================================================
  //The target
  // ==========================================================================
  G4Box* solidTarget = new G4Box("Target", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2);
  fTargetLogicalVolume = new G4LogicalVolume(solidTarget, fTargetMaterial, "Target");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fTargetThickness/2),
                    fTargetLogicalVolume,
                    "Target",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);

  // ==========================================================================
  //The detector
  // ==========================================================================
  G4Sphere* solidDetector = new G4Sphere("Detector",
                                         fDetectorRadius,
                                         fDetectorRadius + 1*mm,
                                         0*degree,
                                         360*degree,
                                         0*degree,
                                         180*degree);
//  G4Box* solidDetector = new G4Box("Target", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2);
  fDetectorLogicalVolume = new G4LogicalVolume(solidDetector, fDetectorMaterial, "Detector");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, 0),
                    fDetectorLogicalVolume,
                    "Detector",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);
  // fDetectorLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);
  
  // ==========================================================================
  // Virtual detector
  // ==========================================================================
  G4Box* solidVirtual = new G4Box("Virtual", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2);
  fVirtualLogicalVolume = new G4LogicalVolume(solidVirtual, fVirtualMaterial, "Virtual");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fTargetThickness/2 + fTargetThickness + 1.*m),
                    fVirtualLogicalVolume,
                    "Virtual",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);


  return fWorldVolume;
}

void DetectorConstruction::ConstructSDandField() {
  G4cout << "ConstructSDandField" << G4endl;
  SensitiveTarget* sTarget = new SensitiveTarget("Target", "TargetHitsCollection");
  SetSensitiveDetector(fTargetLogicalVolume, sTarget);
  G4SDManager::GetSDMpointer()->AddNewDetector(sTarget);

  SensitiveDetector* sDetector = new SensitiveDetector("Detector", "DetectorHitsCollection");
  SetSensitiveDetector(fDetectorLogicalVolume, sDetector);
  G4SDManager::GetSDMpointer()->AddNewDetector(sDetector);

  SensitiveVirtual* sVirtual = new SensitiveVirtual("Virtual", "VirtualHitsCollection");
  SetSensitiveDetector(fVirtualLogicalVolume, sVirtual);
  G4SDManager::GetSDMpointer()->AddNewDetector(sVirtual);



}

void DetectorConstruction::DefineMaterials() {

}

void DetectorConstruction::ComputeParameters(){
  fDetectorRadius = fTargetXYSize*1.5;
  fWorldRadius = fDetectorRadius*15.;
}

G4Material* DetectorConstruction::SetMaterial(G4String mat){
  G4cout << "Set material" << G4endl;
  G4NistManager* man = G4NistManager::Instance();
  G4cout << "instance of NIST manager found" << G4endl;
  man->SetVerbose(1);
  G4Material* newMat = man->FindOrBuildMaterial(mat);
  G4cout << "Build material" << newMat << G4endl;
  if (mat) {
    G4cout << "The " << newMat->GetName() << " material is selected." << G4endl;
  } else {
      G4cout << "The " << mat << " material is not found!" << G4endl;
    }
    return newMat;
}

void DetectorConstruction::SetWorldMaterial(G4String mat){
  G4cout << "Set World Material" << G4endl;
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fWorldMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetTargetMaterial(G4String mat){
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fTargetMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetDetectorMaterial(G4String mat){
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fDetectorMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetVirtualMaterial(G4String mat){
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fVirtualMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
