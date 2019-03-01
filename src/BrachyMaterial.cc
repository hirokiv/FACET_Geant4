// 2019-02-20
// Fetched by https://gitlab.cern.ch/geant4/geant4/blob/fbd4999cf7355ec0b49f3aa4c3a443c94f2c0529/examples/advanced/brachytherapy/src/BrachyDetectorConstructionLeipzig.cc
//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// Code developed by:
//  S.Guatelli
//
//    *******************************
//    *                             *
//    *    BrachyMaterial.cc        *
//    *                             *
//    *******************************
//
// $Id: BrachyMaterial.cc,v 1.2 2002/11/18 15:18:38 guatelli Exp $
// GEANT4 tag $Name: geant4-05-01 $
//

#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "globals.hh"
#include "G4MaterialTable.hh"
#include "Randomize.hh"  
#include "G4RunManager.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4SystemOfUnits.hh"
#include "BrachyMaterial.hh"

BrachyMaterial::BrachyMaterial()
{;}

BrachyMaterial::~BrachyMaterial()
{;}
void BrachyMaterial::DefineMaterials()
{
// Define required materials

 G4double A;  // atomic mass
 G4double Z;  // atomic number
 G4double d;  // density
 
   
// // Elements for Tube

 A = 54.94*g/mole;
 G4Element* elMn  = new G4Element("Manganese","Mn",Z=25.,A);
 
 A = 28.09*g/mole;
 G4Element* elSi  = new G4Element("Silicon","Si",Z=14.,A);

 A = 52.00*g/mole;
 G4Element* elCr  = new G4Element("Chromium","Cr",Z=24.,A);

 A = 58.70*g/mole;
 G4Element* elNi  = new G4Element("Nickel","Ni",Z=28.,A);
//
 A = 55.85*g/mole;
 G4Element* elFe  = new G4Element("Iron","Fe",Z=26.,A);

 // Stainless steel (Medical Physics, Vol 25, No 10, Oct 1998)
 d = 8.02*g/cm3 ;
 G4Material* matsteel = new G4Material("StainlessSteel",d,5);
 matsteel->AddElement(elMn, 0.02);
 matsteel->AddElement(elSi, 0.01);
 matsteel->AddElement(elCr, 0.19);
 matsteel->AddElement(elNi, 0.10);
 matsteel->AddElement(elFe, 0.68);

}

G4Material* BrachyMaterial::GetMat(G4String material)
{
 G4cout << "BrachyMaterial::GetMat() called" << G4endl;
 G4Material* pttoMaterial = G4Material::GetMaterial(material); 
 return pttoMaterial; 
}

