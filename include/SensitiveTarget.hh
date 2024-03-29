#ifndef SENSITIVETARGET_H
#define SENSITIVETARGET_H 1


#include "G4VSensitiveDetector.hh"
#include "TargetHit.hh"

class G4Step;
class G4HCofThisEvent;

class SensitiveTarget : public G4VSensitiveDetector
{
public:
  SensitiveTarget(const G4String& name, const G4String& hitsCollectionName);
  virtual ~SensitiveTarget();

  //Methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

private:
  TargetHitsCollection* fHitsCollection;
};


#endif /* SENSITIVETARGET_H */
