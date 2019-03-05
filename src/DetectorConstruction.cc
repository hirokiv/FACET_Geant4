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
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "BrachyMaterial.hh"  // Stainless steel defined

DetectorConstruction::DetectorConstruction()
  :G4VUserDetectorConstruction(), fCheckOverlaps(true), fWorldVolume(nullptr),
   fTargetLogicalVolume(nullptr), fDetectorLogicalVolume(nullptr), fVirtualLogicalVolume(nullptr)
{
  G4cout << "DetectorConstruction Constructor" << G4endl;
  fTargetXYSize = 5*cm;
  fTargetThickness = 1.0*mm;
  G4cout << "ComputeParameters" << G4endl;
  ComputeParameters();
  pMat = new BrachyMaterial();
  pMat->DefineMaterials();
  SetWorldMaterial   ("G4_Galactic");
  SetTargetMaterial  ("G4_Ta");
  SetDetectorMaterial("G4_Galactic");
  SetMagnetMaterial  ("G4_Galactic");
  SetTubeMaterial    ("StainlessSteel");
  SetVirtualMaterial ("G4_Galactic");

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
//   G4Sphere* solidDetector = new G4Sphere("Detector",
//                                          fDetectorRadius,
//                                          fDetectorRadius + 1*mm,
//                                          0*degree,
//                                          360*degree,
//                                          0*degree,
//                                          180*degree);

  G4Box* solidDetector = new G4Box("Detector", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/20000000);
  fDetectorLogicalVolume = new G4LogicalVolume(solidDetector, fDetectorMaterial, "Detector");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fTargetThickness/20000000 + fTargetThickness),
                    fDetectorLogicalVolume,
                    "Detector",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);
  // fDetectorLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);
  
  // ==========================================================================
  // Virtual detector just after the collimator
  // ==========================================================================
  G4double const Dd = 0.5 * m; // Where we place the magnet
  G4Box* solidVirtual = new G4Box("Virtual_temp", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2);
  fVirtualLogicalVolume = new G4LogicalVolume(solidVirtual, fVirtualMaterial, "Virtual");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fTargetThickness/2 + Dd),
                    fVirtualLogicalVolume,
                    "Virtual",
//                    "Virtual",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);

  // ==========================================================================
  // Virtual detector for electrons
  // ==========================================================================
  G4RotationMatrix* yRot4e = new G4RotationMatrix;
  G4double zp = (0.5 + 0.68) * m;
//  G4double zp = (0.5 + 0.68) * m;
  G4double xp = 0.18 * m;
  G4double fDTiltSize = 20 * cm;
  yRot4e->rotateY(-0.26*rad);
  G4Box* solidDTilt = new G4Box("Virtual", fDTiltSize/2, fDTiltSize/2, fTargetThickness/2);
  fDTiltLogicalVolume = new G4LogicalVolume(solidDTilt, fVirtualMaterial, "Virtual");

  new G4PVPlacement(yRot4e,
                    G4ThreeVector(xp, 0, zp),
                    fDTiltLogicalVolume,
                    "Virtual4e",
//                    "Virtual",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);
  // ==========================================================================
  // Virtual detector for positrons
  // ==========================================================================
  G4RotationMatrix* yRot4p = new G4RotationMatrix;
  yRot4p->rotateY(0.26*rad);

  new G4PVPlacement(yRot4p,
                    G4ThreeVector(-xp, 0, zp),
                    fDTiltLogicalVolume,
                    "Virtual4p",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps); 
//  // ==========================================================================
//  // Virtual detector for gammas
//  // ==========================================================================
//  G4RotationMatrix* yRot4g = new G4RotationMatrix;
//  yRot4g->rotateY(0*deg);
//
//  new G4PVPlacement(yRot4g,
//                    G4ThreeVector(0, 0, fTargetThickness/2 + fTargetThickness + 2*dDist),
//                    fVirtualLogicalVolume,
//                    "Virtual4g",
//                    lWorld,
//                    false,
//                    0,
//                    fCheckOverlaps); 

//  // ==========================================================================
//  // Virtual detector2 for QuickPIC input
//  // ==========================================================================
//  G4Box* solidQPIC = new G4Box("QPIC", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2);
//  fQPICLogicalVolume = new G4LogicalVolume(solidQPIC, fVirtualMaterial, "QPIC");
//  new G4PVPlacement(0,
//                    G4ThreeVector(0, 0, fTargetThickness/2 + fTargetThickness),
//                    fQPICLogicalVolume,
//                    "QPIC",
//                    lWorld,
//                    false,
//                    0,
//                    fCheckOverlaps);

  // ==========================================================================
  // Dipole magnet
  // ==========================================================================
    G4double const Tesla = volt*second/meter2; // Define tesla
    G4double fMRMin =   0.0*cm;
    G4double fMRMax =  30.0*cm; // Magnet radius
    G4double fMDz   =  20.0*cm; // Thickness
    G4double fMSPhi =   0.0*deg;
    G4double fMDPhi = 180.0*deg;
//    G4Box* solidMagnet = new G4Box("Virtual", fMRMax/2, fMRMax/2, fMRMax/2);
    G4VSolid* solidMagnet = new G4Tubs("Magnet", fMRMin, fMRMax, fMDz/2, fMSPhi, fMDPhi);
    fMagnetLogicalVolume = new G4LogicalVolume(solidMagnet, fMagnetMaterial, "Magnet");
  
    G4MagneticField* magField = new G4UniformMagField( G4ThreeVector(0.0, 0.5 * Tesla, 0.0 ));
  //  G4FieldManager* globalFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    G4FieldManager* localFieldMgr = new G4FieldManager(magField);
    fMagnetLogicalVolume->SetFieldManager(localFieldMgr, true);
  //  globalFieldMgr->SetDetectorField(magField);
    G4RotationMatrix* xRot = new G4RotationMatrix;
    xRot->rotateX(-90*deg);
    new G4PVPlacement(xRot,
                      G4ThreeVector(0, 0, fTargetThickness/2 +  fTargetThickness + Dd),
                      fMagnetLogicalVolume,
                      "Magnet",
                      lWorld,
                      false,
                      0,
                      fCheckOverlaps);

  // ==========================================================================
  // Collimater
  // ==========================================================================
  G4double fCRMin =  6.0*cm;
  G4double fCRMax =  7.0*cm;
  G4double fCDz   = Dd;
  G4double fCSPhi =   0.0*deg;
  G4double fCDPhi = 360.0*deg;
  G4VSolid* solidTube = new G4Tubs("Tube", fCRMin, fCRMax, fCDz/2, fCSPhi, fCDPhi);
  fTubeLogicalVolume = new G4LogicalVolume(solidTube, fTubeMaterial, "Tube");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fCDz/2),
                    fTubeLogicalVolume,
                    "Tube",
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
//  SetSensitiveDetector(fDetectorLogicalVolume, sDetector);
  SetSensitiveDetector(fVirtualLogicalVolume, sDetector);
  G4SDManager::GetSDMpointer()->AddNewDetector(sDetector);

  SensitiveVirtual* sVirtual = new SensitiveVirtual("Virtual", "VirtualHitsCollection");
  SetSensitiveDetector(fVirtualLogicalVolume, sVirtual);
//  SetSensitiveDetector(fDTiltLogicalVolume, sVirtual);
  G4SDManager::GetSDMpointer()->AddNewDetector(sVirtual);

//  SensitiveVirtual* sQPIC = new SensitiveVirtual("QPIC", "VirtualHitsCollection");
//  SetSensitiveDetector(fQPICLogicalVolume, sQPIC);
//  G4SDManager::GetSDMpointer()->AddNewDetector(sQPIC);
}

void DetectorConstruction::DefineMaterials() {

}

void DetectorConstruction::ComputeParameters(){
//  fDetectorRadius = fTargetXYSize*1.5;
  fWorldRadius = 250.0 * cm;
}

G4Material* DetectorConstruction::SetMaterial(G4String mat){ G4cout << "Set material" << G4endl;
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
  G4cout << "Set Target Material" << G4endl;
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fTargetMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetDetectorMaterial(G4String mat){
  G4cout << "Set Detector Material" << G4endl;
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fDetectorMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetVirtualMaterial(G4String mat){
  G4cout << "Set Virtual Material" << G4endl;
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fVirtualMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetMagnetMaterial(G4String mat){
  G4cout << "Set Magnet Material" << G4endl;
  G4Material* newMat = SetMaterial(mat);
  if (newMat) {
    fMagnetMaterial = newMat;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetTubeMaterial(G4String mat){
  G4cout << "Set Tube Material" << G4endl;
//  G4Material* newMat = SetMaterial(mat);
  G4Material* newMat = pMat->GetMat(mat);// SetMaterial(mat);
  if (newMat) {
    fTubeMaterial = newMat;
    G4cout << "Set Tube Material called" << G4endl;
  }
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
