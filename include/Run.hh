#ifndef RUN_H
#define RUN_H 1

#include "G4Run.hh"
#include "G4THitsMap.hh"
#include "AnalysisManager.hh"

class G4Event;
//class AnalysisManager;
class DetectorConstruction;


class Run : public G4Run
{
public:
  Run();
  virtual ~Run();

  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);

  G4int GetNumberOfEvents() {return fNumberOfEvents;}

private:
  G4int fHCID1;
  G4int fHCID2;
  G4int fHCID3;
  G4int                 fNumberOfEvents;
  G4RootAnalysisManager* rootAnalysisManager1;
  G4RootAnalysisManager* rootAnalysisManager2;
  G4CsvAnalysisManager*  csvAnalysisManager;
};


#endif /* RUN_H */
