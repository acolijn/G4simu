#ifndef __RUNACTIONMESSENGER_H__
#define __RUNACTIONMESSENGER_H__

#include <G4UImessenger.hh>
#include <globals.hh>

class RunAction;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class RunActionMessenger: public G4UImessenger
{
public:
  RunActionMessenger(RunAction *pRunAction);
  ~RunActionMessenger();
  
  void SetNewValue(G4UIcommand *pCommand, G4String hNewValues);
private:
  RunAction           *m_pRunAction;
  G4UIdirectory              *m_pDirectory;
  G4UIcmdWithAnInteger       *m_pRanSeedCmd;
};

#endif 

