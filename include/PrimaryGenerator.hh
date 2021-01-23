//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PrimaryGenerator.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
//#include "ParticleH5FileReader.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "TwissBeamGenerator.hh"

class G4Event;
class G4ParticleDefinition;
//class ParticleH5FileReader;
class TwissBeamGenerator;
//class F04PrimaryGeneratorMessenger;
class PrimaryGeneratorMessenger;
//class G4ParticleGun;
//class G4GeneralParticleSource;

//------------------------------------------------------------------------------
  class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
//------------------------------------------------------------------------------
{
  public:
    PrimaryGenerator();    
   ~PrimaryGenerator();
    void GeneratePrimaries(G4Event*);
    void ElectronGun(G4Event*);
//    void ReadH5(G4Event*);
    G4double EnergyDist();

  private:
    std::vector<std::vector<std::string> > dataList; 
    std::vector<double> CDF;
    std::vector<double> en_list;
//    ParticleH5FileReader ph5;
    TwissBeamGenerator fTwiss;

    G4double fMomentum;
    G4double fSig_r;
    G4double fSig_z;
    G4double fEmitt_n;
    G4String fParticle;
    G4String fGENMODE;

    PrimaryGeneratorMessenger* fParticleMessenger; // messenger of this class

  //    G4GeneralParticleSource* fpParticleGPS;
  public:
    void SetMomentum(G4double mom)         {fMomentum = mom; };
    void SetEmittance(G4double emitt)      {fEmitt_n = emitt; };
    void SetSigr(G4double sigr)            {fSig_r = sigr; };
    void SetSigz(G4double sigz)            {fSig_z = sigz; };
    void SetParticle(G4String specy)       {fParticle = specy; };
    void SetGENMODE(G4String GENMODE)       {fGENMODE = GENMODE; };
};

#endif
