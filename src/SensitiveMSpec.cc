#include "SensitiveMSpec.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include "G4VProcess.hh"
#include "G4UnitsTable.hh"
#include "G4TouchableHandle.hh"
#include "G4SystemOfUnits.hh"

SensitiveMSpec::SensitiveMSpec(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

SensitiveMSpec::~SensitiveMSpec()
{}

void SensitiveMSpec::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection
  fHitsCollection = new MSpecHitsCollection(SensitiveDetectorName, collectionName[0]);

  //Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}


//------------------------------------------------------------------------------
G4int SensitiveMSpec::WriteBeamFile(G4String filename, G4ThreeVector position_World,
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




G4bool SensitiveMSpec::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  MSpecHit* newHit = new MSpecHit();

  newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newHit->SetParentID(aStep->GetTrack()->GetParentID());
  newHit->SetEdep(aStep->GetTotalEnergyDeposit());
  newHit->SetPrePos(aStep->GetPreStepPoint()->GetPosition());
  newHit->SetPostPos(aStep->GetPostStepPoint()->GetPosition());
  newHit->SetParticleTime(aStep->GetPreStepPoint()->GetGlobalTime());
  newHit->SetParticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  if (aStep->GetTrack()->GetCreatorProcess()) {
    newHit->SetCreatorProcessName(aStep->GetTrack()->GetCreatorProcess()->GetProcessName());
  }
  newHit->SetKineticEnergy(aStep->GetTrack()->GetKineticEnergy());
  newHit->SetMomentumDirection(aStep->GetPreStepPoint()->GetMomentumDirection());
  newHit->SetCharge(aStep->GetPreStepPoint()->GetCharge());
  newHit->SetCurrentStepNumber(aStep->GetTrack()->GetCurrentStepNumber());

  fHitsCollection->insert(newHit);
//    // Write the position and time into the file for the QuickPIC input
//    if (aStep->GetTrack()->GetTrackID()==1)
//  {
//    if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="e-") WriteBeamFile("beam_electron.txt", 
//  		aStep->GetPreStepPoint()->GetPosition(), 
//  		aStep->GetPreStepPoint()->GetMomentumDirection(), 
//  		aStep->GetTrack()->GetKineticEnergy(),
//  		aStep->GetPreStepPoint()->GetGlobalTime()
//  		);
//    if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="e+") WriteBeamFile("beam_positron.txt", 
//  		aStep->GetPreStepPoint()->GetPosition(), 
//  		aStep->GetPreStepPoint()->GetMomentumDirection(), 
//  		aStep->GetTrack()->GetKineticEnergy(),
//  		aStep->GetPreStepPoint()->GetGlobalTime()
//  		);
//    if (aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="gamma") WriteBeamFile("beam_gamma___.txt", 
//  		aStep->GetPreStepPoint()->GetPosition(), 
//  		aStep->GetPreStepPoint()->GetMomentumDirection(), 
//  		aStep->GetTrack()->GetKineticEnergy(),
//  		aStep->GetPreStepPoint()->GetGlobalTime()
//  		);
//  }
  ///////////////////////////////////////////////////////

  return true;
}

void SensitiveMSpec::EndOfEvent(G4HCofThisEvent*)
{
}
