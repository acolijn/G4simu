#ifndef SteppingAction_H
#define SteppingAction_H 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class AnalysisManager;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(AnalysisManager* );
  ~SteppingAction(){};

  void UserSteppingAction(const G4Step*);

private:

G4String particle;
AnalysisManager* myAnalysisManager;


};

#endif
