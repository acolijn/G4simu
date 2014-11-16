void xsecBeta(string fileName, string matName){
  char hName[100], hTitle[100], pName[100];
  // plotting routines
  gStyle->SetOptLogy();
  gStyle->SetMarkerStyle(0);
  gStyle->SetMarkerSize(0);

  TCanvas *xsec = new TCanvas("xsec","xsec",10,20,700,500);

  TFile *_f = TFile::Open(fileName.c_str());
  _f->cd("physics/electromagnetic");
  string modelName = EMmodel->GetTitle();

  sprintf(pName,"%s_X0",matName.c_str());
  TParameter<double> *_X0 = (TParameter<double>*)(gDirectory->Get(pName));
  
  // msc
  sprintf(hName,"%s_msc",matName.c_str());
  TH1F *_msc = (TH1F*)(gDirectory->Get(hName));
  _msc->SetTitle("msc");
  TH1F *_tot   = (TH1F*)_msc->Clone();
  sprintf(hName,"Total - %s    X0 = %3.1f (cm)",matName.c_str(),_X0->GetVal());
  _tot->SetTitle(hName);

  // eIoni 
  sprintf(hName,"%s_eIoni",matName.c_str());
  TH1F *_eIoni  = (TH1F*)(gDirectory->Get(hName));
  _eIoni->SetTitle("eIoni");
  _tot->Add(_eIoni,1);
  // eBrem
  sprintf(hName,"%s_eBrem",matName.c_str());
  TH1F *_eBrem  = (TH1F*)(gDirectory->Get(hName));
  _eBrem->SetTitle("eBrem");
  _tot->Add(_eBrem,1);

  _tot->SetLineColor(1);
  _tot->GetXaxis()->SetTitle("^{10}log(E/MeV)");
  _tot->GetYaxis()->SetTitle("- #frac{1}{E} #frac{dE}{dx}  (X_{0}^{-1})");

  _msc->SetLineColor(2);
  _eIoni->SetLineColor(6);
  _eBrem->SetLineColor(4);

  _msc->SetLineStyle(2);
  _eIoni->SetLineStyle(3);
  _eBrem->SetLineStyle(4);

  
  _f->cd("physics");  
  std::string ledata = G4LEDATA->GetTitle();
  printf("%s \n",ledata.c_str());
  
  sprintf(hTitle,"EM physics: %s   EM data: %s",modelName.c_str(),ledata.c_str());
  xsec->SetTitle(hTitle);

  _tot->GetXaxis()->SetRangeUser(-1.,10.);
  _tot->Draw("l");
  _tot->SetMinimum(1e-1);
//  _msc->Draw("lsame");
  _eIoni->Draw("lsame");
  _eBrem->Draw("lsame");

  TLegend *leg = xsec->BuildLegend();
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  

  
  sprintf(hName,"pdf/%s_eloss.pdf",matName.c_str());
  xsec->Print(hName);

}
