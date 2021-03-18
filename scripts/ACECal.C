/*
ROOT Macro to extract ACE4Sim data and save the 
time, charge, and fit data as pngs
*/

#include <fstream>

{

  // ------------ PARAMETERS ------------
  
  // prevent popup windows
  gROOT->SetBatch(kTRUE);

  // total number of events for these runs
  static constexpr int Ntot{100000};

  // ACECal Parameters
  static constexpr int layers{5};
  static constexpr int nelements{4};
  static constexpr int wgNum{layers*nelements};
  static constexpr auto offset{0.3};
  
  // save path
  auto savePath{""};

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
  
  // create canvas for the temporal profiles
  auto c1{new TCanvas("c1", "ACE Hit Time Profile", 800*nelements, 600*layers)};

  // create wgNum subpads
  c1->Divide(nelements, layers);
  
  // ensure declaration of subpads and draw them
  TPad* c1_[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    c1_[wg] = (TPad*)(c1->GetPad(wg+1));
    c1_[wg]->Draw();
  }

  // turn stats on
  gStyle->SetOptStat();

  // get the min and max x-axis bins for setting range
  auto minX{HTA[1]->FindFirstBinAbove() - 2};
  auto maxX{HTA[wgNum-1]->FindLastBinAbove(10)};
  
  // draw the canvases and then draw the histograms
  for(int wg=0; wg<wgNum; wg++){
    c1_[wg]->cd();
    HTA[wg]->SetFillColor(kBlue-4);
    HTA[wg]->GetYaxis()->SetTitle("Hits");
    HTA[wg]->GetXaxis()->SetTitle("time (ns)");
    HTA[wg]->GetXaxis()->SetRange(minX, maxX);
    HTA[wg]->GetYaxis()->SetRangeUser(0, tMax);
    std::string timeTitle{"Temporal Hit Profile in ACE" + std::to_string(wg+1)};
    HTA[wg]->SetTitle(timeTitle.c_str());
    HTA[wg]->Draw("hist same bar");
  }

  //
  // ----- TOTAL CHARGE -----
  //

  // load H2 objects
  TH2D* TAC[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    std::string TACname{"TotalACE"+std::to_string(wg)+"Charge;1"};
    TAC[wg] = (TH2D*)file->Get(TACname.c_str());
  }

  // create new canvas
  auto c2{new TCanvas("c2", "Total Charge", 800*nelements, 800*layers)};

  // create wgNum pads
  //c2->Divide(nelements,layers);

  // ensure declaration of subpads and draw them
  TPad* c2_[wgNum];

  auto c2Ww{c2->GetWw()};
  auto offset_switch{false}; // offset when true
  auto white{(1.0-offset)/nelements}; // whitespace for non offset
  auto oddEnd{1.0-(offset/nelements)}; // percent of canvas x filled for non offset
  auto layerCount{1}; // keep track of layer
  // c2->SetCanvasSize(c2Ww+white,c2->GetWh()); // resize canvas to restore aspect ratios
  double tX1{0}; double tX2{0}; double tY1{0}; double tY2{0}; 
  for(int wg=0; wg<wgNum; wg++){
    
    // check if offset needed
    if( (wg%nelements) < ((wg-1)%nelements) ){
      offset_switch = not offset_switch;
      layerCount++;
    }

    if(offset_switch){
      tX1 = (oddEnd/nelements)*(wg%nelements) + (offset/nelements);
    } else {
      tX1 = (oddEnd/nelements)*(wg%nelements);
    }
    tX2 = tX1+(oddEnd/nelements);
    tY1 = 1.0-(1.0/layers)*layerCount;
    tY2 = tY1 + (1.0/layers);

    // c2_[wg] = (TPad*)(c2->GetPad(wg+1));
    // c2_[wg]->SetPad(tX1,tY1,tX2,tY2);
    c2_[wg] = new TPad("","",tX1,tY1,tX2,tY2);
    c2_[wg]->Draw();
    
  }

  // turn all statistic settings on
  gStyle->SetOptStat(1111111);

  // y range values
  auto yMin2{-6}; auto yMax2{6};

  // draw the canvases and then draw the histograms
  for(int wg=0; wg<wgNum; wg++){
    
    c2_[wg]->cd();                                   // get correct subpad
    auto xMin2{TAC[wg]->FindFirstBinAbove()};        // set xMin
    auto xMax2{TAC[wg]->FindLastBinAbove()};         // set xMax
    TAC[wg]->GetXaxis()->SetRange(xMin2, xMax2);     // change x range
    TAC[wg]->GetYaxis()->SetRangeUser(yMin2, yMax2); // change y range
    TAC[wg]->GetXaxis()->SetTitle("x (mm)");         // set x axis title
    TAC[wg]->GetYaxis()->SetTitle("y (mm)");         // set y axis title
    std::string totalTitle{"Total Charge in ACE" + std::to_string(wg+1)};
    TAC[wg]->SetTitle(totalTitle.c_str());           // set title
    TAC[wg]->SetStats(0);                            // remove stats box
    c2_[wg]->SetLogz();                              // log scale
    c2_[wg]->SetRightMargin(0.16);                   // set margin
    TAC[wg]->Draw("COLZ");                           // draw the hist
    
  }

  //
  // ----- EXCESS CHARGE -----
  //

  // load H2 objects
  TH2D* EAC[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    std::string EACname{"ExcessACE"+std::to_string(wg)+"Charge;1"};
    EAC[wg] = (TH2D*)file->Get(EACname.c_str());
  }

  // create new canvas
  auto c3{new TCanvas("c3", "Excess Charge", 800*nelements, 800*layers)};

  // create wgNum pads
  c3->Divide(nelements,layers);

  // ensure declaration of subpads and draw them
  TPad* c3_[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    c3_[wg] = (TPad*)(c3->GetPad(wg+1));
    c3_[wg]->Draw();
  }

  // turn all statistic settings on
  gStyle->SetOptStat(1111111);

  // y range values
  auto yMin3{-6}; auto yMax3{6};

  // draw the canvases and then draw the histograms
  for(int wg=0; wg<wgNum; wg++){
    c3_[wg]->cd();                                   // get correct subpad
    auto xMin3{EAC[wg]->FindFirstBinAbove()};        // set xMin
    auto xMax3{EAC[wg]->FindLastBinAbove()};         // set xMax
    EAC[wg]->GetXaxis()->SetRange(xMin3, xMax3);     // change x range
    EAC[wg]->GetYaxis()->SetRangeUser(yMin3, yMax3); // change y range
    EAC[wg]->GetXaxis()->SetTitle("x (mm)");         // set x axis title
    EAC[wg]->GetYaxis()->SetTitle("y (mm)");         // set y axis title
    std::string excessTitle{"Excess Charge in ACE" + std::to_string(wg+1)};
    EAC[wg]->SetTitle(excessTitle.c_str());          // set title
    EAC[wg]->SetStats(0);                            // remove stats box
    EAC[wg]->SetMinimum(EAC[wg]->GetMinimum(0.));    // set minimum
    c3_[wg]->SetLogz();                              // log scale
    c3_[wg]->SetRightMargin(0.16);                   // set margin
    EAC[wg]->Draw("COLZ");                           // draw the hist
  }

  //
  // ----- ENERGY -----
  //

  // load objects (from TTree) - this is annoying
  auto tree{(TTree*)file->Get("Energy;1")};
  TH1F* AE[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    auto ctemp{new TCanvas("ctemp", "temp", 600, 600)};
    ctemp->cd();
    std::string ACE{"ACE"+std::to_string(wg+1)};
    std::string branch{ACE/*+">>"+ACE*/};
    std::string sav{ACE+">>"+ACE};
    std::string selection{branch + "*(" + branch + ">0)" };
    tree->Draw(sav.c_str(), selection.c_str(), "hist");
    AE[wg] = (TH1F*)gDirectory->Get(ACE.c_str());
    delete ctemp;
  }

  // create new canvas
  auto c4{new TCanvas("c4", "Energy", 800*nelements, 600*layers)};

  // create wgNum pads
  c4->Divide(nelements,layers);

  // ensure declaration of subpads and draw them
  TPad* c4_[wgNum];
  for(int wg=0; wg<wgNum; wg++){
    c4_[wg] = (TPad*)(c4->GetPad(wg+1));
    c4_[wg]->Draw();
  }

  // turn all statistic settings on
  gStyle->SetOptStat(1111111);

  // draw the canvases and then draw the histograms
  for(int wg=0; wg<wgNum; wg++){
    c4_[wg]->cd();                                  // set correct subpad
    AE[wg]->GetYaxis()->SetTitle("Counts");         // set y axis title
    AE[wg]->GetXaxis()->SetTitle("Energy (TeV)");  // set x axis title
    std::string energyTitle{"Energy of Particles in ACE" + std::to_string(wg+1)};
    AE[wg]->SetTitle(energyTitle.c_str());          // set title
    c4_[wg]->SetLogy();                             // log scale
    AE[wg]->Draw("hist same bar");                  // draw the hist
  }
  

  //
  // ----- FIT -----
  //
  
  // auto c4{new TCanvas("c4", "fit", 1000, 1500)};

  // gStyle->SetOptStat();
    
  // // Create three pads
  // auto top_pad2{new TPad("top_pad2", "top_pad2",0,0.67,1,1)};
  // //top_pad->SetLogy();
  // top_pad2->Draw();

  // auto mid_pad2{new TPad("mid_pad2", "mid_pad2",0,.33,1.0,0.67)};
  // //mid_pad->SetLogy();
  // mid_pad2->Draw();

  // auto bot_pad2{new TPad("bot_pad2", "bot_pad2",0.0,0.0,1,0.33)};
  // // bot_pad->SetLogy();
  // bot_pad2->Draw();

  // // get excess min and max y bins
  // auto yMinE0{EA0C->FindFirstBinAbove(0,2)};
  // auto yMaxE0{EA0C->FindLastBinAbove(0,2)};
  // auto yMinE1{EA1C->FindFirstBinAbove(0,2)};
  // auto yMaxE1{EA1C->FindLastBinAbove(0,2)};
  // auto yMinE2{EA2C->FindFirstBinAbove(0,2)};
  // auto yMaxE2{EA2C->FindLastBinAbove(0,2)};
    
  // // get global min and max coordinates from min/max bins
  // auto tMin0{EA0C->GetYaxis()->GetBinCenter(yMinE0) - 1};
  // auto tMax0{EA0C->GetYaxis()->GetBinCenter(yMaxE0) + 1};
  // auto tMin1{EA1C->GetYaxis()->GetBinCenter(yMinE1) - 1};
  // auto tMax1{EA1C->GetYaxis()->GetBinCenter(yMaxE1) + 1};
  // auto tMin2{EA2C->GetYaxis()->GetBinCenter(yMinE2) - 1};
  // auto tMax2{EA2C->GetYaxis()->GetBinCenter(yMaxE2) + 1};
    
  // // get y projection of the 2D histograms
  // auto EA0CY{(TH1D*)EA0C->ProjectionY("PEA0C",yMinE0,yMaxE0)};
  // auto EA1CY{(TH1D*)EA1C->ProjectionY("PEA1C",yMinE1,yMaxE1)};
  // auto EA2CY{(TH1D*)EA2C->ProjectionY("PEA2C",yMinE2,yMaxE2)};

  // //Cauchy distribution fit function
  // auto ft{new TF1("fc","[2]/(3.14159*[1]*(1 + ((x-[0])/[1])*((x-[0])/[1])))")};
  // ft->SetParNames("Peak_Coord","HWHM","Amplitude");
    
  // // estimate parameters accurately:
    
  // // get peak coordinate
  // auto maxBin0{EA0CY->GetMaximumBin()};
  // auto peak0{EA0CY->GetBinCenter(maxBin0)};
  // auto maxBin1{EA1CY->GetMaximumBin()};
  // auto peak1{EA1CY->GetBinCenter(maxBin1)};
  // auto maxBin2{EA2CY->GetMaximumBin()};
  // auto peak2{EA2CY->GetBinCenter(maxBin2)};

  // // get HWHM
  // auto lBin0{EA0CY->FindFirstBinAbove(maxBin0/2)};
  // auto rBin0{EA0CY->FindLastBinAbove(maxBin0/2)};
  // auto hwhm0{(EA0CY->GetBinCenter(rBin0) - EA0CY->GetBinCenter(lBin0))/2};
  // auto lBin1{EA1CY->FindFirstBinAbove(maxBin1/2)};
  // auto rBin1{EA1CY->FindLastBinAbove(maxBin1/2)};
  // auto hwhm1{(EA1CY->GetBinCenter(rBin1) - EA1CY->GetBinCenter(lBin1))/2};
  // auto lBin2{EA2CY->FindFirstBinAbove(maxBin2/2)};
  // auto rBin2{EA2CY->FindLastBinAbove(maxBin2/2)};
  // auto hwhm2{(EA2CY->GetBinCenter(rBin2) - EA2CY->GetBinCenter(lBin2))/2};
  
  // // get amplitude
  // auto amp0{EA0CY->GetMaximum()};
  // auto amp1{EA1CY->GetMaximum()};
  // auto amp2{EA2CY->GetMaximum()};
    
  // top_pad2->cd();
  // // EA0CY->Scale(1./(Ntot/120));
  // ft->SetParameters(peak0, hwhm0, amp0);
  // ft->SetRange(-60,60);
  // EA0CY->Fit("fc");
  // EA0CY->Draw("");
  // EA0CY->SetFillColor(kBlue-4);
  // top_pad->SetLeftMargin(0.15);
  // EA0CY->GetYaxis()->SetTitleOffset(1.6); 
  // EA0CY->GetYaxis()->SetTitle("excess charge ACE0");
  // EA0CY->GetXaxis()->SetTitle("distance, mm");

  // mid_pad2->cd();
  // // EA1CY->Scale(1./(Ntot/120));
  // ft->SetParameters(peak1, hwhm1, amp1);
  // ft->SetRange(-60,60);
  // EA1CY->Fit("fc");
  // EA1CY->Draw("");
  // EA1CY->SetFillColor(kBlue-4);
  // mid_pad->SetLeftMargin(0.15); 
  // EA1CY->GetYaxis()->SetTitleOffset(1.6); 
  // EA1CY->GetYaxis()->SetTitle("excess charge ACE1");
  // EA1CY->GetXaxis()->SetTitle("distance, mm");

  // bot_pad2->cd();
  // // EA2CY->Scale(1./(Ntot/120));
  // ft->SetParameters(peak2, hwhm2, amp2);
  // ft->SetRange(-60,60);
  // EA2CY->Fit("fc");
  // EA2CY->Draw("");
  // EA2CY->SetFillColor(kBlue-4);
  // bot_pad->SetLeftMargin(0.15);
  // EA2CY->GetYaxis()->SetTitleOffset(1.6); 
  // EA2CY->GetYaxis()->SetTitle("excess charge  ACE2");
  // EA2CY->GetXaxis()->SetTitle("distance, mm");

  
  // ----- Saving the data as pngs: -----

  // continue processing after drawing
  gSystem->ProcessEvents();

  // change directory to save path
  // gSystem->cd(savePath);

  // construct image files
  auto c1png{TImage::Create()};
  auto c2png{TImage::Create()};
  auto c3png{TImage::Create()};
  auto c4png{TImage::Create()};

  c1png->FromPad(c1);
  c2png->FromPad(c2);
  c3png->FromPad(c3);
  c4png->FromPad(c4);
  
  // and save the png's
  c1png->WriteImage("time.png");
  c2png->WriteImage("totalCharge.png");
  c3png->WriteImage("excessCharge.png");
  c4png->WriteImage("energy.png");

  // continue processing after saving
  gSystem->ProcessEvents();

  // change to directory containing data
  // gSystem->cd("../..");;
  
  // close ROOT
  gApplication->Terminate();
  
}
