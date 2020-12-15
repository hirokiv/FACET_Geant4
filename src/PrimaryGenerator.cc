//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PrimaryGenerator.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "PrimaryGenerator.hh"
//#include "G4ParticleGun.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "CSVReader.hh"
#include "globals.hh"

#include "PrimaryGeneratorMessenger.hh"
//#include "ParticleH5FileReader.hh"
#include "TwissBeamGenerator.hh"

// class ParticleH5FileReaderInstance; 
class TwissBeamGenerator;

//------------------------------------------------------------------------------
  PrimaryGenerator::PrimaryGenerator()
    : fMomentum(100.0*MeV), fSig_r(10.*um), fSig_z(13.*um), fEmitt_n(20.), fParticle("e-"),
      fGENMODE("GAUSS")
//  : fpParticleGPS(0) 
//------------------------------------------------------------------------------
{
//  fpParticleGun = new G4ParticleGun();
 // fpParticleGPS = new G4GeneralParticleSource();
//    // Creating an object of CSVWriter
    CSVReader csvreader = CSVReader("../CSV/spectrum_entire.csv",",");
    // Get the data from CSV File
    dataList = csvreader.getData();
    double ddata;
    // Print the content of row by row on screen
    for(std::vector<std::string> vec : dataList)
    {
        G4int i = 0;
        for(std::string data : vec)
        {
                ddata = ::atof(data.c_str());
                if (i==0) en_list.push_back(ddata);
                if (i==3) CDF.push_back(ddata);
                i++;
        }
    }

// Readout output data from QuickPIC (or OSIRIS)
//     ParticleH5FileReader ph5 = ParticleH5FileReader();
 
   fParticleMessenger = new PrimaryGeneratorMessenger(this);

  if (fGENMODE == "TWISS") {
    fTwiss = TwissBeamGenerator();
    fTwiss.Trigger();
    G4cout << "=============================" << G4endl;
    G4cout << "                             " << G4endl;
    G4cout << "TWISS parameters set" << G4endl;
    G4cout << "                             " << G4endl;
    G4cout << "=============================" << G4endl;
  } 

}

//------------------------------------------------------------------------------
  PrimaryGenerator::~PrimaryGenerator()
//------------------------------------------------------------------------------
{
//  delete fpParticleGun;
//  delete fpParticleGPS;
  delete fParticleMessenger;
}

void boxmuller (G4double& z1, G4double& z2, G4double mu,G4double sigma) {
  // Box Muller method for generating 
    const G4double pi = atan(1.0)*4;
    G4double num1,num2,x1,x2;
    // uniform random value between 0~1
    num1 = (double)rand()/(double)RAND_MAX;
    num2 = (double)rand()/(double)RAND_MAX;
    // Box muller method
    x1 = sqrt(-2.0*log(num1))*cos(2*pi*num2);
    x2 = sqrt(-2.0*log(num1))*sin(2*pi*num2);
    // linear transform
    z1  = mu + sigma*x1;
    z2  = mu + sigma*x2; 
    //  cout << z1 << "," << z2 << endl; 
}

G4double PrimaryGenerator::EnergyDist()
{
//  G4cout << "================EnergyDist==========" << G4endl;
  G4double rnd = (G4double)rand()/(G4double)RAND_MAX;
  while (rnd < CDF[0]) rnd = (G4double)rand()/(G4double)RAND_MAX;
  G4int n = 0;
  while ( rnd > CDF[n] ) n = n + 1;
  G4double weight = (CDF[n] - rnd)/(CDF[n] - CDF[n-1]);
  fMomentum = en_list[n-1]*weight + en_list[n]*(1-weight);
  return fMomentum/1000.; // Turn into GeV
}

//------------------------------------------------------------------------------
  void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
//------------------------------------------------------------------------------
{
  PrimaryGenerator::ElectronGun( anEvent );

  if (fGENMODE == "HDF5") {
    ph5.StoreFiles();
    PrimaryGenerator::ReadH5( anEvent );
  }
}

void PrimaryGenerator::ReadH5(G4Event* anEvent){
  // Particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // Setup for primary particle
  G4String particleName = "e+";
  G4double RM = 0.511 * MeV;

  ph5 = ParticleH5FileReader();

  G4double pos_X =  ph5.GetX1();
  G4double mom_X = (ph5.GetP1() )*RM; 

  G4double pos_Y =  ph5.GetX2();
  G4double mom_Y = (ph5.GetP2() )*RM; 

  G4double pos_Z =  ph5.GetX3();
  G4double mom_Z = (ph5.GetP3() )*RM; 
  ph5.AddCount();

//  G4cout << "Position(X,Y,Z) :  "
//	 << pos_X/cm << "  "
//	 << pos_Y/cm << "  "
//	 << pos_Z/cm << G4endl;
//  G4cout << "Momentum(X,Y,Z) :  "
//         << mom_X << "  "
//         << mom_Y << "  "
//         << mom_Z << G4endl;

  G4ThreeVector momentumVector = G4ThreeVector(mom_X, mom_Y, mom_Z);
//  G4cout << "===============Vector================" << G4endl;
  //Create a primary particle - need to create for every event
  G4PrimaryParticle* primaryParticle = 
    new G4PrimaryParticle( particleTable->FindParticle(particleName), 
        momentumVector.x(),        momentumVector.y(),        momentumVector.z() );

  // Gun Position
  G4ThreeVector vertex = G4ThreeVector(pos_X, pos_Y, pos_Z);
  G4PrimaryVertex* primaryVertex = new G4PrimaryVertex(vertex, 0.0*second);

//  Add the primary particles to the primary vertex
  primaryVertex->SetPrimary( primaryParticle );
  // Add the vertex to the event
  anEvent->AddPrimaryVertex( primaryVertex );
//  G4cout << "==============EofPart==============" << G4endl;
}

void PrimaryGenerator::ElectronGun(G4Event* anEvent){
  // Particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // Setup for primary particle
//  G4String particleName = "e-";
  G4String particleName = fParticle;
//  G4String particleName = "gamma";
//  G4String particleName = "proton";
  G4double r1, r2; // Random number variables
  G4double pos_X, pos_Y; // Position
  G4double mom_X, mom_Y; // Momentum

//  G4double momentum = PrimaryGenerator::EnergyDist()*GeV;
//  G4double momentum = 10000.0*MeV;
  boxmuller (r1, r2, 0, 1);
  G4double momentum = fMomentum + 0.002 * fMomentum * r1; // Assume 0.2 % energy spread

  // Setup beam parameters
//  G4double emitt_n = 20; // Normalized emittance mm-mrad
  G4double emitt_n = fEmitt_n; // Normalized emittance mm-mrad
  G4double emitt_g = emitt_n/(momentum*1000/GeV/0.511); // Geometric emittance mm-mrad
//  G4cout << "Emitt_n  0 " << emitt_n << G4endl;
//  G4cout << "Emitt_g  0 " << emitt_g << G4endl;

  if (strcmp(fGENMODE,"GAUSSIAN") == 0)
  {
    G4double sigma_r = fSig_r; // beam radius
    G4double sigma_rv = emitt_g/(sigma_r/um); // rad
  
    boxmuller (r1, r2, 0, 1);
    pos_X = sigma_r*r1;
    mom_X = sin(sigma_rv*r2); 
    boxmuller (r1, r2, 0, 1);
    pos_Y = sigma_r*r1;
    mom_Y = sin(sigma_rv*r2); 
  } 
  else if (strcmp(fGENMODE,"TWISS") == 0)
  {
    fTwiss.Shoot();
    pos_X = fTwiss.GetX() * um;
    mom_X = fTwiss.GetPX(); 
    pos_Y = fTwiss.GetY() * um;
    mom_Y = fTwiss.GetPY(); 
//    G4cout << "Twiss parameters set " << G4endl;
//    G4cout << pos_X/um << " " ;
//    G4cout << mom_X << " " ;
//    G4cout << pos_Y/um << " " ;
//    G4cout << mom_Y << G4endl;
//    G4cout << " " << G4endl;
  }

  boxmuller (r1, r2, 0, 1);
  G4double sigma_z = fSig_z;
  G4double pos_Z_center = -100*um;
  G4double pos_Z = sigma_z*r1 + pos_Z_center;
  G4double mom_Z = sqrt(1-mom_X*mom_X-mom_Y*mom_Y); 

  G4ThreeVector momentumVector = G4ThreeVector(mom_X, mom_Y, mom_Z)*momentum;
//  G4cout << "===============Vector================" << G4endl;
  //Create a primary particle -need to create for every event
  G4PrimaryParticle* primaryParticle = 
    new G4PrimaryParticle( particleTable->FindParticle(particleName), 
        momentumVector.x(),        momentumVector.y(),        momentumVector.z() );

  // Gun Position - randomization
  G4ThreeVector vertex = G4ThreeVector(pos_X, pos_Y, pos_Z);
  G4PrimaryVertex* primaryVertex = new G4PrimaryVertex(vertex, 0.0*second);

//  fpParticleGPS->setParticlePosition(position);
//  fpParticleGun->GeneratePrimaryVertex(anEvent);

//  Add the primary particles to the primary vertex
  primaryVertex->SetPrimary( primaryParticle );
  // Add the vertex to the event
  anEvent->AddPrimaryVertex( primaryVertex );
//  G4cout << "==============EofPart==============" << G4endl;
}

//  void PrimaryGenerator::SetParticle(G4String particleName)
//  //------------------------------------------------------------------------------
//  {
//    fParticle = particleName;
//  }


