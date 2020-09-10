//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  TwissBeamGenerator.hh
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* 
 * Beam Delivery Simulation (BDSIM) Copyright (C) Royal Holloway, 
 * University of London 2001 - 2019.
 *
 * This file is part of BDSIM.
 *
 * BDSIM is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published 
 * by the Free Software Foundation version 3 of the License.
 *
 * BDSIM is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BDSIM.  If not, see <http://www.gnu.org/licenses/>.
 * */
#ifndef BUNCHTWISS_H
#define BUNCHTWISS_H 

#include <iostream>
#include <fstream>

#include <Eigen/Dense>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include "G4SystemOfUnits.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"



/*
 *   We need a functor that can pretend it's const,
 *     but to be a good random number generator 
 *       it needs mutable state.
 *       */
using namespace std;

namespace Eigen {
  namespace internal {
    template<typename Scalar>
    struct scalar_normal_dist_op
    {
      static boost::mt19937 rng;    // The uniform pseudo-random algorithm
      mutable boost::normal_distribution<Scalar> norm;  // The gaussian combinator
    
      EIGEN_EMPTY_STRUCT_CTOR(scalar_normal_dist_op)
    
      template<typename Index>
      inline const Scalar operator() (Index, Index = 0) const { return norm(rng); }
    };
    
    template<typename Scalar> boost::mt19937 scalar_normal_dist_op<Scalar>::rng;
    
    template<typename Scalar>
    struct functor_traits<scalar_normal_dist_op<Scalar> >
    { enum { Cost = 50 * NumTraits<Scalar>::MulCost, PacketAccess = false, IsRepeatable = false }; };
  } // end namespace internal
} // end namespace Eigen



/**
 * @brief A bunch distribution according to the twiss parameterisation.
 * 
 * @author Stewart Boogert
 */

class TwissBeamGenerator
{
private:
  /// @{ Twiss parameters
  double alphaX;
  double alphaY;
  double betaX;
  double betaY;
  double gamma; // beam energy
  double gammaX;
  double gammaY; 
  double nemitX;
  double nemitY;
  double emitX;
  double emitY;
  double dispX;
  double dispY;
  double dispXP;
  double dispYP;
  const char* outFileName;
  Eigen::MatrixXd samplesX;
  Eigen::MatrixXd samplesY;
  // Define mean and covariance of the distribution
  int fSize = 2;
  Eigen::Vector2d fMeanX; 
  Eigen::Vector2d fMeanY; 
  Eigen::Matrix2d fCovarX;
  Eigen::Matrix2d fCovarY;
  Eigen::Matrix2d fNormTransformX;
  Eigen::Matrix2d fNormTransformY;
  // random value seed
  int fCount = 0;
  Eigen::internal::scalar_normal_dist_op<double> randN; // Gaussian functor



public:
  TwissBeamGenerator();
  ~TwissBeamGenerator();
  Eigen::MatrixXd Sampler(Eigen::Vector2d,Eigen::Matrix2d,Eigen::Matrix2d,int);
  void CalcMeanCovarNTrans(double,double,double,int);
  void Trigger(); // Trigger sampler function
  void AddCount();
  void Shoot();
  G4double GetX();
  G4double GetPX();
  G4double GetY();
  G4double GetPY();


};


#endif
