#include "MyField.hh"
#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"
#include <math.h>

// Generate sinusoidal + arbitral amplitude magnetic field

MyField::MyField(G4double amp, G4double lambda)
{
  fAmp = amp;
  fLambda = lambda;
//  fTargetThickness = Thickness; 
//  fTargetXYSize = XYSize;
}

MyField::~MyField()
{}

void MyField::GetFieldValue( const G4double point[4], G4double *field) const
{
    // point[0..2] position
    // point[3] time
    // unit of point values should be in um
    // Artificial example
    // lambda' unit in um recommended
    // amp's unit in T recommended
    G4double k = (2. * CLHEP::pi) / fLambda; // means peaks every 4 um
//    G4double amp = 1. * tesla; // = 1 mega Gauss

//    if (
//        ( (point[0] < fTargetXYSize/2   ) && (point[0] > -fTargetXYSize/2 )) &&
//        ( (point[1] < fTargetXYSize/2   ) && (point[1] > -fTargetXYSize/2 )) &&
//        ( (point[2] < fTargetThickness)   && (point[2] > 0                ))
//       )
//    {
	G4double AxialGrowth;
        G4double z0 = 0.5*mm; // Center of teh erf function
        G4double zsat = 0.75*mm; //Saturate at 0.75
        if (point[0] < z0) 
          AxialGrowth = 1 + erf((point[2]-z0)/(zsat-z0)*2)/2;
        else 
	  AxialGrowth = 1;
        
        field[0] = AxialGrowth * fAmp * cos( k * point[1]); //amp * sin( k * point[0]);
        field[1] = 0;//fAmp * sin( k * point[0]);
        field[2] = 0; 

//    }else{ 
//        field[0] = 0;
//        field[0] = 0;
//        field[0] = 0;
//    }
    
//    G4cout << "=================" << G4endl;
//    G4cout << "k        : " << k               << G4endl;
//    G4cout << "point[0] : " << point[0] /mm    << G4endl;
//    G4cout << "field[0] : " << field[0] /tesla << G4endl;
//    G4cout << "point[1] : " << point[1] /mm    << G4endl;
//    G4cout << "field[0] : " << field[1] /tesla << G4endl;
}
