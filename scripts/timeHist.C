/*
ROOT Macro to save ACE4 Hit Time Histograms into ~/ACE4Sim/data as pngs
*/

{

  // save path
  auto savePath{"data/"};
  
  // open ROOT data file
  auto file{TFile::Open("ACE4Sim_Data_2.root")};

  // create canvas for each ACE element
  auto c{new TCanvas("c", "ACE Hit Time Profile", 800, 800)};

  // turn stats on
  gStyle->SetOptStat();
  
  // load H1 objects
  auto HTA0{(TH1D*)file->Get("HitTimesACE0")};  
  auto HTA1{(TH1D*)file->Get("HitTimesACE1")};
  auto HTA2{(TH1D*)file->Get("HitTimesACE2")};

  // create three pads
  auto top_pad{new TPad("top_pad", "top_pad",0,0.67,1,1)};
  top_pad->Draw();

  auto mid_pad{new TPad("mid_pad", "mid_pad",0,.33,1.0,0.67)};
  mid_pad->Draw();

  auto bot_pad{new TPad("bot_pad", "bot_pad",0.0,0.0,1,0.33)};
  bot_pad->Draw();
  
  // draw the canvases and then draw the histograms
  top_pad->cd();
  HTA0->Draw("hist same bar");
  HTA0->SetFillColor(kBlue-4);
  HTA0->GetYaxis()->SetTitle("Hits");
  HTA0->GetXaxis()->SetTitle("time (ns)");
  mid_pad->cd();
  HTA1->Draw("hist same bar");
  HTA1->SetFillColor(kBlue-4);
  HTA1->GetYaxis()->SetTitle("Hits");
  HTA1->GetXaxis()->SetTitle("time (ns)");
  bot_pad->cd();
  HTA2->Draw("hist same bar");
  HTA2->SetFillColor(kBlue-4);
  HTA2->GetYaxis()->SetTitle("Hits");
  HTA2->GetXaxis()->SetTitle("time (ns)");

  // continue processing after drawing
  gSystem->ProcessEvents();

  // change directory to save path
  gSystem->cd(savePath);

  // construct image file
  auto cpng{TImage::Create()};

  cpng->FromPad(c);
  
  // and save the png's
  cpng->WriteImage("ACEHitTimesHist.png");

}
