#ifndef __PSTACKINGACTION_H__
#define __PSTACKINGACTION_H__

#include <globals.hh>
#include <G4UserStackingAction.hh>

class AnalysisManager;

class StackingAction: public G4UserStackingAction
{
public:
	StackingAction(AnalysisManager *pAnalysisManager=0);
	~StackingAction();
  
	virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
	virtual void NewStage();
	virtual void PrepareNewEvent();

private:
	AnalysisManager *m_pAnalysisManager;
};

#endif // __PSTACKINGACTION_H__

