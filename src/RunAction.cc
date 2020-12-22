#include "RunAction.hh"
#include "Run.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction(DetectorConstruction* det)
  : G4UserRunAction(), fDetector(det)
{
  G4cout << "RunAction constructor" << G4endl;
  //Set printing event number per each 1000 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
  fAnalysisManager = new AnalysisManager();
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  //Open an output file
  G4RootAnalysisManager* rootAnalysisManager = G4RootAnalysisManager::Instance();
  if ( rootAnalysisManager->IsActive() ) {
    rootAnalysisManager->OpenFile();
  }

  G4CsvAnalysisManager* csvAnalysisManager = G4CsvAnalysisManager::Instance();
  csvAnalysisManager->OpenFile();

}

void RunAction::EndOfRunAction(const G4Run* run)
{
  Run* aRun = (Run*)run;
  G4RootAnalysisManager* rootAnalysisManager = G4RootAnalysisManager::Instance();
  // save histograms
  if ( rootAnalysisManager->IsActive() ) {
    rootAnalysisManager->Write();
    rootAnalysisManager->CloseFile();
  }

  //save Ntuples
  G4CsvAnalysisManager* csvAnalysisManager = G4CsvAnalysisManager::Instance();
  if (csvAnalysisManager->IsActive())
    {
      csvAnalysisManager->Write();
      csvAnalysisManager->CloseFile();
    }
  if (IsMaster()) {
      //fAnalysisManager->PrintStatistic();

      G4cout << "\n==================================\n" << G4endl;
      G4cout << "\nTotal number of events: " << aRun->GetNumberOfEvents() << G4endl;
  }
  csvAnalysisManager->Write();
  csvAnalysisManager->CloseFile();
 
  fAnalysisManager->~AnalysisManager();
}


G4Run* RunAction::GenerateRun()
{
  return (new Run());
}
