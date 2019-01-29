#ifndef ANALYSISMANAGER_H
#define ANALYSISMANAGER_H 1

#include "globals.hh"
#include "G4CsvAnalysisManager.hh"
#include "G4RootAnalysisManager.hh"

//#include "g4root.hh"

//#include "g4csv.hh"
//#include "g4xml.hh"


class AnalysisManager
{
public:
  AnalysisManager();
  virtual ~AnalysisManager();
  void Book();
  void PrintStatistic();

private:
  G4bool    fFactoryOn;
  G4String  fFileName;
};



#endif /* ANALYSISMANAGER_H */
