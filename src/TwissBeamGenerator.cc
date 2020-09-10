#include "TwissBeamGenerator.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"



/*
  Draw nn samples from a size-dimensional normal distribution
  with a specified mean and covariance
*/

//------------------------------------------------------------------------------
  TwissBeamGenerator::TwissBeamGenerator() 
     : alphaX(1.7), /* non-deminsional */
       alphaY(1.7), /* non-deminsional */
       betaX(33*1e4), /* in um */ 
       betaY(33*1e4), /* in um */ 
       gamma(19750), /* Lorentz factor, (not twiss) */
       nemitX(35), /* Noramlized emittance mm-mrad */ 
       nemitY(35), /* Noramlized emittance mm-mrad */ 
       outFileName("output.txt")
//------------------------------------------------------------------------------
{ 
// Eigen::internal::scalar_normal_dist_op<double>::rng.seed(0); // Seed the rng

}

  TwissBeamGenerator::~TwissBeamGenerator()
{
  Eigen::internal::scalar_normal_dist_op<double>::rng.seed(1); // Seed the rng

}

void TwissBeamGenerator::CalcMeanCovarNTrans(double alpha, double beta, double emit, int dim)
{
  double gamma_twiss = (1 + alpha*alpha) / beta;
  Eigen::Vector2d mean;
  mean  <<  0,  0;
  Eigen::Matrix2d covar;
  covar  <<  emit*beta, emit*alpha,
           emit*alpha,  emit*gamma_twiss;

  Eigen::LLT<Eigen::MatrixXd> cholSolver(covar);
  // We can only use the cholesky decomposition if 
  // the covariance matrix is symmetric, pos-definite.
  // But a covariance matrix might be pos-semi-definite.
  // In that case, we'll go to an EigenSolver
  Eigen::Matrix2d normTransform;
  if (cholSolver.info()==Eigen::Success) {
    // Use cholesky solver
    normTransform = cholSolver.matrixL();
  } else {
    // Use eigen solver
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigenSolver(covar);
    normTransform = eigenSolver.eigenvectors() 
                   * eigenSolver.eigenvalues().cwiseSqrt().asDiagonal();
  }

  if (dim == 0) {
    fMeanX = mean;
    fCovarX = covar;
    fNormTransformX = normTransform;
  }
  else if (dim == 1) {
    fMeanY = mean;
    fCovarY = covar;
    fNormTransformY = normTransform;
  }
}

void TwissBeamGenerator::Trigger() {
  emitX = nemitX / gamma ; // Geometric emittance
  emitY = nemitY / gamma ; // Geometric emittance

  CalcMeanCovarNTrans(alphaX, betaX, emitX,0);
  CalcMeanCovarNTrans(alphaY, betaY, emitY,1);

  std::ofstream ofs(outFileName);
  ofs << "================================" << std::endl;
  ofs << " " << std::endl;
  ofs << "MeanX\n" << fMeanX << std::endl;
  ofs << "CovarX\n" << fCovarX << std::endl;
  ofs << "NormTransformX\n" << fNormTransformX << std::endl;
  ofs << " " << std::endl;
  ofs << "MeanY\n" << fMeanY << std::endl;
  ofs << "CovarY\n" << fCovarY << std::endl;
  ofs << "NormTransformY\n" << fNormTransformY << std::endl;
  ofs << " " << std::endl;
  ofs << "================================" << std::endl;
}


Eigen::MatrixXd TwissBeamGenerator::Sampler(Eigen::Vector2d mean, Eigen::Matrix2d covar, Eigen::Matrix2d normTransform, int nn) 
{
//  G4cout << " random number " << G4endl;
//  G4cout << Eigen::MatrixXd::NullaryExpr(fSize,nn,randN) << G4endl;
  Eigen::MatrixXd samples = (normTransform 
                           * Eigen::MatrixXd::NullaryExpr(fSize,nn,randN)).colwise() 
                           + mean;

  //  Covariance is regarded as positive values. Invert in y direction
  if (covar(2) > 0) 
  { 
    samples = (Eigen::Matrix2d(2,2) << 1,0,0,-1).finished() * samples;
  }

//  std::cout << "Samples\n" << samples << std::endl;
  
//  std::cout << "Writing to output.txt\n" << samples << std::endl;
//  std::ofstream ofs(outFileName);
//  ofs << "Samples\n" << samples << std::endl;
  return samples;
}

void TwissBeamGenerator::Shoot()
{
 samplesX =  Sampler(fMeanX, fCovarX, fNormTransformX, 1);
 samplesY =  Sampler(fMeanY, fCovarY, fNormTransformY, 1);
}

void TwissBeamGenerator::AddCount()
{
  fCount = fCount + 1; // Add internal number in the class object
  G4cout << fCount << G4endl;
}

G4double TwissBeamGenerator::GetX(){
//  G4cout << "samplesX(1,1)" << G4endl;
//  G4cout << samplesX(1,1) << G4endl;
  return (G4double)samplesX(0,0);
}
G4double TwissBeamGenerator::GetPX(){
//  G4cout << "samplesX(2,1)" << G4endl;
//  G4cout << samplesX(2,1) << G4endl;
  return (G4double)samplesX(1,0);
}
G4double TwissBeamGenerator::GetY(){
  return (G4double)samplesY(0,0);
}
G4double TwissBeamGenerator::GetPY(){
  return (G4double)samplesY(1,0);
}

  

