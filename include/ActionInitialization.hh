#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H 1

#include "G4VUserActionInitialization.hh"

class DetectorConstruction;

class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization(DetectorConstruction*);
  virtual ~ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  DetectorConstruction* fDetector;
};


#endif // ACTIONINITIALIZATION_H
