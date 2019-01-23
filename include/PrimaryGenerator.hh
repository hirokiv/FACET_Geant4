//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PrimaryGenerator.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
class G4Event;
class G4ParticleDefinition;
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
    G4double EnergyDist();

  private:
    std::vector<std::vector<std::string> > dataList; 
    std::vector<double> CDF;
    std::vector<double> en_list;

//    G4GeneralParticleSource* fpParticleGPS;
};

#endif
