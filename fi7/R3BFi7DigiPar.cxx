#include <iostream>
#include "R3BFi7DigiPar.h"
#include "TMath.h"

R3BFi7DigiPar::R3BFi7DigiPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name,title,context)
{
  clear();
}

void R3BFi7DigiPar::putParams(FairParamList* list)
{
  std::cout<<"-I- R3BFi7DigiPar::putParams() called"<<std::endl;
  if(!list) return;
//  list->add("max_paddle", (Int_t)nMaxPaddle);
//  list->add("max_plane", (Int_t)nMaxPlane);
}

Bool_t R3BFi7DigiPar::getParams(FairParamList* list)
{
  std::cout<<"-I- R3BFi7DigiPar::getParams() called"<<std::endl;
  if (!list) return kFALSE;
  std::cout<<"-I- R3BFi7DigiPar::getParams() 1 ";
  
//  if (!list->fill("max_paddle", &nMaxPaddle)) return kFALSE;
//  if (!list->fill("max_plane", &nMaxPlane)) return kFALSE;
  return kTRUE;
}


void R3BFi7DigiPar::Print(Option_t *option) const
{
    std::cout<<"-I- Fi7 Digi Parameters:"<<std::endl;
//    std::cout<<"   Max Paddle   = "<<nMaxPaddle<<std::endl;
//    std::cout<<"   Max Plane   = "<<nMaxPlane<<std::endl;
}

ClassImp(R3BFi7DigiPar);
