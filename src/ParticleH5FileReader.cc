#include "ParticleH5FileReader.hh"
#include "G4SystemOfUnits.hh"


ParticleH5FileReader::ParticleH5FileReader()    
{
  G4int verbose = 1;
  std::string filename = "/DataStorage/QUICKPIC_OUTPUT/G07/RAW-BEAM-04_1200.h5";
  density = 4.5*1e16; // x 10^16 [cm-3] 
  step = 12000; // steps On QuickPIC. should be the same as the file number x dT

  dx = 5.31e5 * std::pow(density, -0.5) * cm; // Normalized length
  omega_pe = 5.64e4 * std::pow(density, 0.5); //  plasma freq. [/s]
  z_center = step / omega_pe * 3.0e10 * cm; // center position after steps [cm]
  G4cout << " center of the beam : " << z_center/cm  << G4endl;
  G4cout << " Normalized length : " << dx/cm  << G4endl;

//  boost::multi_array<double,2>  x1_t = ParticleH5FileReader::READ_H5(filename, "x1", verbose ); // Length in normalized unit
//  boost::multi_array<double,2>  x2_t = ParticleH5FileReader::READ_H5(filename, "x2", verbose );
//  boost::multi_array<double,2>  x3_t = ParticleH5FileReader::READ_H5(filename, "x3", verbose ); // Actually shows zeta = ct-z
//  boost::multi_array<double,2>  p1_t = ParticleH5FileReader::READ_H5(filename, "p1", verbose );
//  boost::multi_array<double,2>  p2_t = ParticleH5FileReader::READ_H5(filename, "p2", verbose );
//  boost::multi_array<double,2>  p3_t = ParticleH5FileReader::READ_H5(filename, "p3", verbose );
//  G4int NN = 300000; // x1_t.size();
//    for (int i = 0; i<NN; ++i){
//      p1[i] = (G4double)p1_t[i][0];
//      p2[i] = (G4double)p2_t[i][0];
//      p3[i] = (G4double)p3_t[i][0];
//      x1[i] = (G4double)x1_t[i][0];
//      x2[i] = (G4double)x2_t[i][0];
//      x3[i] = (G4double)x3_t[i][0];
//    }
    ParticleH5FileReader::READ_H5(x1, filename, "x1", verbose ); // Length in normalized unit
    ParticleH5FileReader::READ_H5(x2, filename, "x2", verbose );
    ParticleH5FileReader::READ_H5(x3, filename, "x3", verbose ); // Actually shows zeta = ct-z
    ParticleH5FileReader::READ_H5(p1, filename, "p1", verbose );
    ParticleH5FileReader::READ_H5(p2, filename, "p2", verbose );
    ParticleH5FileReader::READ_H5(p3, filename, "p3", verbose );

}

ParticleH5FileReader::~ParticleH5FileReader()
{
}

void ParticleH5FileReader::AddCount()
{
  count = count + 1; // Add internal number in the class object
} 

//boost::multi_array<double,2> ParticleH5FileReader::READ_H5(std::string filename, std::string field, int verbose)

boost::multi_array<double,2> ParticleH5FileReader::READ_H5(G4double *out, std::string filename, std::string field, int verbose)
{
        H5::H5File file(filename, H5F_ACC_RDONLY);
        H5::DataSet dataset = file.openDataSet(field);

        H5::DataSpace dataspace = dataset.getSpace();

        int rank = dataspace.getSimpleExtentNdims();

        hsize_t dims_out[2];
        int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
        int NX = (unsigned long)(dims_out[0]);
        int NY = (unsigned long) 1;//(dims_out[1]);
        if (verbose == 1) {
                G4cout << "rank  " << rank << ", dimensions " << NX << " x " << NY << G4endl;
                G4cout << "ndims " << ndims << G4endl;
	}

        boost::multi_array<double,2> data(boost::extents[NX][1]);
        dataset.read(data.data(),H5::PredType::NATIVE_DOUBLE);

        if (verbose == 1) {
            int NXidx = 5;
            int NYidx = 1;
            for(int i=0;i<NXidx;i++){
                for(int j=0;j<NYidx;j++) 
                    G4cout << data[i][j] << " ";
                G4cout << G4endl;
        	}
        }

        for (int i = 0; i<NN; ++i){
          out[i] = (G4double)data[i][0];
        }

        return data;
}


G4double ParticleH5FileReader::GetX1() 
{
// G4cout << "############ DEbug info ##############" << G4endl;
// G4cout << " Count : " << count << G4endl;
// G4cout << " X1    : " << x1[count] << G4endl;
// G4cout << " dx    : " << dx << G4endl;
 return x1[count] * dx;  // Return in cm
}
G4double ParticleH5FileReader::GetX2() 
{
 return x2[count] * dx; 
}
G4double ParticleH5FileReader::GetX3() 
{ 
 return x3[count] * dx + z_center;
}
G4double ParticleH5FileReader::GetP1() 
{
 return p1[count]; 
}
G4double ParticleH5FileReader::GetP2()
{
 return p2[count]; 
}
G4double ParticleH5FileReader::GetP3() 
{
 return p3[count]; 
}
G4int ParticleH5FileReader::GetCount() 
{
 return (G4int)count; 
}
