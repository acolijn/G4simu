#ifndef _DetectorMessenger_h
#define _DetectorMessenger_h 1

#include <G4UImessenger.hh>
#include <globals.hh>

class DetectorConstruction;

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

class DetectorMessenger: public G4UImessenger
{
public:
	DetectorMessenger(DetectorConstruction *pDetector);
	~DetectorMessenger();

	void     SetNewValue(G4UIcommand *pUIcommand, G4String hString);
private:
	DetectorConstruction* m_pDetector;

	G4UIdirectory *m_pDetectorDir;

        G4UIcmdWithADoubleAndUnit *m_pSourcePosCmd;
};

#endif

