#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "SensitiveTarget.hh"
#include "SensitiveDetector.hh"
#include "SensitiveVirtual.hh"
#include "SensitiveMSpec.hh"
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
#include "G4Mag_UsualEqRhs.hh"
#include "G4ChordFinder.hh"
#include "G4SimpleRunge.hh"
#include "G4TransportationManager.hh"
#include "G4Tubs.hh"
#include "BrachyMaterial.hh"  // Stainless steel and Liquid Xenon defined
#include "MyField.hh" // Define arbitral magnetic field
#include "G4AutoDelete.hh"
#include "G4UserLimits.hh"
#include "G4PVReplica.hh"


DetectorConstruction::DetectorConstruction()
  :G4VUserDetectorConstruction(), fCheckOverlaps(true), fWorldVolume(nullptr),
   fTargetLogicalVolume(nullptr), fDetectorLogicalVolume(nullptr), fVirtualLogicalVolume(nullptr), fTargetThickness(0*mm), fAmp(0.0*tesla), fLambda(1.*cm), fInstability(false), fStepLimit(nullptr), fnReplicas_z(1)
{
  G4cout << "DetectorConstruction Constructor" << G4endl;

  fVirtualThickness = 1* nm;
  fVirtualMargin = 0.001* nm;
  fTargetXYSize = 5*cm;
  G4cout << "ComputeParameters" << G4endl;
  ComputeParameters();
  pMat = new BrachyMaterial();
  pMat->DefineMaterials();
  SetWorldMaterial   ("G4_Galactic");
  SetTargetMaterial  ("G4_Ta");
//  SetTargetMaterial  ("G4_Ta");
//  SetTargetMaterial  ("G4_Galactic");
  SetDetectorMaterial("G4_Galactic");
  SetMagnetMaterial  ("G4_Galactic");
  SetTubeMaterial    ("StainlessSteel");
//  SetTubeMaterial    ("G4_Galactic");
  SetVirtualMaterial ("G4_Galactic");

  fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction() {
  delete fDetectorMessenger;
  delete fStepLimit;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
//  G4double const Tesla = volt*second/meter2; // Define tesla
  G4bool DipoleField = 0;

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
  //A thin plate before target
  // ==========================================================================
  G4double fPlateThickness = 1 * nm;
  G4Box* solidPlate = new G4Box("Plate", fTargetXYSize/2, fTargetXYSize/2, fPlateThickness/2);
  G4LogicalVolume* fPlateLogicalVolume = new G4LogicalVolume(solidPlate, fWorldMaterial, "Plate");


  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, -fPlateThickness),
                    fPlateLogicalVolume,
                    "Plate",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);


  // ==========================================================================
  //The target
  // ==========================================================================
  G4Box* solidTarget = new G4Box("Target", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2);
  fTargetLogicalVolume = new G4LogicalVolume(solidTarget, fTargetMaterial, "Target");


//    G4int nReplicas_z = 50;  // no. of divisions in z
    G4Material* slicedTargetMaterial = fTargetMaterial;
    G4Box* solidSlicedTarget = new G4Box("slicedTarget", fTargetXYSize/2, fTargetXYSize/2, fTargetThickness/2/fnReplicas_z);
    G4LogicalVolume* logicalSlicedTarget = new G4LogicalVolume(solidSlicedTarget, slicedTargetMaterial, "slicedTarget");

    new G4PVReplica("physTarget",
					logicalSlicedTarget,  // child volume
					fTargetLogicalVolume, // Mother volume
					kZAxis,               // Replication axis in z
					fnReplicas_z,          // Number of replication
					fTargetThickness/fnReplicas_z); // Width



  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fTargetThickness/2+fVirtualMargin),
                    fTargetLogicalVolume,
                    "Target",
                    lWorld,
                    false,
                    0,
                    fCheckOverlaps);




// For setting magnet so that it overlaps on the target
  if (fInstability == 1){ 
    SetMagneticFieldInsideTarget();
  }



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

  G4Box* solidDetector = new G4Box("Detector", fTargetXYSize/2, fTargetXYSize/2, fVirtualThickness/2);
  fDetectorLogicalVolume = new G4LogicalVolume(solidDetector, fDetectorMaterial, "Detector");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fVirtualThickness + fTargetThickness),
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
  G4Box* solidVirtual = new G4Box("Virtual_temp", fTargetXYSize/2, fTargetXYSize/2, fVirtualThickness/2);
  fVirtualLogicalVolume = new G4LogicalVolume(solidVirtual, fVirtualMaterial, "Virtual");
  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, fVirtualThickness/2 + Dd),
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
  G4double zp = (0.5 + 0.87) * m;
//  G4double zp = (0.5 + 0.68) * m;
  G4double xp = 0.5 * m;
  G4double fDTiltSize = 100 * cm;
  G4double rotate = 0.52*rad;
  yRot4e->rotateY( -rotate );
  G4Box* solidDTilt = new G4Box("Virtual", fDTiltSize/2, fDTiltSize/2, fVirtualThickness/2);
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
  yRot4p->rotateY( rotate );

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
    G4double fMRMin =   0.0*cm;
    G4double fMRMax =  30.0*cm; // Magnet radius
    G4double fMDz   =  20.0*cm; // Thickness
    G4double fMSPhi =   0.0*deg;
    G4double fMDPhi = 180.0*deg;

//    G4Box* solidMagnet = new G4Box("Virtual", fMRMax/2, fMRMax/2, fMRMax/2);
    G4VSolid* solidMagnet = new G4Tubs("Magnet", fMRMin, fMRMax, fMDz/2, fMSPhi, fMDPhi);
    fMagnetLogicalVolume = new G4LogicalVolume(solidMagnet, fMagnetMaterial, "Magnet");
  
    if ( DipoleField == 1 ){
//        G4MagneticField* magField = new MyField(fTargetThickness, fTargetXYSize);
      G4MagneticField* magField = new G4UniformMagField( G4ThreeVector(0.0, 0.5 * tesla, 0.0 ));
    //    G4FieldManager* globalFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    //    globalFieldMgr->SetDetectorField(magField);
        G4FieldManager* localFieldMgr = new G4FieldManager(magField);
        fMagnetLogicalVolume->SetFieldManager(localFieldMgr, true);
    };


    G4RotationMatrix* xRot = new G4RotationMatrix;
    xRot->rotateX(-90*deg);
    new G4PVPlacement(xRot,
                      G4ThreeVector(0, 0, fVirtualThickness/2 +  fTargetThickness + Dd),
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
  SetSensitiveDetector(fDetectorLogicalVolume, sDetector);
  G4SDManager::GetSDMpointer()->AddNewDetector(sDetector);

  SensitiveVirtual* sVirtual = new SensitiveVirtual("Virtual", "VirtualHitsCollection");
  SetSensitiveDetector(fVirtualLogicalVolume, sVirtual);
  G4SDManager::GetSDMpointer()->AddNewDetector(sVirtual);

  SensitiveVirtual* sMSpec = new SensitiveVirtual("MSpec", "MSpecHitsCollection");
  SetSensitiveDetector(fDTiltLogicalVolume, sMSpec);
  G4SDManager::GetSDMpointer()->AddNewDetector(sMSpec);


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

void DetectorConstruction::SetTargetThickness(G4double thickness){
  G4cout << "Set Target Thickness" << G4endl;
  fTargetThickness = thickness;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetFieldStrength(G4double amp){
  G4cout << "Set Field Strength" << G4endl;
  fAmp = amp;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetFieldWavelength(G4double lambda){
  G4cout << "Set Field Wavelength" << G4endl;
  fLambda = lambda;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetInstability(G4bool flag){
  fInstability = flag;
  if (flag==1) {
    G4cout << "############# Filamentation instability turned ON ##############" << G4endl; 
    G4cout << "Set magnetic field inside the target by the instability growth" << G4endl;
  }
}

void DetectorConstruction::SetMaxStep( G4double maxStep )
{
  G4cout << "Set maxStepsize inside the target" << G4endl;
  if (( fStepLimit ) && ( maxStep > 0.)) 
  { 
    G4cout << "fStepLimit exists" << G4endl;
    fStepLimit->SetMaxAllowedStep(maxStep); 
  }
}

void DetectorConstruction::SetNReplicas_z( G4int n )
{
  G4cout << "Set No of replicas inside the target" << G4endl;
  fnReplicas_z = n;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}



void DetectorConstruction::SetMagneticFieldInsideTarget() {
    G4MagneticField* targetField = new MyField(fAmp,fLambda);
    G4FieldManager* targetFieldMgr = new G4FieldManager(targetField);
//=========
  // Set step length
//    targetFieldMgr->CreateChordFinder(targetField);
    G4Mag_UsualEqRhs* fEquation = new G4Mag_UsualEqRhs(targetField);
    G4MagIntegratorStepper *pStepper = new G4SimpleRunge(fEquation);
    G4ChordFinder *pChordFinder = new G4ChordFinder(targetField, 1.*um, pStepper);
    targetFieldMgr->SetChordFinder(pChordFinder);
//=========
    fTargetLogicalVolume->SetFieldManager(targetFieldMgr, true);
    G4cout << "MagneticField configured inside the target" << G4endl;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();

    G4double maxStep = fLambda/5.;
// Change stepsize dependeing on the Characteristic wavelength
    fStepLimit = new G4UserLimits(maxStep);
    fTargetLogicalVolume->SetUserLimits(fStepLimit);
}


