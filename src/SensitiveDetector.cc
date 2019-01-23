#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "G4VProcess.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4TouchableHandle.hh"

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

SensitiveDetector::~SensitiveDetector()
{}

void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection
  fHitsCollection = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  //Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}


//------------------------------------------------------------------------------
G4int SensitiveDetector::WriteBeamFile(G4String filename, G4ThreeVector position_World,
   G4ThreeVector mom_dir, G4double kin_ene, G4double time)
//------------------------------------------------------------------------------
{
  FILE *outputfile;         // output stream
  G4double gamma = kin_ene/0.511; // Kinetic energy[MeV] to gamma
  G4double c_light = 3e-1; //  speed of light in [m/ns]
  G4double z_init = - 50e-6; // Always start simulation at z=-50 um 
  // Note time is in ns

  outputfile = fopen(filename, "a");  // open file in write mode
  if (outputfile == NULL) {          // 
    printf("cannot open\n");         // 
    exit(1);                         //
  }

  G4double l = sqrt(mom_dir.x()*mom_dir.x() + mom_dir.y()*mom_dir.y() + mom_dir.z()*mom_dir.z());


  // X[m] Y[m] px/pz py/pz CT-Z[m] Gamma
  fprintf(outputfile, "%16.8e %16.8e %16.8e %16.8e %16.8e %16.8e \n",
  position_World.x()/m, position_World.y()/m,
   mom_dir.x()/l, mom_dir.y()/l,
   c_light*time - position_World.z()/m + z_init, gamma); // write in file

  fclose(outputfile);          // Close file
  return 0;
}




G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  DetectorHit* newHit = new DetectorHit();
  //G4cout << "New hit!" << G4endl;

  newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newHit->SetParentID(aStep->GetTrack()->GetParentID());
  newHit->SetEdep(aStep->GetTotalEnergyDeposit());
  newHit->SetPrePos(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetPrePos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetParticleTime(aStep->GetPreStepPoint()->GetGlobalTime());
  newHit->SetParticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  if (aStep->GetTrack()->GetCreatorProcess()) {
    newHit->SetCreatorProcessName(aStep->GetTrack()->GetCreatorProcess()->GetProcessName());
  }
  newHit->SetKineticEnergy(aStep->GetTrack()->GetKineticEnergy());
  newHit->SetMomentumDirection(aStep->GetPreStepPoint()->GetMomentumDirection());
  newHit->SetCharge(aStep->GetPreStepPoint()->GetCharge());
  newHit->SetCurrentStepNumber(aStep->GetTrack()->GetCurrentStepNumber());

//    G4cout << aStep->GetPreStepPoint()->GetMomentumDirection().x() <<  "  ";
//    G4cout << aStep->GetPreStepPoint()->GetMomentumDirection().y() <<  "  ";
//    G4cout << aStep->GetPreStepPoint()->GetMomentumDirection().z() <<  G4endl;

  fHitsCollection->insert(newHit);
  //newHit->Print();
  ///////////////////////////////////////////////////////
  G4String particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName(); 
  G4ThreeVector position_World = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector momentum_dir = aStep->GetPreStepPoint()->GetMomentumDirection();
  G4double kin_energy = aStep->GetTrack()->GetKineticEnergy();
  G4double time = aStep->GetPreStepPoint()->GetGlobalTime();
  // Write the position and time into the file for the QuickPIC input
  if (particleName=="e-") WriteBeamFile("beam_electron.txt", position_World, momentum_dir, kin_energy, time);
  if (particleName=="e+") WriteBeamFile("beam_positron.txt", position_World, momentum_dir, kin_energy, time);
  if (particleName=="gamma") WriteBeamFile("beam_gamma___.txt", position_World, momentum_dir, kin_energy, time);
  if (particleName=="opticalphoton") WriteBeamFile("beam_ophoton_.txt", position_World, momentum_dir, kin_energy, time);
  ///////////////////////////////////////////////////////

  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{}
