void xsecGamma(string fileName, string matName){
  char hName[100], hTitle[100];
  // plotting routines
  gStyle->SetOptLogy();
  gStyle->SetMarkerStyle(0);
  gStyle->SetMarkerSize(0);

  TCanvas *xsec = new TCanvas("xsec","xsec",10,20,700,500);

  TFile *_f = TFile::Open(fileName.c_str());
  _f->cd("physics/electromagnetic");
  string modelName = EMmodel->GetTitle();

  // Compton
  sprintf(hName,"%s_compt",matName.c_str());
  TH1F *_compt = (TH1F*)(gDirectory->Get(hName));
  _compt->SetTitle("Compton");
  TH1F *_tot   = (TH1F*)_compt->Clone();
  sprintf(hName,"Total - %s",matName.c_str());
  _tot->SetTitle(hName);

  // photo-electric
  sprintf(hName,"%s_phot",matName.c_str());
  TH1F *_phot  = (TH1F*)(gDirectory->Get(hName));
  _phot->SetTitle("Photo Electric");
  _tot->Add(_phot,1);
  // conversion
  sprintf(hName,"%s_conv",matName.c_str());
  TH1F *_conv  = (TH1F*)(gDirectory->Get(hName));
  _conv->SetTitle("Conversion");
  _tot->Add(_conv,1);

  _tot->SetLineColor(1);
  _tot->GetXaxis()->SetTitle("^{10}log(E/MeV)");
  _tot->GetYaxis()->SetTitle("Mass attenuation (cm^{2}/g)");

  _phot->SetLineColor(2);
  _compt->SetLineColor(6);
  _conv->SetLineColor(4);

  _phot->SetLineStyle(2);
  _compt->SetLineStyle(3);
  _conv->SetLineStyle(4);

  
  _f->cd("physics");  
  std::string ledata = G4LEDATA->GetTitle();
  printf("%s \n",ledata.c_str());
  
  sprintf(hTitle,"EM physics: %s   EM data: %s",modelName.c_str(),ledata.c_str());
  xsec->SetTitle(hTitle);

  _tot->Draw("l");
  _tot->SetMinimum(1e-3);
  _phot->Draw("lsame");
  _compt->Draw("lsame");
  _conv->Draw("lsame");

  TLegend *leg = xsec->BuildLegend();
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  

  
  sprintf(hName,"pdf/%s_attentuation.pdf",matName.c_str());
  xsec->Print(hName);

}
