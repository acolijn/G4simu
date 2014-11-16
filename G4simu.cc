#include <string>
#include <sstream>
#include <unistd.h>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4UIterminal.hh>
#include <G4UItcsh.hh>
#include <G4VisExecutive.hh>
#include <G4GeneralParticleSource.hh>

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "AnalysisManager.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "fileMerger.hh"

#include "stdHit.hh"

// SVN

void usage();

/* 
 * Main: G4simu
 *   
 * Goal: simulation of XAMS setup
 *
 * A.P. Colijn - nov 2014 
 */

int
main(int argc, char **argv)
{
  // switches
  int c = 0;
  
  std::stringstream hStream;
	
  bool bInteractive = false;
  bool bVisualize = false;
  bool bVrmlVisualize = false;
  bool bOpenGlVisualize = false;
  bool bPreInitFromFile = false;
	
  bool bMacroFile = false;
  std::string hMacroFilename, hDataFilename, hPreInitFilename;
  std::string hCommand;
  int iNbEventsToSimulate = 0;
  
  // parse switches ... 
  while((c = getopt(argc,argv,"v:f:o:p:n:i")) != -1){
    switch(c)	{
        
      case 'v': 
        bVisualize = true;
        hStream.str(optarg);
        if(hStream.str() == "vrml")
          bVrmlVisualize = true;
        else if(hStream.str() == "opengl")
          bOpenGlVisualize = true;
        hStream.clear();
        break;
        
      case 'f':
        bMacroFile = true;
        hMacroFilename = optarg;
        break;
        
      case 'o':
        hDataFilename = optarg;
        break;
				
      case 'p':
        bPreInitFromFile = true;
        hPreInitFilename = optarg;
        break;
        
      case 'n':
        hStream.str(optarg);
        hStream.clear();
        hStream >> iNbEventsToSimulate;
        break;
        
      case 'i':
        bInteractive = true;
        break;
        
      default:
        usage();
    }
  }

  stdHit *pHit = new stdHit();
  
  // 
  if(hDataFilename.empty()) hDataFilename = "events.root";
  
  // create the run manager
  G4RunManager *pRunManager = new G4RunManager;
	
  // Detector Construction
  G4String detectorRoot = "DET_"+hDataFilename;
  DetectorConstruction *detCon = new DetectorConstruction(detectorRoot); 
  pRunManager->SetUserInitialization(detCon);
  
  // Physics List
  G4String physicsRoot = "PHYS_"+hDataFilename;
  PhysicsList *physList = new PhysicsList(physicsRoot); 
  pRunManager->SetUserInitialization(physList);
	
  // Visualization Manager
  G4VisManager* pVisManager = new G4VisExecutive;
  pVisManager->Initialize();
  
  // create the primary generator action
  G4String generatorRoot = "GEN_"+hDataFilename;
  PrimaryGeneratorAction *pPrimaryGeneratorAction = new PrimaryGeneratorAction(generatorRoot);
  
  // create an analysis manager object
  AnalysisManager *pAnalysisManager = new AnalysisManager(pPrimaryGeneratorAction);
  pAnalysisManager->SetDataFilename(hDataFilename);
  
  if(iNbEventsToSimulate) pAnalysisManager->SetNbEventsToSimulate(iNbEventsToSimulate);
	
  // set user-defined action classes
  pRunManager->SetUserAction(pPrimaryGeneratorAction);
  pRunManager->SetUserAction(new StackingAction(pAnalysisManager));
  pRunManager->SetUserAction(new SteppingAction(pAnalysisManager));
  pRunManager->SetUserAction(new RunAction(pAnalysisManager));
  pRunManager->SetUserAction(new EventAction(pAnalysisManager));
  
  // geometry IO
  G4UImanager* pUImanager = G4UImanager::GetUIpointer();
  
  if(bPreInitFromFile)
  {
    hCommand = "/control/execute " + hPreInitFilename;
    pUImanager->ApplyCommand(hCommand);
  }
	
  // initialize it all....
  pRunManager->Initialize();
  
  G4UIsession * pUIsession = 0;
  if(bInteractive) pUIsession = new G4UIterminal(new G4UItcsh);
  
  if(bVisualize)
  {
    pUImanager->ApplyCommand("/vis/scene/create");
    if(bVrmlVisualize)
      pUImanager->ApplyCommand("/vis/open VRML2FILE");
    if(bOpenGlVisualize)
      pUImanager->ApplyCommand("/vis/open OGLIX");
		
    pUImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90 0 deg");
    pUImanager->ApplyCommand("/vis/viewer/set/upVector 0 0 1");
    pUImanager->ApplyCommand("/vis/viewer/zoom 1.0");
    pUImanager->ApplyCommand("/tracking/storeTrajectory 1");
    pUImanager->ApplyCommand("/vis/scene/add/trajectories");
  }
  
  // run time parameter settings
  if(bMacroFile)
  {
    hCommand = "/control/execute " + hMacroFilename;
    pUImanager->ApplyCommand(hCommand);
  }
	
  
  if(iNbEventsToSimulate)
  {
    hStream.str("");
    hStream.clear();
    hStream << "/run/beamOn " << iNbEventsToSimulate;
    pUImanager->ApplyCommand(hStream.str());
  }
  
  if(bInteractive)
  {
    pUIsession->SessionStart();
    delete pUIsession;
  }
  
  delete pAnalysisManager;
  if(bVisualize) delete pVisManager;
  delete pRunManager;
  
  // merge the output files to one .... events.root
  vector<std::string> auxfiles;
  auxfiles.push_back(detectorRoot);
  auxfiles.push_back(physicsRoot);
  auxfiles.push_back(generatorRoot);

  fileMerger myMerger(hDataFilename,auxfiles);
  myMerger.Merge();
  myMerger.CleanUp();
	
  return 0;
}

void
usage()
{
  exit(0);
}

