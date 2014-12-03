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

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

using namespace CLHEP;

DetectorMessenger::DetectorMessenger(DetectorConstruction *pDetector)
:m_pDetector(pDetector)
{ 
	m_pDetectorDir = new G4UIdirectory("/G4simu/detector/");
	m_pDetectorDir->SetGuidance("detector control.");

	m_pSourcePosCmd = new G4UIcmdWithADoubleAndUnit("/G4simu/detector/setSourcePos", this);
	m_pSourcePosCmd->SetGuidance("Set source X position");
	m_pSourcePosCmd->AvailableForStates(G4State_PreInit);
	m_pSourcePosCmd->SetDefaultValue(0);
	m_pSourcePosCmd->SetDefaultUnit("cm");
    
    m_pNaIPosCmd = new G4UIcmdWithADoubleAndUnit("/G4simu/detector/setNaIPos", this);
    m_pNaIPosCmd->SetGuidance("Set NaI X position (center of detector)");
    m_pNaIPosCmd->AvailableForStates(G4State_PreInit);
    m_pNaIPosCmd->SetDefaultValue(0);
    m_pNaIPosCmd->SetDefaultUnit("cm");

}

DetectorMessenger::~DetectorMessenger()
{
  delete m_pDetectorDir;
}

void DetectorMessenger::SetNewValue(G4UIcommand *pUIcommand, G4String hNewValue)
{
        // source position
	if(pUIcommand == m_pSourcePosCmd)
			m_pDetector->SetSourcePos((m_pSourcePosCmd->GetNewDoubleValue(hNewValue)));
}
