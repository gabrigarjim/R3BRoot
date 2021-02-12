#include "R3BCalifaGainMatch.h"

#include "TClonesArray.h"
#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVector3.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TGeoManager.h"

#include "R3BCalifaMappedData.h"
#include "R3BCalifaMappingPar.h"
#include "R3BEventHeader.h"

#include <iostream>
#include <stdlib.h>



R3BCalifaGainMatch::R3BCalifaGainMatch()
    : FairTask("R3B CALIFA Gain Matcher",1)
    ,fCalifaMappedDataCA(NULL)
    ,fV1(0)
    ,fV2(0)
    ,fMap_Pars(NULL)
    ,fDeltaV(0)
    ,fNumCrystals(0)

{
}

R3BCalifaGainMatch::~R3BCalifaGainMatch()
{
    LOG(INFO) << "R3BCalifaGainMatch: Delete instance";
    if (fCalifaMappedDataCA)
        delete fCalifaMappedDataCA;
}

void R3BCalifaGainMatch::SetParContainers() {

  FairRuntimeDb* rtdb = FairRuntimeDb::instance();
  if (!rtdb)
  {
      LOG(ERROR) << "R3BCalifaGainMatch:: FairRuntimeDb not opened!";
  }

  fMap_Pars = (R3BCalifaMappingPar*)rtdb->getContainer("califaMappingPar");

  if (!fMap_Pars)
  {
      LOG(ERROR) << "R3BCalifaGainMatch::Init() Couldn't get handle on califaMappingPar container";
  }

  if (fMap_Pars)
   {
      fNumCrystals=fMap_Pars->GetNumCrystals();
      LOG(INFO) << "R3BCalifaGainMatch:: califaMappingPar container opened";
      LOG(INFO) << "Number of Crystals : "<<fNumCrystals;
   }

}


void R3BCalifaGainMatch::SetVInterval(Int_t v1, Int_t v2){

   fV1=v1;
   fV2=v2;

   if(v1>=v2)
   LOG(ERROR)<<"V1 must be smaller than V2 ! ";

   else {

     fNFiles = (fV2-fV1)/(fDeltaV) +1;
     LOG(INFO)<<"Expected "<<fNFiles<<" files for voltages between V1= "<<fV1<<" and V2= "<<fV2;
  }
}




InitStatus R3BCalifaGainMatch::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}



InitStatus R3BCalifaGainMatch::Init()
{
    LOG(INFO) << "R3BCalifaGainMatch::Init";

    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fCalifaMappedDataCA = (TClonesArray*)rootManager->GetObject("CalifaMappedData");
    if (!fCalifaMappedDataCA)
    {
        return kFATAL;
    }

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    char name[100];
    fh_Map_energy_crystal = new TH1F*[fNumCrystals];

    for (Int_t i = 0; i < fNumCrystals; i++)
        if (fMap_Pars->GetInUse(i + 1) == 1)
        {

            sprintf(name, "fh_Map_energy_crystal_%i", i + 1);
            fh_Map_energy_crystal[i] = new TH1F(name, name, 120, fLeftLimit, fRightLimit);

          }

    return kSUCCESS;
}


void R3BCalifaGainMatch::Exec(Option_t* opt)
{
    Int_t nHits = fCalifaMappedDataCA->GetEntries();
    if (!nHits)
        return;

    Int_t crystalId = 0;
    Float_t energy=0.0;

    for (Int_t i = 0; i < nHits; i++)
    {
        crystalId = ((R3BCalifaMappedData*)fCalifaMappedDataCA->At(i))->GetCrystalId();
        energy = ((R3BCalifaMappedData*)fCalifaMappedDataCA->At(i))->GetEnergy();

        fh_Map_energy_crystal[crystalId - 1]->Fill(energy);
    }


    return;
}



void R3BCalifaGainMatch::Reset() {}

void R3BCalifaGainMatch::FinishEvent() {}


ClassImp(R3BCalifaGainMatch)
