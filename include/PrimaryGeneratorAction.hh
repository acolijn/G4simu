#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include <TFile.h>
#include <TH1.h>
#include <TParameter.h>

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(G4String fname);
  virtual ~PrimaryGeneratorAction();

  // APC added functions
  const G4String &GetParticleTypeOfPrimary() { return m_hParticleTypeOfPrimary; }
  G4double        GetEnergyOfPrimary() { return m_dEnergyOfPrimary; }
  G4ThreeVector   GetPositionOfPrimary() { return m_hPositionOfPrimary; }
  G4ThreeVector   GetDirectionOfPrimary() { return m_hPositionOfPrimary; }

public:
  virtual void GeneratePrimaries(G4Event* anEvent);

private:
  G4GeneralParticleSource* fParticleGun;
  
  // APC added variables
  G4String         m_hParticleTypeOfPrimary;
  G4double         m_dEnergyOfPrimary;
  G4ThreeVector    m_hPositionOfPrimary;
  G4ThreeVector    m_hDirectionOfPrimary;


  // histograms
  TFile      *_f_prim;
  TDirectory *_generator;
  G4String         generatorRootFile;

  TH1F       *_log_primary_w;
  TH1F       *_cost_all;
  TH1F       *_cost_accept;
  TH1F       *_energy_all;
  TH1F       *_energy_accept;

};

#endif

