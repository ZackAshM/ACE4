/*
ROOT Macro to save ACE4 Charge Histograms into ~/ACE4Sim/data as pngs
*/

{

  // prevent popup windows
  gROOT->SetBatch(kTRUE);
  
  // save path
  auto savePath{"data/"};
  
  // open ROOT data file
  auto file{TFile::Open("ACE4Sim_Data_1.root")};

  // create canvas for each ACE charge histogram
  auto c0{new TCanvas("c0", "ACE0Charge", 800, 1600)};
  c0->Divide(1,2);                   // split canvas into two
  c0_1->SetRightMargin(0.13);        // set right margin; default 0.1
  c0_2->SetRightMargin(0.13);
  c0_1->SetLogz(); c0_2->SetLogz();  // set logscale on entries
  auto c1{new TCanvas("c1", "ACE1Charge", 800, 1600)};
  c1->SetRightMargin(0.13);
  c1->Divide(1,2);
  c1_1->SetRightMargin(0.13);
  c1_2->SetRightMargin(0.13);
  c1_1->SetLogz(); c1_2->SetLogz();
  auto c2{new TCanvas("c2", "ACE2Charge", 800, 1600)};
  c2->SetRightMargin(0.13);
  c2->Divide(1,2);
  c2_1->SetRightMargin(0.13);
  c2_2->SetRightMargin(0.13);
  c2_1->SetLogz(); c2_2->SetLogz();

  // load H2 objects
  auto TA0C{(TH2D*)file->Get("TotalACE0Charge")};
  auto EA0C{(TH2D*)file->Get("ExcessACE0Charge")};
  auto TA1C{(TH2D*)file->Get("TotalACE1Charge")};
  auto EA1C{(TH2D*)file->Get("ExcessACE1Charge")};
  auto TA2C{(TH2D*)file->Get("TotalACE2Charge")};
  auto EA2C{(TH2D*)file->Get("ExcessACE2Charge")};

  // here are commands to change the properties of the plots
  TA0C->GetXaxis()->SetRangeUser(-6, 6);    // change x range to -6, 6
  TA0C->GetYaxis()->SetRangeUser(-6, 6);    // change y range to -6, 6
  TA0C->GetXaxis()->SetTitle("x (mm)");     // set x axis title
  TA0C->GetYaxis()->SetTitle("y (mm)");     // set y axis title
  TA0C->SetStats(0);                        // remove stats box
  
  EA0C->GetXaxis()->SetRangeUser(-6, 6);
  EA0C->GetYaxis()->SetRangeUser(-6, 6);
  EA0C->GetXaxis()->SetTitle("x (mm)");
  EA0C->GetYaxis()->SetTitle("y (mm)");
  EA0C->SetStats(0);
  EA0C->SetMinimum(EA0C->GetMinimum(0.));
  
  TA1C->GetXaxis()->SetRangeUser(-6, 6);
  TA1C->GetYaxis()->SetRangeUser(-6, 6);
  TA1C->GetXaxis()->SetTitle("x (mm)");
  TA1C->GetYaxis()->SetTitle("y (mm)");
  TA1C->SetStats(0);
  
  EA1C->GetXaxis()->SetRangeUser(-6, 6);
  EA1C->GetYaxis()->SetRangeUser(-6, 6);
  EA1C->GetXaxis()->SetTitle("x (mm)");
  EA1C->GetYaxis()->SetTitle("y (mm)");
  EA1C->SetStats(0);
  EA1C->SetMinimum(EA1C->GetMinimum(0.));
  
  TA2C->GetXaxis()->SetRangeUser(-6, 6);
  TA2C->GetYaxis()->SetRangeUser(-6, 6);
  TA2C->GetXaxis()->SetTitle("x (mm)");
  TA2C->GetYaxis()->SetTitle("y (mm)");
  TA2C->SetStats(0);
  
  EA2C->GetXaxis()->SetRangeUser(-6, 6);
  EA2C->GetYaxis()->SetRangeUser(-6, 6);
  EA2C->GetXaxis()->SetTitle("x (mm)");
  EA2C->GetYaxis()->SetTitle("y (mm)");
  EA2C->SetStats(0);
  EA2C->SetMinimum(EA2C->GetMinimum(0.));

  // draw the canvases and then draw the histograms
  c0->cd(1);
  TA0C->Draw("COLZ");
  c0->cd(2);
  EA0C->Draw("COLZ");
  c1->cd(1);
  TA1C->Draw("COLZ");
  c1->cd(2);
  EA1C->Draw("COLZ");
  c2->cd(1);
  TA2C->Draw("COLZ");
  c2->cd(2);
  EA2C->Draw("COLZ");

  // continue processing after drawing
  gSystem->ProcessEvents();

  // change directory to save path
  gSystem->cd(savePath);

  // construct image files
  auto c0png{TImage::Create()};
  auto c1png{TImage::Create()};
  auto c2png{TImage::Create()};
  
  c0png->FromPad(c0);
  c1png->FromPad(c1);
  c2png->FromPad(c2);

  // and save the png's
  c0png->WriteImage("ACE0ChargeHist.png");
  c1png->WriteImage("ACE1ChargeHist.png");
  c2png->WriteImage("ACE2ChargeHist.png");

  // close ROOT
  gApplication->Terminate();

}
