#include <G4Event.hh>

#include "EventAction.hh"

EventAction::EventAction(AnalysisManager *pAnalysisManager)
{
  m_pAnalysisManager = pAnalysisManager;
}

EventAction::~EventAction()
{
}

void
EventAction::BeginOfEventAction(const G4Event *pEvent)
{
  if(pEvent->GetEventID() % 10000 == 0)
    {
      G4cout << "EventAction::BeginOfEventAction:: Begin event " << pEvent->GetEventID()<< "------" << G4endl;
    }
  
  if(m_pAnalysisManager)
    m_pAnalysisManager->BeginOfEvent(pEvent);
}

void EventAction::EndOfEventAction(const G4Event *pEvent)
{
  if(m_pAnalysisManager)
    m_pAnalysisManager->EndOfEvent(pEvent);
}


