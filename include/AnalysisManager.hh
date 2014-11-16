#ifndef __ANALYSISMANAGER_H__
#define __ANALYSISMANAGER_H__

#include <globals.hh>

#include <TParameter.h>
#include <TDirectory.h>
#include <G4Timer.hh>
#include <G4ThreeVector.hh>

class G4Run;
class G4Event;
class G4Step;

class TFile;
class TTree;

class AnalysisMessenger;
class EventData;
class PrimaryGeneratorAction;

class AnalysisManager
{
public:
    AnalysisManager(PrimaryGeneratorAction *pPrimaryGeneratorAction);
    virtual ~AnalysisManager();
    
public:
    virtual void BeginOfRun(const G4Run *pRun);
    virtual void EndOfRun(const G4Run *pRun);
    virtual void BeginOfEvent(const G4Event *pEvent);
    virtual void EndOfEvent(const G4Event *pEvent);
    virtual void Step(const G4Step *pStep);
    
    void SetDataFilename(const G4String &hFilename) { m_hDataFilename = hFilename; }
    void SetNbEventsToSimulate(G4int iNbEventsToSimulate) { m_iNbEventsToSimulate = iNbEventsToSimulate;}
    void SetTreeType(G4String ttype){m_hTreeType=ttype;}
    
private:
    G4bool FilterEvent(EventData *pEventData);
    
private:
    G4int m_LXeCollectionID;
    G4int m_NaICollectionID;
    vector<G4int> m_CollectionIDs;
    
    G4String m_hDataFilename;
    G4int m_iNbEventsToSimulate;
    
    TFile      *m_pTreeFile;
    TTree      *m_pTree;
    TDirectory *_events;
    
    TParameter<int> *m_pNbEventsToSimulateParameter;
    //  TParameter<double> *nRejectParameter;
    // TParameter<double> *nAcceptParameter;
    
    PrimaryGeneratorAction *m_pPrimaryGeneratorAction;
    
    EventData *m_pEventData;
    G4bool            plotPhysics;
    
    G4Timer *runTime;
    G4bool            writeEmptyEvents;
    
    AnalysisMessenger *m_pAnalysisMessenger;
    G4String m_hTreeType;

};

#endif 

