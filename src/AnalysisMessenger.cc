#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
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

#include "AnalysisMessenger.hh"
#include "AnalysisManager.hh"

using namespace CLHEP;

AnalysisMessenger::AnalysisMessenger(AnalysisManager *pAnalysis)
:m_pAnalysis(pAnalysis)
{ 
	m_pAnalysisDir = new G4UIdirectory("/G4simu/analysis/");
	m_pAnalysisDir->SetGuidance("detector control.");

	m_pTreeTypeCmd = new G4UIcmdWithAString("/G4simu/analysis/setTreeType", this);
	m_pTreeTypeCmd->SetGuidance("Set output tree type: <raw> <compact>");
	m_pTreeTypeCmd->AvailableForStates(G4State_PreInit);
	m_pTreeTypeCmd->SetDefaultValue("raw");

}

AnalysisMessenger::~AnalysisMessenger()
{
  delete m_pAnalysisDir;
}

void AnalysisMessenger::SetNewValue(G4UIcommand *pUIcommand, G4String hNewValue)
{
        // source position
	if(pUIcommand == m_pTreeTypeCmd) m_pAnalysis->SetTreeType(hNewValue);
}
