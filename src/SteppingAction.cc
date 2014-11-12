#include "SteppingAction.hh"
#include "AnalysisManager.hh"

#include "G4SteppingManager.hh"

#include <string.h>
#include <cmath>

SteppingAction::SteppingAction(AnalysisManager *myAM):myAnalysisManager(myAM)
{
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  

}



