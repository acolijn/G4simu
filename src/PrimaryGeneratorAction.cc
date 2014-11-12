#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

PrimaryGeneratorAction::PrimaryGeneratorAction(G4String fName)
 : G4VUserPrimaryGeneratorAction(), 
   fParticleGun(0)
{
  fParticleGun = new G4GeneralParticleSource ();

  // APC added
  m_hParticleTypeOfPrimary = "";
  m_dEnergyOfPrimary = 0.;
  m_hPositionOfPrimary = G4ThreeVector(0., 0., 0.);
  m_hDirectionOfPrimary = G4ThreeVector(0., 0., 0.);
  
  generatorRootFile = fName;
  // histograms.....
  _f_prim = new TFile(generatorRootFile,"RECREATE");
  
  _generator = _f_prim->mkdir("generator");
  
  _generator->cd();
  _log_primary_w = new TH1F("_log_primary_w","_log_primary_w",100,-20,0);
  _cost_all      = new TH1F("_cost_all","_cost_all",100,-1,1);
  _energy_all    = new TH1F("_energy_all","_energy_all",1000,0,5);
  
  _cost_accept   = new TH1F("_cost_accept","_cost_accept",100,-1,1);
  _energy_accept = new TH1F("_energy_accept","_energy_accept",1000,0,5);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  _f_prim->cd();
  _generator->cd();

  _f_prim->cd();
  
  _f_prim->Write();
  _f_prim->Close();

  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // generate an event
  fParticleGun->GeneratePrimaryVertex(anEvent);

  // particle name of primary
  m_hParticleTypeOfPrimary = fParticleGun->GetParticleDefinition()->GetParticleName();
  // kinetic energy of primary
  m_dEnergyOfPrimary       = fParticleGun->GetParticleEnergy();
  // position of primary
  m_hPositionOfPrimary     = fParticleGun->GetParticlePosition();
  // direction of primary
  m_hDirectionOfPrimary    = fParticleGun->GetParticleMomentumDirection();
  // fill histograms
  _energy_accept->Fill(m_dEnergyOfPrimary);
  _cost_accept->Fill(m_hDirectionOfPrimary.z());
}

