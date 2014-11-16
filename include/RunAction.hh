#ifndef __PRUNACTION_H__
#define __PRUNACTION_H__

#include <G4UserRunAction.hh>
#include "RunActionMessenger.hh"

class G4Run;

class AnalysisManager;


class RunAction: public G4UserRunAction
{
public:
	RunAction(AnalysisManager *pAnalysisManager=0);
	~RunAction();

public:
	void BeginOfRunAction(const G4Run *pRun);
	void EndOfRunAction(const G4Run *pRun);
        
	void SetRanSeed(G4int hRanSeed) { m_hRanSeed = hRanSeed; }

private:
	G4int m_hRanSeed;
	AnalysisManager *m_pAnalysisManager;
	RunActionMessenger *m_pMessenger;
};

#endif // __PRUNACTION_H__

