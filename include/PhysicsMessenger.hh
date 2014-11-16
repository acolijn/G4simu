#ifndef __PHYSICSMESSENGER_H__
#define __PHYSICSMESSENGER_H__
//	
//  PhysicsMessenger class.
//
//  Allow for setting of preferences in the PhysicsList
//
//  A.P. Colijn 27-07-2011
//

#include <G4UImessenger.hh>
#include <globals.hh>

class PhysicsList;

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

class PhysicsMessenger: public G4UImessenger
{
public:
  PhysicsMessenger(PhysicsList *pPhysicsList);
  ~PhysicsMessenger();
  
  void SetNewValue(G4UIcommand *pCommand, G4String hNewValues);
private:
  PhysicsList         *m_pPhysicsList;
  G4UIdirectory              *m_pDirectory;
  G4UIcmdWithAString         *m_pEMlowEnergyModelCmd;
  G4UIcmdWithAString         *m_pHadronicModelCmd;
  G4UIcmdWithABool           *m_pCerenkovCmd;
  G4UIcmdWithABool           *m_pHistosCmd;

};

#endif // __PHYSICSMESSENGER_H__

