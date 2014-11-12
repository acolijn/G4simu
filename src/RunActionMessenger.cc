#include <G4Geantino.hh>
#include <G4ThreeVector.hh>
#include <G4ParticleTable.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3Vector.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithABool.hh>
#include <G4Tokenizer.hh>
#include <G4ios.hh>
#include <fstream>
#include <iomanip>

#include "RunActionMessenger.hh"
#include "RunAction.hh"

RunActionMessenger::RunActionMessenger(RunAction *pRunAction):
m_pRunAction(pRunAction)
{
	// create directory
	m_pDirectory = new G4UIdirectory("/run/random/");
	m_pDirectory->SetGuidance("RunAction control commands.");

	// set random seed
	m_pRanSeedCmd = new G4UIcmdWithAnInteger("/run/random/setRandomSeed", this);
	m_pRanSeedCmd->SetGuidance("Random seed");
	G4int iDef = 12345;
	m_pRanSeedCmd->SetDefaultValue(iDef);

}

RunActionMessenger::~RunActionMessenger()
{
	delete m_pDirectory;
}

void
RunActionMessenger::SetNewValue(G4UIcommand * command, G4String newValues)
{
	if(command == m_pRanSeedCmd)
		m_pRunAction->SetRanSeed(m_pRanSeedCmd->GetNewIntValue(newValues));
}

