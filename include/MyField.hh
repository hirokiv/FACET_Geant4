#ifndef MYFIELD_H
#define MYFIELD_H 1

#include "G4MagneticField.hh"
#include "G4SystemOfUnits.hh"

class MyField : public G4MagneticField
{
  public: 
    MyField(G4double,G4double);
    virtual ~MyField();
    void GetFieldValue(const G4double*, G4double*) const override;
  private:
    G4double fTargetThickness;
    G4double fTargetXYSize;
    G4double fAmp;
    G4double fLambda;
};

#endif
