#ifndef __PEVENTACTION_H__
#define __PEVENTACTION_H__

#include <G4UserEventAction.hh>

#include "AnalysisManager.hh"

class G4Event;

class EventAction : public G4UserEventAction
{
public:
	EventAction(AnalysisManager *pAnalysisManager = 0);
	~EventAction();

public:
	void BeginOfEventAction(const G4Event *pEvent);
	void EndOfEventAction(const G4Event *pEvent);

private:
	AnalysisManager *m_pAnalysisManager;
};

#endif // __PEVENTACTION_H__

