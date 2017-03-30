/* Fit2nbbLV.h
 *
 * Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
 * Created: 20/03/2017
 *
 */

#include "Fit2nbbLV.h"

#include <iostream>
#include <math.h>

#include <BAT/BCMath.h>

// ---------------------------------------------------------
Fit2nbbLV::Fit2nbbLV(std::string name) : BCModel(name.c_str()), kUseRange(false) {
   
    // define parameters
    /* [0] */ this->AddParameter("2nbb", 0, 230);
    /* [1] */ this->AddParameter("2nbbLV", 0, 0.003);
    /* [2] */ this->AddParameter("K42homLAr", 0, 0.0003);
    /* [3] */ this->AddParameter("K40onFiberShroud", 0, 1.5);
    /* [4] */ this->AddParameter("Bi212onFiberShroud", 0, 0.05);
    /* [5] */ this->AddParameter("Bi214onFiberShroud", 0, 0.07);
    /* [6] */ this->AddParameter("alphaBEGe", 1000, 2000);
    /* [7] */ this->AddParameter("alphaCOAX", 2000, 3000);
    //
    //// LEGEND
    //
    // [0] 2nbb
    // [1] 2nbbLV
    // [2] K42 in LAr
    // [3] K40 in fibers
    // [4] Pb212 + Tl208 in fibers
    // [5] Pb214 + Bi214 in fibers
    // [6] alphaBEGe
    // [7] alphaCOAX
    //

    // priors
    /*this->SetPriorGauss(0,217,11);
    this->SetPriorConstant(1);
    this->SetPriorConstant(2);    
    this->SetPriorConstant(3);    
    this->SetPriorConstant(4);    
    this->SetPriorConstant(5);*/
    this->SetPriorConstantAll();
}
// ---------------------------------------------------------
void Fit2nbbLV::SetBinning(std::vector<int>& v) {
    ubin = v;
    downBin = 0;
    upBin = v.size()-1;
    return;
}
// ---------------------------------------------------------
void Fit2nbbLV::SetFitRange(double down, double up) {
    
    if (ubin.empty()) {
        std::cerr << "Error: you must call Fit2nbbLV::SetBinning first!\n" << std::flush;
        return;
    }

    int idown = 0; 
    int iup   = 0;
    int ubinsize = ubin.size();
    
    // maybe this needs some pre-testing
    for ( int i = 0; i < ubinsize; ++i ) {
        if ( (double)ubin[i] <= down ) idown++;
        if ( (double)ubin[i] <  up   ) iup++;
    }

    if ( idown != 0 and iup != 0 and iup > idown ) {
        kUseRange = true;
        downBin = idown;
        upBin = iup;
    }

    else std::cout << "Fit2nbbLV::SetFitRange: something went wrong...\n";

    return;
}
// ---------------------------------------------------------
double Fit2nbbLV::LogLikelihood(const std::vector<double> & parameters) {
    
    double logprob = 0.;
    double f;
    //int last = parameters.size()-1;
    
    for ( int i = downBin; i < upBin; ++i ) {

        // BEGe
        f = parameters[0]*simBEGe[0][i] + parameters[0]*parameters[1]*n2n1*simBEGe[1][i]; 
        for ( int j = 2; j <= 3; ++j ) f += parameters[j]*simBEGe[j][i];
        f += parameters[4]*(simBEGe[4][i] + BrTl*simBEGe[5][i]);
        f += parameters[5]*(simBEGe[6][i] +      simBEGe[7][i]);
        f += parameters[6]*simBEGe[8][i];
        logprob += dataBEGe[i]*log(f) - f;// - BCMath::LogFact(dataBEGe[i]);

        // COAX
        f = parameters[0]*simCOAX[0][i] + parameters[0]*parameters[1]*n2n1*simCOAX[1][i]; 
        for ( int j = 2; j <= 3; ++j ) f += parameters[j]*simCOAX[j][i];
        f += parameters[4]*(simCOAX[4][i] + BrTl*simCOAX[5][i]);
        f += parameters[5]*(simCOAX[6][i] +      simCOAX[7][i]);
        f += parameters[7]*simCOAX[8][i];
        logprob += dataCOAX[i]*log(f) - f;// - BCMath::LogFact(dataCOAX[i]);
	}

    return logprob;
}
