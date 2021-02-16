#ifndef R3BCALIFAGAINMATCH_H
#define R3BCALIFAGAINMATCH_H

#include "FairTask.h"
#include "R3BCalifaMappingPar.h"
#include "R3BCalifaMappedData.h"
#include "R3BCalifaCrystalCalData.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "TH1F.h"


class TClonesArray;
class R3BCalifaMappingPar;



class R3BCalifaGainMatch : public FairTask
{

public:

    /* Default constructor */
    R3BCalifaGainMatch();

    /* Destructor */
    virtual ~R3BCalifaGainMatch();

    /* Load mapping pars */
    void SetParContainers();

    virtual void SearchPeaks();

    void CreateHistograms();

    void FillHistograms(std::vector<TString> fileList);

    void SetHistoLimits(Int_t l, Int_t r) {

       fRightLimit=r;
       fLeftLimit=l;

    };

    void SetSigma(Float_t sigma) {

        fSigma=sigma;
    }

    void SetNumPeaks(Int_t peaks) {

        fNumPeaks=peaks;
    }


    void SetThreshold(Float_t threshold) {

        fThreshold=threshold;
    }


    void setOutputFile(TFile *f){

      file=f;
    }





 protected:

    R3BCalifaMappingPar *fMap_Pars;

    Int_t fNumCrystals;
    Int_t fRightLimit;
    Int_t fLeftLimit;
    TH1F** fh_Map_energy_crystal;
    TFile *eventFile;
    TTree* eventTree;
    TClonesArray* mappedCA;
    TBranch* mappedBranch;
    Int_t fNumPeaks;
    Float_t fSigma;
    Float_t fThreshold;
    TFile *file;


 public:
    // Class definition
    ClassDef(R3BCalifaGainMatch,2);
};

#endif
