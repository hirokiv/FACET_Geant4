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
  void WriteParticlesInfo(std::string str);
  void ParticlesInfoFileGen(std::string str);
  std::string GetFilename() {return fString;};

  G4int GetNumberOfEvents() {return fNumberOfEvents;};

private:
  G4int fHCID1;
  G4int fHCID2;
  G4int fHCID3;
  G4int fHCID4;
  G4int                 fNumberOfEvents;
  // In-house emittance calculation for the electrons
  G4int emitt_N; // # of emittance calculations
  G4double E_N; //GeV
  G4double E_range; // \pm E_range %

  std::vector<G4double> xx2;          // um
  std::vector<G4double> vx2;        // rad
  std::vector<G4double> xvx; // mm-mrad
  std::vector<G4int> E_count; // Number of electrons used for the calculation
  std::vector<G4double> E_sum; // Number of electrons used for the calculation

  std::string fString; // File name for writing electron information

  G4RootAnalysisManager* rootAnalysisManager1;
  G4RootAnalysisManager* rootAnalysisManager2;
  G4CsvAnalysisManager*  csvAnalysisManager;


};


#endif /* RUN_H */
