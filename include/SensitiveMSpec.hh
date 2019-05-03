#ifndef SENSITIVEMSPEC_H
#define SENSITIVEMSPEC_H 1

#include "G4VSensitiveDetector.hh"
#include "MSpecHit.hh"

class G4Step;
class G4HCofThisEvent;

class SensitiveMSpec : public G4VSensitiveDetector
{
public:
  SensitiveMSpec(const G4String& name, const G4String& hitsCollectionName);
  virtual ~SensitiveMSpec();

  //Methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
  virtual G4int  WriteBeamFile(G4String filename, G4ThreeVector position_World,
   G4ThreeVector momentum, G4double kin_ene, G4double time);


private:
  MSpecHitsCollection* fHitsCollection;
};


#endif /* SENSITIVETARGET_H */
