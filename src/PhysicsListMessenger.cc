//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm12/src/PhysicsListMessenger.cc
/// \brief Implementation of the PhysicsListMessenger class
//
// $Id: PhysicsListMessenger.cc 82461 2014-06-23 10:44:06Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
:G4UImessenger(),fPhysicsList(pPhys),
 fPhysDir(0), fListCmd(0), fGammaToMuPairFacCmd(0), fAnnihiToMuPairFacCmd(0), fAnnihiToHadronFacCmd(0)
{
  fPhysDir = new G4UIdirectory("/testem/phys/");
  fPhysDir->SetGuidance("Control the physics lists");

  fGammaToMuPairFacCmd=new G4UIcmdWithADouble
                                      ("/testem/phys/SetGammaToMuPairFac",this);
  fGammaToMuPairFacCmd->SetGuidance(
         "Set factor to artificially increase the GammaToMuPair cross section");
  fGammaToMuPairFacCmd->SetParameterName("GammaToMuPairFac",false);
  fGammaToMuPairFacCmd->SetRange("GammaToMuPairFac>0.0");
  fGammaToMuPairFacCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fAnnihiToMuPairFacCmd=new G4UIcmdWithADouble
                                     ("/testem/phys/SetAnnihiToMuPairFac",this);
  fAnnihiToMuPairFacCmd->SetGuidance(
        "Set factor to artificially increase the AnnihiToMuPair cross section");
  fAnnihiToMuPairFacCmd->SetParameterName("AnnihiToMuPairFac",false);
  fAnnihiToMuPairFacCmd->SetRange("AnnihiToMuPairFac>0.0");
  fAnnihiToMuPairFacCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fAnnihiToHadronFacCmd=
    new G4UIcmdWithADouble("/testem/phys/SetAnnihiToHadronFac",this);
  fAnnihiToHadronFacCmd->SetGuidance(
       "Set factor to artificially increase the AnnihiToHadrons cross section");
  fAnnihiToHadronFacCmd->SetParameterName("AnnihiToHadFac",false);
  fAnnihiToHadronFacCmd->SetRange("AnnihiToHadFac>0.0");
  fAnnihiToHadronFacCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  
  fListCmd = new G4UIcmdWithAString("/testem/phys/addPhysics",this);  
  fListCmd->SetGuidance("Add modula physics list.");
  fListCmd->SetParameterName("PList",false);
  fListCmd->AvailableForStates(G4State_PreInit);
  fListCmd->SetToBeBroadcasted(false);        

  fStepMaxCMD = new G4UIcmdWithADoubleAndUnit("/testem/phys/stepMax",this);
  fStepMaxCMD->SetGuidance("Set max. step length in the detector");
  fStepMaxCMD->SetParameterName("mxStep",false);
  fStepMaxCMD->SetUnitCategory("Length");
  fStepMaxCMD->SetRange("mxStep>0.0");
  fStepMaxCMD->SetDefaultUnit("mm");
  fStepMaxCMD->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//.s...oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
  delete fGammaToMuPairFacCmd;
  delete fAnnihiToMuPairFacCmd;
  delete fAnnihiToHadronFacCmd;

  delete fListCmd;
  delete fPhysDir;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{       
  if( command == fListCmd )
   { fPhysicsList->AddPhysicsList(newValue);}

  if( command == fStepMaxCMD ) 
  {
    fPhysicsList->SetStepMax( fStepMaxCMD->GetNewDoubleValue(newValue));
  }

  if(command == fGammaToMuPairFacCmd)
   { fPhysicsList->SetGammaToMuPairFac(
          fGammaToMuPairFacCmd->GetNewDoubleValue(newValue));}

  if( command == fAnnihiToMuPairFacCmd)
   { fPhysicsList->SetAnnihiToMuPairFac(
                          fAnnihiToMuPairFacCmd->GetNewDoubleValue(newValue));}

  if( command == fAnnihiToHadronFacCmd)
   { fPhysicsList->SetAnnihiToHadronFac(
                          fAnnihiToHadronFacCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
