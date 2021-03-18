/*
ROOT Macro to save ACE4 Hit Time Histograms for 
rapidity studies into ~/ACE4Sim/data/rapidity as pngs
*/

#include <fstream>

{
  
  // prevent popup windows
  gROOT->SetBatch(kTRUE);

  // total number of events for these runs
  auto Ntot{10000};
  
  // save path
  auto savePath{"data/rapidity"};

  // text file for fit information
  ofstream output;
  output.open("data/rapidity/rapidity.txt");
  output << "# Rapidity Cauchy Fit Data for ACE4Sim \n" << 
    "# particle=e-; num_beam_particles=1; energy=14.6GeV \n#\n" <<
    "# Cauchy Distribution: \n" <<
    "# f(x) = [Amp] / ( pi*[HWHM]*( (x-[Peak]) / [HWHM] )^2 ) \n" <<
    "# where [Peak] is the x coordinate of the peak,\n" << 
    "# [HWHM] is the half width at half maximum,\n" <<
    "# and [Amp] is the scaled amplitude. \n#\n" <<
    "# -------------------------------------------------- \n\n"
    "# Rapidity  ACE  [Peak]mm  [HWHM]mm  [Amp]" << endl;

  for (int i=5; i<100; i+=10) {

    // get the ith rapidity root file
    if (i>90) i = 90;
    auto number{std::to_string(i)};
    auto fileName{"ACE4Sim_Data_rap" + number + ".root"};
  
    // open ROOT data file
    auto file{TFile::Open(fileName.c_str())};

    // create canvas for each ACE element
    auto c{new TCanvas(number.c_str(), "ACE Hit Time Profile", 800, 800)};

    // set title
    auto titleText{"Temporal Hit Profile for Rapidity " + number + " degrees"};

    // turn stats on
    gStyle->SetOptStat();
  
    // load H1 objects
    auto HTA0{(TH1D*)file->Get("HitTimesACE0;1")};  
    auto HTA1{(TH1D*)file->Get("HitTimesACE1;1")};
    auto HTA2{(TH1D*)file->Get("HitTimesACE2;1")};

    // create three pads
    auto top_pad{new TPad("top_pad", "top_pad",0,0.67,1,1)};
    top_pad->Draw();

    auto mid_pad{new TPad("mid_pad", "mid_pad",0,.33,1.0,0.67)};
    mid_pad->Draw();

    auto bot_pad{new TPad("bot_pad", "bot_pad",0.0,0.0,1,0.33)};
    bot_pad->Draw();

    // get the min and max x-axis bins for setting range
    auto minX{HTA0->FindFirstBinAbove() - 2};
    auto maxX{HTA2->FindLastBinAbove(100)};
  
    // draw the canvases and then draw the histograms
    top_pad->cd();
    HTA0->Draw("hist same bar");
    HTA0->SetFillColor(kBlue-4);
    HTA0->GetYaxis()->SetTitle("Hits");
    HTA0->GetXaxis()->SetTitle("time (ns)");
    HTA0->GetXaxis()->SetRange(minX, maxX);
    HTA0->SetTitle(titleText.c_str());
    mid_pad->cd();
    HTA1->Draw("hist same bar");
    HTA1->SetFillColor(kBlue-4);
    HTA1->GetYaxis()->SetTitle("Hits");
    HTA1->GetXaxis()->SetTitle("time (ns)");
    HTA1->GetXaxis()->SetRange(minX, maxX);
    HTA1->SetTitle("");
    bot_pad->cd();
    HTA2->Draw("hist same bar");
    HTA2->SetFillColor(kBlue-4);
    HTA2->GetYaxis()->SetTitle("Hits");
    HTA2->GetXaxis()->SetTitle("time (ns)");
    HTA2->GetXaxis()->SetRange(minX, maxX);
    HTA2->SetTitle("");

    // Charge Histograms
    auto c1{new TCanvas("c1", "Excess Charge", 2000, 1200)};

    // turn all statistic settings on
    gStyle->SetOptStat(1111111);

    // create six pads
    c1->Divide(3,2);

    // ensures declaration of subpads
    auto c1_1{(TPad*)(c1->GetPad(1))};
    auto c1_2{(TPad*)(c1->GetPad(2))};
    auto c1_3{(TPad*)(c1->GetPad(3))};
    auto c1_4{(TPad*)(c1->GetPad(4))};
    auto c1_5{(TPad*)(c1->GetPad(5))};
    auto c1_6{(TPad*)(c1->GetPad(6))};

    // draw pads
    c1_1->Draw(); c1_2->Draw(); c1_3->Draw();
    c1_4->Draw(); c1_5->Draw(); c1_6->Draw();

    // load each charge hist
    auto TA0C{(TH2D*)file->Get("TotalACE0Charge;1")};
    auto EA0C{(TH2D*)file->Get("ExcessACE0Charge;1")};
    auto TA1C{(TH2D*)file->Get("TotalACE1Charge;1")};
    auto EA1C{(TH2D*)file->Get("ExcessACE1Charge;1")};
    auto TA2C{(TH2D*)file->Get("TotalACE2Charge;1")};
    auto EA2C{(TH2D*)file->Get("ExcessACE2Charge;1")};

    // bin threshold for setting range
    int threshold;
    if (i == 5) {
      threshold = 0;
    }else if (i < 45) {
      threshold = 100;
    }else if (i >= 45) {
      threshold = 2000;
    }else threshold = 1000;

    // x range values
    auto xMin{-6};
    auto xMax{6};

    // here are commands to change the properties of the plots
    auto yMin1{TA0C->FindFirstBinAbove(threshold, 2)};  // first y bin above threshold
    auto yMax1{TA0C->FindLastBinAbove(threshold, 2)};   // last y bin above threshold
    TA0C->GetXaxis()->SetRangeUser(xMin, xMax);           // change x range
    TA0C->GetYaxis()->SetRange(yMin1, yMax1);           // change y range
    TA0C->GetXaxis()->SetTitle("x (mm)");               // set x axis title
    TA0C->GetYaxis()->SetTitle("y (mm)");               // set y axis title
    TA0C->SetStats(0);                                  // remove stats box
  
    EA0C->GetXaxis()->SetRangeUser(xMin, xMax);
    EA0C->GetYaxis()->SetRange(yMin1, yMax1);
    EA0C->GetXaxis()->SetTitle("x (mm)");
    EA0C->GetYaxis()->SetTitle("y (mm)");
    EA0C->SetStats(0);
    EA0C->SetMinimum(EA0C->GetMinimum(0.));

    auto yMin2{TA1C->FindFirstBinAbove(threshold, 2)};
    auto yMax2{TA1C->FindLastBinAbove(threshold, 2)};
    TA1C->GetXaxis()->SetRangeUser(xMin, xMax);
    TA1C->GetYaxis()->SetRange(yMin2, yMax2);
    TA1C->GetXaxis()->SetTitle("x (mm)");
    TA1C->GetYaxis()->SetTitle("y (mm)");
    TA1C->SetStats(0);
  
    EA1C->GetXaxis()->SetRangeUser(xMin, xMax);
    EA1C->GetYaxis()->SetRange(yMin2, yMax2);
    EA1C->GetXaxis()->SetTitle("x (mm)");
    EA1C->GetYaxis()->SetTitle("y (mm)");
    EA1C->SetStats(0);
    EA1C->SetMinimum(EA1C->GetMinimum(0.));

    auto yMin3{TA2C->FindFirstBinAbove(threshold, 2)};
    auto yMax3{TA2C->FindLastBinAbove(threshold, 2)};
    TA2C->GetXaxis()->SetRangeUser(xMin, xMax);
    TA2C->GetYaxis()->SetRange(yMin3, yMax3);
    TA2C->GetXaxis()->SetTitle("x (mm)");
    TA2C->GetYaxis()->SetTitle("y (mm)");
    TA2C->SetStats(0);
  
    EA2C->GetXaxis()->SetRangeUser(xMin, xMax);
    EA2C->GetYaxis()->SetRange(yMin3, yMax3);
    EA2C->GetXaxis()->SetTitle("x (mm)");
    EA2C->GetYaxis()->SetTitle("y (mm)");
    EA2C->SetStats(0);
    EA2C->SetMinimum(EA2C->GetMinimum(0.));
    
    // draw the canvases and then draw the histograms
    c1->cd(1);
    c1_1->SetRightMargin(0.16);
    TA0C->Draw("COLZ");
    c1->cd(4);
    c1_4->SetRightMargin(0.16);
    EA0C->Draw("COLZ");
    c1->cd(2);
    c1_2->SetRightMargin(0.16);
    TA1C->Draw("COLZ");
    c1->cd(5);
    c1_5->SetRightMargin(0.16);
    EA1C->Draw("COLZ");
    c1->cd(3);
    c1_3->SetRightMargin(0.16);
    TA2C->Draw("COLZ");
    c1->cd(6);
    c1_6->SetRightMargin(0.16);
    EA2C->Draw("COLZ");
    

    // Cauchy Distribution Fitting
    auto c2{new TCanvas("c2", "fit", 1000, 1500)};

    gStyle->SetOptStat();
    
    // Create three pads
    auto top_pad2{new TPad("top_pad2", "top_pad2",0,0.67,1,1)};
    //top_pad->SetLogy();
    top_pad2->Draw();

    auto mid_pad2{new TPad("mid_pad2", "mid_pad2",0,.33,1.0,0.67)};
    //mid_pad->SetLogy();
    mid_pad2->Draw();

    auto bot_pad2{new TPad("bot_pad2", "bot_pad2",0.0,0.0,1,0.33)};
    // bot_pad->SetLogy();
    bot_pad2->Draw();

    // get excess min and max y bins
    auto yMinE0{EA0C->FindFirstBinAbove(0,2)};
    auto yMaxE0{EA0C->FindLastBinAbove(0,2)};
    auto yMinE1{EA1C->FindFirstBinAbove(0,2)};
    auto yMaxE1{EA1C->FindLastBinAbove(0,2)};
    auto yMinE2{EA2C->FindFirstBinAbove(0,2)};
    auto yMaxE2{EA2C->FindLastBinAbove(0,2)};
    
    // get global min and max coordinates from min/max bins
    auto tMin0{EA0C->GetYaxis()->GetBinCenter(yMinE0) - 1};
    auto tMax0{EA0C->GetYaxis()->GetBinCenter(yMaxE0) + 1};
    auto tMin1{EA1C->GetYaxis()->GetBinCenter(yMinE1) - 1};
    auto tMax1{EA1C->GetYaxis()->GetBinCenter(yMaxE1) + 1};
    auto tMin2{EA2C->GetYaxis()->GetBinCenter(yMinE2) - 1};
    auto tMax2{EA2C->GetYaxis()->GetBinCenter(yMaxE2) + 1};
    
    // get y projection of the 2D histograms
    auto EA0CY{(TH1D*)EA0C->ProjectionY("PEA0C",yMinE0,yMaxE0)};
    auto EA1CY{(TH1D*)EA1C->ProjectionY("PEA1C",yMinE1,yMaxE1)};
    auto EA2CY{(TH1D*)EA2C->ProjectionY("PEA2C",yMinE2,yMaxE2)};

    //Cauchy distribution fit function
    auto ft{new TF1("fc","[2]/(3.14159*[1]*(1 + ((x-[0])/[1])*((x-[0])/[1])))")};
    if (i == 5) ft->SetParameters(0,1.0,50);
    ft->SetParNames("Peak_Coord","HWHM","Amplitude");
    
    // estimate parameters accurately
    
    // get peak coordinate
    auto maxBin0{EA0CY->GetMaximumBin()};
    auto peak0{EA0CY->GetBinCenter(maxBin0)};
    auto maxBin1{EA1CY->GetMaximumBin()};
    auto peak1{EA1CY->GetBinCenter(maxBin1)};
    auto maxBin2{EA2CY->GetMaximumBin()};
    auto peak2{EA2CY->GetBinCenter(maxBin2)};

    // get HWHM
    auto lBin0{EA0CY->FindFirstBinAbove(maxBin0/2)};
    auto rBin0{EA0CY->FindLastBinAbove(maxBin0/2)};
    auto hwhm0{(EA0CY->GetBinCenter(rBin0) - EA0CY->GetBinCenter(lBin0))/2};
    auto lBin1{EA1CY->FindFirstBinAbove(maxBin1/2)};
    auto rBin1{EA1CY->FindLastBinAbove(maxBin1/2)};
    auto hwhm1{(EA1CY->GetBinCenter(rBin1) - EA1CY->GetBinCenter(lBin1))/2};
    auto lBin2{EA2CY->FindFirstBinAbove(maxBin2/2)};
    auto rBin2{EA2CY->FindLastBinAbove(maxBin2/2)};
    auto hwhm2{(EA2CY->GetBinCenter(rBin2) - EA2CY->GetBinCenter(lBin2))/2};
    // get amplitude
    auto amp0{EA0CY->GetMaximum()};
    auto amp1{EA1CY->GetMaximum()};
    auto amp2{EA2CY->GetMaximum()};
    
    top_pad2->cd();
    // EA0CY->Scale(1./(Ntot/120));
    if (i != 5) ft->SetParameters(peak0, hwhm0, amp0);
    ft->SetRange(-60,60);
    EA0CY->Fit("fc");
    EA0CY->Draw("");
    EA0CY->SetFillColor(kBlue-4);
    top_pad->SetLeftMargin(0.15);
    EA0CY->GetYaxis()->SetTitleOffset(1.6); 
    EA0CY->GetYaxis()->SetTitle("excess charge ACE0");
    EA0CY->GetXaxis()->SetTitle("distance, mm");

    output << i << "   " << "1    " << ft->GetParameter(0) << "   " << ft->GetParameter(1) << "   " << ft->GetParameter(2) << endl;
   
    mid_pad2->cd();
    // EA1CY->Scale(1./(Ntot/120));
    if (i != 5) ft->SetParameters(peak1, hwhm1, amp1);
    ft->SetRange(-60,60);
    EA1CY->Fit("fc");
    EA1CY->Draw("");
    EA1CY->SetFillColor(kBlue-4);
    mid_pad->SetLeftMargin(0.15); 
    EA1CY->GetYaxis()->SetTitleOffset(1.6); 
    EA1CY->GetYaxis()->SetTitle("excess charge ACE1");
    EA1CY->GetXaxis()->SetTitle("distance, mm");

    output << i << "   " << "2    " << ft->GetParameter(0) << "   " << ft->GetParameter(1) << "   " << ft->GetParameter(2) << endl;
   
    bot_pad2->cd();
    // EA2CY->Scale(1./(Ntot/120));
    if (i != 5) ft->SetParameters(peak2, hwhm2, amp2);
    ft->SetRange(-60,60);
    EA2CY->Fit("fc");
    EA2CY->Draw("");
    EA2CY->SetFillColor(kBlue-4);
    bot_pad->SetLeftMargin(0.15);
    EA2CY->GetYaxis()->SetTitleOffset(1.6); 
    EA2CY->GetYaxis()->SetTitle("excess charge  ACE2");
    EA2CY->GetXaxis()->SetTitle("distance, mm");

    output << i << "   " << "3    " << ft->GetParameter(0) << "   " << ft->GetParameter(1) << "   " << ft->GetParameter(2) << endl;

    // Saving the data as pngs:

    // continue processing after drawing
    gSystem->ProcessEvents();

    // change directory to save path
    gSystem->cd(savePath);

    // construct image files
    auto cpng{TImage::Create()};
    auto c1png{TImage::Create()};
    auto c2png{TImage::Create()};

    cpng->FromPad(c);
    c1png->FromPad(c1);
    c2png->FromPad(c2);
  
    // and save the png's
    auto output1{"rapidity" + number + "_time.png"};
    cpng->WriteImage(output1.c_str());
    auto output2{"rapidity" + number + "_charge.png"};
    c1png->WriteImage(output2.c_str());
    auto output3{"rapidity" + number + "_fit.png"};
    c2png->WriteImage(output3.c_str());

    // continue processing after saving
    gSystem->ProcessEvents();

    // change to directory containing data
    gSystem->cd("../..");
    
  }

  output.close();
  
  // close ROOT
  gApplication->Terminate();
  
}
