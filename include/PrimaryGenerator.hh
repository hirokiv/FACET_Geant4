//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PrimaryGenerator.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "ParticleH5FileReader.hh"
class G4Event;
class G4ParticleDefinition;
class ParticleH5FileReader;
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
    void ReadH5(G4Event*);
    G4double EnergyDist();

  private:
    std::vector<std::vector<std::string> > dataList; 
    std::vector<double> CDF;
    std::vector<double> en_list;
    ParticleH5FileReader ph5;

//    G4GeneralParticleSource* fpParticleGPS;
};

#endif
