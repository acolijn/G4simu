#ifndef __DETECTORCONSTRUCTION_H__
#define __DETECTORCONSTRUCTION_H__

#include <globals.hh>

#include <vector>
#include <map>
#include "TFile.h"
#include "TDirectory.h"

using std::vector;
using std::map;

class G4Sphere;
class G4Colour;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorMessenger;
class G4SubtractionSolid;

#include <G4UnionSolid.hh>
#include <G4UImanager.hh>

#include <G4VUserDetectorConstruction.hh>

class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(G4String fName);
    ~DetectorConstruction();
    
    G4VPhysicalVolume* Construct();
    static G4double GetGeometryParameter(const char *szParameter);
    // function needed by DetectorMessenger
    void SetSourcePos(G4double x)    { m_hSourcePosition = x; }
    void SetNaIPos(G4double x)    { m_hNaIPosition = x; }

    
    
private:
    void DefineMaterials();
    void DefineGeometryParameters();
    
    void ConstructLaboratory();
    void ConstructXAMS();
    void ConstructCollimatorSystem();
    // define the sensitive detectors
    void ConstructSD();
    void PrintGeometryInformation();
    
    void MakeDetectorPlots();
    void StoreMaterialParameters();
    void StoreGeometryParameters();
    
    
    // == LOGICAL VOLUMES ==
    G4LogicalVolume *m_pMotherLogicalVolume;
    G4LogicalVolume *m_pLabLogicalVolume;
    
    // Collimator setup
    G4LogicalVolume *m_pNaI_crystal_LogicalVolume;
    G4LogicalVolume *m_pPbBlock_LogicalVolume;
    G4LogicalVolume *m_pCollimatorHole_LogicalVolume;
    
    // Cryostat + TPC
    G4LogicalVolume *m_pOuterCryostat_LogicalVolume;
    G4LogicalVolume *m_pVacuum_LogicalVolume;
    G4LogicalVolume *m_pInnerCryostat_LogicalVolume;
    G4LogicalVolume *m_pLXeOut_LogicalVolume;
    G4LogicalVolume *m_pTeflon_LogicalVolume;
    G4LogicalVolume *m_pLXe_LogicalVolume;
    
    // Source
    G4LogicalVolume *m_pSourceDisk_LogicalVolume;
    G4LogicalVolume *m_pSourceCore_LogicalVolume;
    


    // == PHYSICAL VOLUMES ==
    G4VPhysicalVolume *m_pLabPhysicalVolume;

    // Collimator setup
    G4VPhysicalVolume *m_pNaI_crystal_PhysicalVolume;
    G4VPhysicalVolume *m_pPbBlock_PhysicalVolume;
    G4VPhysicalVolume *m_pCollimatorHole_PhysicalVolume;
    
    // Cryostat + TPC
    G4VPhysicalVolume *m_pOuterCryostat_PhysicalVolume;
    G4VPhysicalVolume *m_pVacuum_PhysicalVolume;
    G4VPhysicalVolume *m_pInnerCryostat_PhysicalVolume;
    G4VPhysicalVolume *m_pLXeOut_PhysicalVolume;
    G4VPhysicalVolume *m_pTeflon_PhysicalVolume;
    G4VPhysicalVolume *m_pLXe_PhysicalVolume;
    
    // Source
    G4VPhysicalVolume *m_pSourceDisk_PhysicalVolume;
    G4VPhysicalVolume *m_pSourceCore_PhysicalVolume;
    
    
    static map<G4String, G4double> m_hGeometryParameters;
    
    DetectorMessenger *m_pDetectorMessenger;
    G4double           m_hSourcePosition;
    G4double           m_hNaIPosition;

    
    
    // ROOT stuff
    TFile      *_fGeom;
    G4String    detRootFile;
    TDirectory *_detector;
    
};

#endif // __DETECTORCONSTRUCTION_H__

