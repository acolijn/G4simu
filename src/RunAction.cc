#include <Randomize.hh>

#include <sys/time.h>

#include "AnalysisManager.hh"

#include "RunAction.hh"

RunAction::RunAction(AnalysisManager *pAnalysisManager)
{
  m_hRanSeed         = 12345; // default value
  m_pMessenger       = new RunActionMessenger(this);
  
  m_pAnalysisManager = pAnalysisManager;
  
}

RunAction::~RunAction()
{
  delete m_pMessenger;
}

void
RunAction::BeginOfRunAction(const G4Run *pRun)
{
  if(m_pAnalysisManager) {
    //    m_pAnalysisManager->SetForcedTransport(m_hForcedTransport);
    m_pAnalysisManager->BeginOfRun(pRun);
  }
  
  // random seeding of the MC
  if(m_hRanSeed > 0){
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    G4cout << "RunAction::BeginOfRunAction Initialize random numbers with seed = "<<m_hRanSeed<<G4endl;
    CLHEP::HepRandom::setTheSeed(m_hRanSeed);
  } else {
    // initialize with time.....
    struct timeval hTimeValue;
    gettimeofday(&hTimeValue, NULL);
    G4cout << "RunAction::BeginOfRunAction WARNING RANDOM NUMBERS INITIALIZED BY TIME !"<<G4endl;
    G4cout << "RunAction::BeginOfRunAction Initialize random numbers with seed = "<<hTimeValue.tv_usec<<G4endl;
    CLHEP::HepRandom::setTheSeed(hTimeValue.tv_usec);
  }
}

void
RunAction::EndOfRunAction(const G4Run *pRun)
{
  if(m_pAnalysisManager)
    m_pAnalysisManager->EndOfRun(pRun);
}

