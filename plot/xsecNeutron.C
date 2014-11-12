void xsecNeutron(string fileName, string matName){
  char hName[100], hTitle[100];
  // plotting routines
  gStyle->SetOptLogy();
  gStyle->SetMarkerStyle(0);
  gStyle->SetMarkerSize(0);

  TCanvas *xsec = new TCanvas("xsec","xsec",10,20,700,500);

  TFile *_f = TFile::Open(fileName.c_str());
  _f->cd("physics/hadronic");
  string modelName = HADmodel->GetTitle();

  // Comton
  sprintf(hName,"%s_neutron_inelastic",matName.c_str());
  TH1F *_inelastic = (TH1F*)(gDirectory->Get(hName));
  _inelastic->SetTitle("Inelastic");
  TH1F *_tot   = (TH1F*)_inelastic->Clone();
  sprintf(hName,"Total - %s",matName.c_str());
  _tot->SetTitle(hName);

  // photo-electric
  sprintf(hName,"%s_neutron_elastic",matName.c_str());
  TH1F *_elastic  = (TH1F*)(gDirectory->Get(hName));
  _elastic->SetTitle("Elastic");
  _tot->Add(_elastic,1);
  // conversion
  sprintf(hName,"%s_neutron_capture",matName.c_str());
  TH1F *_capture  = (TH1F*)(gDirectory->Get(hName));
  _capture->SetTitle("Capture");
  _tot->Add(_capture,1);

  _tot->SetLineColor(1);
  _tot->GetXaxis()->SetTitle("^{10}log(E/MeV)");
  sprintf(hName,"Attenuation coefficient (cm^{-1})");
  _tot->GetYaxis()->SetTitle(hName);

  _elastic->SetLineColor(2);
  _inelastic->SetLineColor(6);
  _capture->SetLineColor(4);

  _elastic->SetLineStyle(2);
  _inelastic->SetLineStyle(3);
  _capture->SetLineStyle(4);

  sprintf(hTitle,"Hadronic model: %s", modelName.c_str());
  xsec->SetTitle(hTitle);
  _tot->Draw("l");
//  _tot->SetMinimum(1e-2);
  _elastic->Draw("lsame");
  _inelastic->Draw("lsame");
  _capture->Draw("lsame");

  TLegend *leg = xsec->BuildLegend();
  leg->SetFillStyle(0);
  leg->SetFillColor(0);
  
  sprintf(hName,"pdf/%s_neutron_xsec.pdf",matName.c_str());
  xsec->Print(hName);
}
