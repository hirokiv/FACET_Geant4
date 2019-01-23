#ifndef RUNACTION_H
#define RUNACTION_H 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "AnalysisManager.hh"

class G4Run;
class DetectorConstruction;

class RunAction : public G4UserRunAction
{
public:
  RunAction(DetectorConstruction*);
  virtual ~RunAction();

  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run* run);
  virtual G4Run* GenerateRun();
private:
  DetectorConstruction* fDetector;
  AnalysisManager*      fAnalysisManager;
};


#endif /* RUNACTION_H */
