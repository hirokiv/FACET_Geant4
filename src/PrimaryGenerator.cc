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

//------------------------------------------------------------------------------
  PrimaryGenerator::PrimaryGenerator()
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
    //
    // Print the content of row by row on screen
    for(std::vector<std::string> vec : dataList)
    {
        G4int i = 0;
        for(std::string data : vec)
        {
                ddata = ::atof(data.c_str());
                if (i==0) en_list.push_back(ddata);
                if (i==3) CDF.push_back(ddata);
//                G4cout<<data << " , ";
                i++;
        }
//        G4cout<<G4endl;
    }
 
}

//------------------------------------------------------------------------------
  PrimaryGenerator::~PrimaryGenerator()
//------------------------------------------------------------------------------
{
//  delete fpParticleGun;
//  delete fpParticleGPS;
}

void boxmuller (G4double& z1, G4double& z2, G4double mu,G4double sigma) {
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
  G4double momentum = en_list[n-1]*weight + en_list[n]*(1-weight);
  return momentum/1000.; // Turn into GeV
}

//------------------------------------------------------------------------------
  void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
//------------------------------------------------------------------------------
{
  // Particle table
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // Setup for primary particle
  G4String particleName = "e-";
//  G4double momentum = PrimaryGenerator::EnergyDist()*GeV;
  G4double momentum = 10.0*GeV;

  // Setup beam parameters
//  G4double emitt_n = 20; // Normalized emittance mm-mrad
  G4double emitt_n = 20; // Normalized emittance mm-mrad
  G4double emitt_g = emitt_n/(momentum*1000/GeV/0.511); // Geometric emittance mm-mrad
//  G4cout << "Emitt_n  0 " << emitt_n << G4endl;
//  G4cout << "Emitt_g  0 " << emitt_g << G4endl;

  G4double sigma_r = 2*um; // beam radius
  G4double sigma_z = 6.5*um; // beam length
  G4double sigma_rv = emitt_g/(sigma_r/um); // rad

  G4double r1, r2;

  boxmuller (r1, r2, 0, 1);
//  r1 = 0;
//  r2 = 0;
  G4double pos_X = sigma_r*r1;
  G4double mom_X = sin(sigma_rv*r2); 

  boxmuller (r1, r2, 0, 1);
  G4double pos_Y = sigma_r*r1;
  G4double mom_Y = sin(sigma_rv*r2); 

  boxmuller (r1, r2, 0, 1);

  G4double pos_Z_center = -50*um;
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

