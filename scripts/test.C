/*
ROOT Macro to extract ACE4Sim data and save the 
time, charge, and fit data as pngs
*/

#include <fstream>

{

  // ------------ PARAMETERS ------------
  
  // prevent popup windows
  //  gROOT->SetBatch(kTRUE);

  // total number of events for these runs
  // static constexpr int Ntot{100000};

  // ACECal Parameters
  static constexpr int layers{5};
  static constexpr int nelements{4};
  static constexpr int wgNum{layers*nelements};
  static constexpr auto offset{0.5};
  
  // save path
  // auto savePath{""};

  // file name
  auto fileName{"ACE4Sim.root"};
  
  // -----------------------------------------

  
  // open ROOT data file
  auto file{TFile::Open(fileName)};

 //
  // ----- TIME -----
  //

  // load H1 objects
  TH1D* HTA[wgNum];
  int tmax[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    std::string HTAname{"HitTimesACE"+std::to_string(wg)+";1"};
    HTA[wg] = (TH1D*)file->Get(HTAname.c_str());
    tmax[wg] = HTA[wg]->GetMaximum();
  }
  int tMax{*std::max_element(tmax, tmax + wgNum)};
  
}
