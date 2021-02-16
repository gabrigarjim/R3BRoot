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
    ,fMap_Pars(NULL)
    ,fNumCrystals(0)

{
}

R3BCalifaGainMatch::~R3BCalifaGainMatch()
{
    LOG(INFO) << "R3BCalifaGainMatch: Delete instance";
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







void  R3BCalifaGainMatch::CreateHistograms()
{
    LOG(INFO) << "R3BCalifaGainMatch::Init";

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
          LOG(INFO) << "R3BCalifaGainMatch: No Rtdb!";
    }

    char name[100];
    fh_Map_energy_crystal = new TH1F*[fNumCrystals];

    for (Int_t i = 0; i < fNumCrystals; i++){
      //  if (fMap_Pars->GetInUse(i + 1) == 1)
      //    {


            sprintf(name, "fh_Map_energy_crystal_%i", i + 1);
            fh_Map_energy_crystal[i] = new TH1F(name, name, 120, fLeftLimit, fRightLimit);

      //  }
      }


}


void R3BCalifaGainMatch::FillHistograms(std::vector<TString> fileList)
{
    Int_t nHits;
    Float_t rawEnergy;
    Int_t crystalId;

  for(Int_t k =0;k<fileList.size();k++) {

    eventFile = TFile::Open(fileList.at(k));
    eventTree = (TTree*)eventFile->Get("evt");
    eventTree->SetBranchStatus("*",0);
    eventTree->SetBranchStatus("CalifaCrystalCalData*",1);


    mappedCA = new TClonesArray("R3BCalifaCrystalCalData",5);
    mappedBranch = eventTree->GetBranch("CalifaCrystalCalData");
    mappedBranch->SetAddress(&mappedCA);

    Int_t nEvents = eventTree->GetEntries();

    for (Int_t j = 0; j<nEvents; j++) {

      mappedCA->Clear();
      eventTree->GetEvent(j);
      nHits = mappedCA->GetEntries();

      if(nHits)
      for(Int_t i =0; i<nHits;i++) {

            rawEnergy=1E6*((R3BCalifaCrystalCalData*)mappedCA->At(i))->GetEnergy();
            crystalId=((R3BCalifaCrystalCalData*)mappedCA->At(i))->GetCrystalId();
            fh_Map_energy_crystal[crystalId - 1]->Fill(rawEnergy);
      }
    }
  }


}


void R3BCalifaGainMatch::SearchPeaks(){

  TSpectrum* ss = new TSpectrum(fNumPeaks);

  for (Int_t i = 0; i < fNumCrystals; i++)
      if (fMap_Pars->GetInUse(i + 1) == 1)
       {

       ss->Search(fh_Map_energy_crystal[i], fSigma, "", fThreshold);
       fh_Map_energy_crystal[i]->Write();

       }

       file->Write();

}



ClassImp(R3BCalifaGainMatch)
