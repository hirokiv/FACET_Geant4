//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ParticleH5FileReader.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef ParticleH5FileReader_h
#define ParticleH5FileReader_h 1

#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "H5Cpp.h"
#include <boost/multi_array.hpp>
// #include <boost>
#include <string>

#define NNMACRO 256 // Specify the maximum number of particles to be injected through reading a file
 
//------------------------------------------------------------------------------
  class ParticleH5FileReader 
//------------------------------------------------------------------------------
{
  public:
    ParticleH5FileReader();    
   ~ParticleH5FileReader();
    void StoreFiles();    
    void AddCount();
//    boost::multi_array<double,2> READ_H5(std::string filename, std::string field, int verbose = 0);
    boost::multi_array<double,2>  READ_H5(G4double *out,  std::string filename, std::string field, int verbose = 0);
    G4double GetX1();
    G4double GetX2();
    G4double GetX3();
    G4double GetP1();
    G4double GetP2();
    G4double GetP3();
//    G4int GetCount() { return (G4int)count };
    G4int GetCount();

  private:
    G4int count = 0;
    G4double density;
    G4double step;
    G4double dx; // Normalized length in cm
    G4double omega_pe; // Normalized time in s^-1
    G4double z_center; // central position in cm
//    boost::multi_array<double,2> *p1;
//    boost::multi_array<double,2> *p2;
//    boost::multi_array<double,2> *p3;
//    boost::multi_array<double,2> *x1;
//    boost::multi_array<double,2> *x2;
//    boost::multi_array<double,2> *x3;
    G4int NN = NNMACRO;  
    G4double p1[NNMACRO] = {};
    G4double p2[NNMACRO] = {};
    G4double p3[NNMACRO] = {};
    G4double x1[NNMACRO] = {};
    G4double x2[NNMACRO] = {};
    G4double x3[NNMACRO] = {};
//    G4GeneralParticleSource* fpParticleGPS;
};

#endif
