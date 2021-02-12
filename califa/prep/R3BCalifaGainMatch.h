#ifndef R3BCALIFAGAINMATCH_H
#define R3BCALIFAGAINMATCH_H

#include "FairTask.h"
#include "R3BCalifaMappingPar.h"
#include "R3BCalifaMappedData.h"
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

    void SetVInterval(Int_t v1,Int_t v2);

    void SetDeltaV(Int_t fDV){
      fDeltaV=fDV;
    };

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    /** Virtual method FinishEvent **/
    virtual void FinishEvent();

    /** Virtual method FinishTask **/
    virtual void FinishTask();

    /** Virtual method Reset **/
    virtual void Reset();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    void SetHistoLimits(Int_t l, Int_t r) {

       fRightLimit=r;
       fLeftLimit=l;

    };




 protected:

    R3BCalifaMappingPar *fMap_Pars;
    TClonesArray* fCalifaMappedDataCA; /**< Array with CALIFA Mapped-input data. >*/

    Int_t fV1;
    Int_t fV2;
    Int_t fDeltaV;
    Int_t fNumCrystals;
    Int_t fNFiles;
    Int_t fRightLimit;
    Int_t fLeftLimit;
    TH1F** fh_Map_energy_crystal;


 public:
    // Class definition
    ClassDef(R3BCalifaGainMatch,1)
};

#endif
