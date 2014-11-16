#ifndef _AnalysisMessenger_h
#define _AnalysisMessenger_h 1

#include <G4UImessenger.hh>
#include <globals.hh>

class AnalysisManager;

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

class AnalysisMessenger: public G4UImessenger
{
public:
	AnalysisMessenger(AnalysisManager *pAnalysis);
	~AnalysisMessenger();

	void     SetNewValue(G4UIcommand *pUIcommand, G4String hString);
private:
	AnalysisManager* m_pAnalysis;

	G4UIdirectory *m_pAnalysisDir;

        G4UIcmdWithAString *m_pTreeTypeCmd;
};

#endif

