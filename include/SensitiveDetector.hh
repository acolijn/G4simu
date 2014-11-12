#ifndef __SENSITIVEDETECTOR_H__
#define __SENSITIVEDETECTOR_H__

#include <G4VSensitiveDetector.hh>

#include "stdHit.hh"

class G4Step;
class G4HCofThisEvent;

class SensitiveDetector: public G4VSensitiveDetector
{
public:
	SensitiveDetector(G4String hName);
	~SensitiveDetector();

	void Initialize(G4HCofThisEvent *pHitsCollectionOfThisEvent);
	G4bool ProcessHits(G4Step *pStep, G4TouchableHistory *pHistory);
	void EndOfEvent(G4HCofThisEvent *pHitsCollectionOfThisEvent);

private:
	stdHitsCollection* m_pHitsCollection;
        G4int iHitsCollectionID;

	std::map<int,G4String> m_hParticleTypes;
};

#endif // __SENSITIVEDETECTOR_H__

