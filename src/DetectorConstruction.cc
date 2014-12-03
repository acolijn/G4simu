#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Cons.hh>
#include <G4Sphere.hh>
#include <G4Torus.hh>
#include <G4Orb.hh>
#include <G4Polyhedra.hh>
#include <G4Polycone.hh>
#include <G4Ellipsoid.hh>
#include <G4ExtrudedSolid.hh>  // SERENA
//#include <G4Trd.hh>
#include <G4UnionSolid.hh>
//#include <G4IntersectionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4PVParameterised.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4VisAttributes.hh>
#include <G4Colour.hh>
#include <globals.hh>

#include <vector>
#include <numeric>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cassert>

using std::vector;
using std::stringstream;
using std::max;

#include "DetectorMessenger.hh"

#include "SensitiveDetector.hh"
#include "DetectorConstruction.hh"

#include "G4PhysicalVolumeStore.hh"

#include "TFile.h"
#include "TParameter.h"

using namespace CLHEP;

map<G4String, G4double> DetectorConstruction::m_hGeometryParameters;

DetectorConstruction::DetectorConstruction(G4String fName)
{
    m_pDetectorMessenger = new DetectorMessenger(this);
    detRootFile = fName;
    m_hSourcePosition = 15*cm;
    m_hNaIPosition = 25*cm;

}

DetectorConstruction::~DetectorConstruction()
{
    delete m_pDetectorMessenger;
}


G4VPhysicalVolume*
DetectorConstruction::Construct()
{
    
    DefineMaterials();
    
    DefineGeometryParameters();
    
    // construction of the geometry
    ConstructLaboratory();
    ConstructXAMS();
    ConstructCollimatorSystem();
    // make the appropriate components active
    ConstructSD();
    
    PrintGeometryInformation();
    
    MakeDetectorPlots();
    
    return m_pLabPhysicalVolume;
}

void
DetectorConstruction::DefineMaterials()
{
    G4NistManager* pNistManager = G4NistManager::Instance();
    
    //================================== elements ===================================
    pNistManager->FindOrBuildElement("U");
    G4Element *Xe = new G4Element("Xenon",     "Xe", 54., 131.293*g/mole);
    G4Element *H  = new G4Element("Hydrogen",  "H",  1.,  1.0079*g/mole);
    G4Element *C  = new G4Element("Carbon",    "C",  6.,  12.011*g/mole);
    G4Element *N  = new G4Element("Nitrogen",  "N",  7.,  14.007*g/mole);
    G4Element *O  = new G4Element("Oxygen",    "O",  8.,  15.999*g/mole);
    G4Element *F  = new G4Element("Fluorine",  "F",  9.,  18.998*g/mole);
    //	G4Element *Al = new G4Element("Aluminium", "Al", 13., 26.982*g/mole);
    G4Element *Si = new G4Element("Silicon",   "Si", 14., 28.086*g/mole);
    G4Element *Cr = new G4Element("Chromium",  "Cr", 24., 51.996*g/mole);
    G4Element *Mn = new G4Element("Manganese", "Mn", 25., 54.938*g/mole);
    G4Element *Fe = new G4Element("Iron",      "Fe", 26., 55.85*g/mole);
    G4Element *Ni = new G4Element("Nickel",    "Ni", 28., 58.693*g/mole);
    //  G4Element *Cu = new G4Element("Copper",    "Cu", 29., 63.546*g/mole);
    
    //	G4Element *Co = pNistManager->FindOrBuildElement("Co");
    //	G4Element *Ti = pNistManager->FindOrBuildElement("Ti");
    //	G4Element *Mo = pNistManager->FindOrBuildElement("Mo");
    
    G4Element *Na = pNistManager->FindOrBuildElement("Na");
    G4Element *I  = pNistManager->FindOrBuildElement("I");
    
    
    //================================== materials ==================================
    
    //------------------------------------- air -------------------------------------
    G4Material *Air = pNistManager->FindOrBuildMaterial("G4_AIR");
    //G4Material *Air = G4Material::GetMaterial("G4_AIR");
    
    
    //----------------------------------- vacuum ------------------------------------
    G4Material *Vacuum = new G4Material("Vacuum", 1.e-20*g/cm3, 2, kStateGas);
    Vacuum->AddElement(N, 0.755);
    Vacuum->AddElement(O, 0.245);
    
    //------------------------------------ water ------------------------
    G4Material *Water = new G4Material("Water", 1.*g/cm3, 2, kStateLiquid);
    Water->AddElement(H, 2);
    Water->AddElement(O, 1);
    
    //------------------------------------ NaI   ------------------------
    G4Material *NaI = new G4Material("NaI", 3.67*g/cm3, 2, kStateSolid);
    NaI->AddElement(Na, 1);
    NaI->AddElement( I, 1);
    
    //------------------------------------ plastic -----------------------------------
    G4Material* PE = new G4Material("PE", 1.0*g/cm3, 2, kStateSolid);
    PE->AddElement(C, 2);
    PE->AddElement(H, 4);
    
    
    //------------------------------------ copper -----------------------------------
    //	G4Material *Copper = new G4Material("Copper", 8.92*g/cm3, 1);
    //	Copper->AddElement(Cu, 1);
    
    //	G4double pdCopperPhotonMomentum[iNbEntries] = {1.91*eV, 6.98*eV, 7.05*eV};//{6.91*eV, 6.98*eV, 7.05*eV};
    //	G4double pdCopperReflectivity[iNbEntries]   = {0.15,    0.2,     0.15};
    //	G4MaterialPropertiesTable *pCopperPropertiesTable = new G4MaterialPropertiesTable();
    
    //	pCopperPropertiesTable->AddProperty("REFLECTIVITY", pdCopperPhotonMomentum, pdCopperReflectivity, iNbEntries);
    //	Copper->SetMaterialPropertiesTable(pCopperPropertiesTable);
    //-------------------------------- liquid xenon ---------------------------------
    G4Material *LXe = new G4Material("LXe", 2.85*g/cm3, 1, kStateLiquid, 168.15*kelvin, 1.5*atmosphere);
    LXe->AddElement(Xe, 1);
    
    const G4int iNbEntries = 3;
    
    G4double pdLXePhotonMomentum[iNbEntries]   = {6.91*eV, 6.98*eV, 7.05*eV};
    G4double pdLXeScintillation[iNbEntries]    = {0.1,     1.0,     0.1};
    G4double pdLXeRefractiveIndex[iNbEntries]  = {1.63,    1.61,    1.58};
    G4double pdLXeAbsorbtionLength[iNbEntries] = {100.*cm, 100.*cm, 100.*cm};
    G4double pdLXeScatteringLength[iNbEntries] = {30.*cm,  30.*cm,  30.*cm};
    
    G4MaterialPropertiesTable *pLXePropertiesTable = new G4MaterialPropertiesTable();
    
    pLXePropertiesTable->AddProperty("FASTCOMPONENT", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries);
    pLXePropertiesTable->AddProperty("SLOWCOMPONENT", pdLXePhotonMomentum, pdLXeScintillation, iNbEntries);
    pLXePropertiesTable->AddProperty("RINDEX", pdLXePhotonMomentum, pdLXeRefractiveIndex, iNbEntries);
    pLXePropertiesTable->AddProperty("ABSLENGTH", pdLXePhotonMomentum, pdLXeAbsorbtionLength, iNbEntries);
    pLXePropertiesTable->AddProperty("RAYLEIGH", pdLXePhotonMomentum, pdLXeScatteringLength, iNbEntries);
    
    pLXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0./keV);
    pLXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 0);
    pLXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3.*ns);
    pLXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27.*ns);
    pLXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0);
    
    LXe->SetMaterialPropertiesTable(pLXePropertiesTable);
    
    //-------------------------------- gaseous xenon --------------------------------
    G4Material *GXe = new G4Material("GXe", 0.005887*g/cm3, 1, kStateGas, 173.15*kelvin, 1.5*atmosphere);
    GXe->AddElement(Xe, 1);
    
    G4double pdGXePhotonMomentum[iNbEntries]   = {6.91*eV, 6.98*eV, 7.05*eV};
    G4double pdGXeScintillation[iNbEntries]    = {0.1,     1.0,     0.1};
    G4double pdGXeRefractiveIndex[iNbEntries]  = {1.00,    1.00,    1.00};
    G4double pdGXeAbsorbtionLength[iNbEntries] = {100*m,   100*m,   100*m};
    G4double pdGXeScatteringLength[iNbEntries] = {100*m,   100*m,   100*m};
    
    G4MaterialPropertiesTable *pGXePropertiesTable = new G4MaterialPropertiesTable();
    
    pGXePropertiesTable->AddProperty("FASTCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries);
    pGXePropertiesTable->AddProperty("SLOWCOMPONENT", pdGXePhotonMomentum, pdGXeScintillation, iNbEntries);
    pGXePropertiesTable->AddProperty("RINDEX", pdGXePhotonMomentum, pdGXeRefractiveIndex, iNbEntries);
    pGXePropertiesTable->AddProperty("ABSLENGTH", pdGXePhotonMomentum, pdGXeAbsorbtionLength, iNbEntries);
    pGXePropertiesTable->AddProperty("RAYLEIGH", pdGXePhotonMomentum, pdGXeScatteringLength, iNbEntries);
    
    pGXePropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 0./(keV));
    pGXePropertiesTable->AddConstProperty("RESOLUTIONSCALE", 0);
    pGXePropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3.*ns);
    pGXePropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 27.*ns);
    pGXePropertiesTable->AddConstProperty("YIELDRATIO", 1.0);
    
    GXe->SetMaterialPropertiesTable(pGXePropertiesTable);
    
    //------------------------------- stainless steel -------------------------------
    G4Material *SS304LSteel = new G4Material("SS304LSteel", 8.00*g/cm3, 5, kStateSolid);
    SS304LSteel->AddElement(Fe, 0.65);
    SS304LSteel->AddElement(Cr, 0.20);
    SS304LSteel->AddElement(Ni, 0.12);
    SS304LSteel->AddElement(Mn, 0.02);
    SS304LSteel->AddElement(Si, 0.01);
    
    
    // ****** BY AUKE-PC ****** --------------------------------------------------------
    
    //
    // AISI 316Ti steel / UNS S 31635
    //
    // 1. Take the maximum specified fraction of each component.
    //    See http://www.atimetals.com/ludlum/Documents/316ti.pdf
    // 2. Leave out tracer elements <0.05% of Phosphor and Sulphur (justifiable?)
    //
    // A.P. Colijn 26-07-2011
    //
    // G4double fractionmass;
    //G4int    ncomponent;
    //  G4Material *SS316Ti = new G4Material("SS316Ti", 8.00*g/cm3, ncomponent=8, kStateSolid);
    //  SS316Ti->AddElement(Cr, fractionmass=18.00*perCent);
    //  SS316Ti->AddElement(Ni, fractionmass=14.00*perCent);
    //  SS316Ti->AddElement(Mo, fractionmass= 3.00*perCent);
    //  SS316Ti->AddElement(Mn, fractionmass= 2.00*perCent);
    //  SS316Ti->AddElement(C , fractionmass= 0.80*perCent);
    //  SS316Ti->AddElement(Si, fractionmass= 0.75*perCent);
    //  SS316Ti->AddElement(Ti, fractionmass= 0.70*perCent);
    //  SS316Ti->AddElement(Fe, fractionmass=60.75*perCent);
    
    //------------------------------------ teflon -----------------------------------
    G4Material* Teflon = new G4Material("Teflon", 2.2*g/cm3, 2, kStateSolid);
    Teflon->AddElement(C, 0.240183);
    Teflon->AddElement(F, 0.759817);
    
    //	G4double pdTeflonPhotonMomentum[iNbEntries]  = {6.91*eV, 6.98*eV, 7.05*eV};
    //	G4double pdTeflonRefractiveIndex[iNbEntries] = {1.63,    1.61,    1.58};
    //	G4double pdTeflonReflectivity[iNbEntries]    = {0.95,    0.95,    0.95};
    //	G4double pdTeflonSpecularLobe[iNbEntries]    = {0.01,    0.01,    0.01};
    //	G4double pdTeflonSpecularSpike[iNbEntries]   = {0.01,    0.01,    0.01};
    //	G4double pdTeflonBackscatter[iNbEntries]     = {0.01,    0.01,    0.01};
    //	G4double pdTeflonEfficiency[iNbEntries]      = {1.0,     1.0,     1.0};
    //	G4MaterialPropertiesTable *pTeflonPropertiesTable = new G4MaterialPropertiesTable();
    //
    //	pTeflonPropertiesTable->AddProperty("RINDEX", pdTeflonPhotonMomentum, pdTeflonRefractiveIndex, iNbEntries);
    //	pTeflonPropertiesTable->AddProperty("REFLECTIVITY", pdTeflonPhotonMomentum, pdTeflonReflectivity, iNbEntries);
    //	pTeflonPropertiesTable->AddProperty("SPECULARLOBECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularLobe, iNbEntries);
    //	pTeflonPropertiesTable->AddProperty("SPECULARSPIKECONSTANT", pdTeflonPhotonMomentum, pdTeflonSpecularSpike, iNbEntries);
    //	pTeflonPropertiesTable->AddProperty("BACKSCATTERCONSTANT", pdTeflonPhotonMomentum, pdTeflonBackscatter, iNbEntries);
    //	pTeflonPropertiesTable->AddProperty("EFFICIENCY", pdTeflonPhotonMomentum, pdTeflonEfficiency, iNbEntries);
    //	Teflon->SetMaterialPropertiesTable(pTeflonPropertiesTable);
    
    
    //---------------------------------- titanium ----------------------------------
    //  G4Material *Titanium = pNistManager->FindOrBuildMaterial("G4_Ti");
    //	G4Material *Titanium = new G4Material("Titanium", 4.506*g/cm3, 1, kStateSolid);
    //	Titanium->AddElement(Ti, 1);
    
    //------------------------------------ acrylic -----------------------------------
    G4Material *Acrylic = new G4Material("Acrylic", 1.18*g/cm3, 3, kStateSolid, 168.15*kelvin, 1.5*atmosphere);
    Acrylic->AddElement(C,5);
    Acrylic->AddElement(H,8);
    Acrylic->AddElement(O,2);
    
}

void
DetectorConstruction::DefineGeometryParameters()
{
    //================================== Laboratory =================================
    m_hGeometryParameters["LabSize"] = 1.*m;
    //================================== NaI crystal ================================
    m_hGeometryParameters["NaI_crystal_R"] = 2*cm;
    m_hGeometryParameters["NaI_crystal_Z"] = 4.0*cm;
    //================================== Disk source ================================
    m_hGeometryParameters["SourceDisk_R"] = 1.5*cm;
    m_hGeometryParameters["SourceDisk_Z"] = 5.0*mm;
    // little sphere in middle of disk
    m_hGeometryParameters["SourceCore_R"] = 1.0*mm;
    
    //================================== XAMS detector ==============================
    m_hGeometryParameters["OuterCryostat_Rout"] = 125*mm;
    m_hGeometryParameters["OuterCryostat_dZ"] = 250*mm;
    m_hGeometryParameters["OuterCryostat_Thickness"] = 3.*mm;
    //============
    m_hGeometryParameters["InnerCryostat_Rout"] = 73.97*mm;
    m_hGeometryParameters["InnerCryostat_dZ"] = 200*mm;
    m_hGeometryParameters["InnerCryostat_Thickness"] = 3.*mm;
    //============
    m_hGeometryParameters["Teflon_Rout"] = 68.49*mm;
    m_hGeometryParameters["Teflon_Thickness"] = 46.16*mm;
}

G4double
DetectorConstruction::GetGeometryParameter(const char *szParameter)
{
    return m_hGeometryParameters[szParameter];
}

void
DetectorConstruction::ConstructLaboratory()
{
    //================================== Laboratory =================================
    const G4double dLabHalfSize = 0.5*GetGeometryParameter("LabSize");
    
    //G4Material *Air = G4Material::GetMaterial("G4_AIR");
    G4Material *Air = G4Material::GetMaterial("G4_AIR");
    
    G4Box *pLabBox         = new G4Box("LabBox", dLabHalfSize, dLabHalfSize, dLabHalfSize);
    m_pLabLogicalVolume    = new G4LogicalVolume(pLabBox, Air, "LabVolume", 0, 0, 0);
    m_pLabPhysicalVolume   = new G4PVPlacement(0, G4ThreeVector(), m_pLabLogicalVolume, "Lab", 0, false, 0);
    m_pMotherLogicalVolume = m_pLabLogicalVolume;
    
    m_pLabLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);
}

void
DetectorConstruction::ConstructSD()
{
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    // make LXe sensitive....
    G4VSensitiveDetector* LXe_SD = new SensitiveDetector(SDname="/LXe");
    SDman->AddNewDetector(LXe_SD);
    m_pLXe_LogicalVolume->SetSensitiveDetector(LXe_SD);
    
    // make NaI sensitive
    G4VSensitiveDetector* NaI_SD = new SensitiveDetector(SDname="/NaI");
    SDman->AddNewDetector(NaI_SD);
    m_pNaI_crystal_LogicalVolume->SetSensitiveDetector(NaI_SD);
}

void
DetectorConstruction::ConstructXAMS()
{
    // get the materials from the materials DB
    G4Material *LXe    = G4Material::GetMaterial("LXe");
    G4Material *SS304  = G4Material::GetMaterial("SS304LSteel");
    G4Material *Teflon = G4Material::GetMaterial("Teflon");
    G4Material *Vacuum = G4Material::GetMaterial("Vacuum");
    
    //================================== NaI crystal =================================
    
    //    const G4double dLXeRadius =     GetGeometryParameter("LXe_R");
    //    const G4double dLXeHalfZ  = 0.5*GetGeometryParameter("LXe_Z");
    
    
    G4double zmax = 400*mm;
    
    // make and place outer cryostat
    G4Tubs *pOuterCryostat_TUBS = new G4Tubs("OuterCryostatTUBS", 0.*cm,
                                             GetGeometryParameter("OuterCryostat_Rout"),zmax/2., 0.*deg, 360.*deg);
    m_pOuterCryostat_LogicalVolume  = new G4LogicalVolume(pOuterCryostat_TUBS, SS304, "OuterCryostat", 0, 0, 0);
    m_pOuterCryostat_PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0,0,0), m_pOuterCryostat_LogicalVolume,
                                                        "OuterCryostat", m_pMotherLogicalVolume, false, 0);
    // make and place the insulation vacuum
    G4Tubs *pVacuum_TUBS = new G4Tubs("VacuumTUBS", 0.*cm,
                                      GetGeometryParameter("OuterCryostat_Rout")-GetGeometryParameter("OuterCryostat_Thickness"),
                                      zmax/2. - 1.0*mm, 0.*deg, 360.*deg);
    m_pVacuum_LogicalVolume  = new G4LogicalVolume(pVacuum_TUBS, Vacuum, "Vacuum", 0, 0, 0);
    m_pVacuum_PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0,0,0), m_pVacuum_LogicalVolume,
                                                 "Vacuum", m_pOuterCryostat_LogicalVolume, false, 0);
    m_pVacuum_LogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);
    
    // make and place the inner cryostat
    G4Tubs *pInnerCryostat_TUBS = new G4Tubs("InnerCryostatTUBS", 0.*cm,
                                             GetGeometryParameter("InnerCryostat_Rout"),
                                             zmax/2. - 2.0*mm, 0.*deg, 360.*deg);
    m_pInnerCryostat_LogicalVolume  = new G4LogicalVolume(pInnerCryostat_TUBS, SS304, "InnerCryostat", 0, 0, 0);
    m_pInnerCryostat_PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0,0,0), m_pInnerCryostat_LogicalVolume,
                                                        "InnerCryostat", m_pVacuum_LogicalVolume, false, 0);
    
    // make and place the outer LXe layer
    G4Tubs *pLXeOut_TUBS = new G4Tubs("pLXeOut_TUBS", 0.*cm,
                                      GetGeometryParameter("InnerCryostat_Rout")-GetGeometryParameter("InnerCryostat_Thickness"),
                                      zmax/2. - 3.0*mm, 0.*deg, 360.*deg);
    m_pLXeOut_LogicalVolume  = new G4LogicalVolume(pLXeOut_TUBS, LXe, "LXeOut", 0, 0, 0);
    m_pLXeOut_PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0,0,0), m_pLXeOut_LogicalVolume,
                                                 "LXeOut", m_pInnerCryostat_LogicalVolume, false, 0);
    
    // make and place the Teflon layer
    G4Tubs *pTeflon_TUBS = new G4Tubs("pTeflon_TUBS", 0.*cm,
                                      GetGeometryParameter("Teflon_Rout"),
                                      zmax/2. - 4.0*mm, 0.*deg, 360.*deg);
    m_pTeflon_LogicalVolume  = new G4LogicalVolume(pTeflon_TUBS, Teflon, "Teflon", 0, 0, 0);
    m_pTeflon_PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0,0,0), m_pTeflon_LogicalVolume,
                                                 "Teflon", m_pLXeOut_LogicalVolume, false, 0);
    
    // make and place LXe
    G4Tubs *pLXe_TUBS     = new G4Tubs("LXe_TUBS", 0.*cm,
                                       GetGeometryParameter("Teflon_Rout")-GetGeometryParameter("Teflon_Thickness"),
                                       zmax/2-5.*mm, 0.*deg, 360.*deg);
    m_pLXe_LogicalVolume  = new G4LogicalVolume(pLXe_TUBS, LXe, "LXe_target", 0, 0, 0);
    m_pLXe_PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0,0,0), m_pLXe_LogicalVolume,
                                              "LXe_target", m_pTeflon_LogicalVolume, false, 0);
    
    
    // visibility
    G4Colour hTitaniumColor(0.600, 0.600, 0.600, 0.1);
    G4VisAttributes *pTitaniumVisAtt = new G4VisAttributes(hTitaniumColor);
    pTitaniumVisAtt->SetVisibility(true);
    m_pOuterCryostat_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);
    m_pInnerCryostat_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);
    
    G4Colour hTeflonColor(0.700, 0.700, 0.700, 0.2);
    G4VisAttributes *pTeflonVisAtt = new G4VisAttributes(hTeflonColor);
    pTeflonVisAtt->SetVisibility(true);
    m_pTeflon_LogicalVolume->SetVisAttributes(pTeflonVisAtt);
    
    G4Colour hLXeColor(0.000, 0.000, 0.400, 0.3);
    G4VisAttributes *pLXeVisAtt = new G4VisAttributes(hLXeColor);
    pLXeVisAtt->SetVisibility(true);
    m_pLXeOut_LogicalVolume->SetVisAttributes(pLXeVisAtt);
    m_pLXe_LogicalVolume->SetVisAttributes(pLXeVisAtt);
}

void
DetectorConstruction::ConstructCollimatorSystem()
{
    //================================== NaI crystal =================================
    const G4double dCrystalRadius =     GetGeometryParameter("NaI_crystal_R");
    const G4double dCrystalHalfZ  = 0.5*GetGeometryParameter("NaI_crystal_Z");
    
    G4Material *NaI = G4Material::GetMaterial("NaI");
    //    G4Material *Air = G4Material::GetMaterial("G4_AIR");
    G4Material *PE = G4Material::GetMaterial("PE");

    // rotation matrix for the NaI crystal
    G4RotationMatrix *pRot = new G4RotationMatrix();
    pRot->rotateY(90.*deg);
    
    // make and place one NaI crystal
    G4Tubs *pNaI_crystal          = new G4Tubs("NaI_crystal1", 0.*cm, dCrystalRadius, dCrystalHalfZ, 0.*deg, 360.*deg);
    m_pNaI_crystal_LogicalVolume  = new G4LogicalVolume(pNaI_crystal, NaI, "NaI_crystalTUBS", 0, 0, 0);
    m_pNaI_crystal_PhysicalVolume = new G4PVPlacement(pRot, G4ThreeVector(m_hNaIPosition,0,0), m_pNaI_crystal_LogicalVolume,
                                                      "NaI_crystal", m_pMotherLogicalVolume, false, 0);
    
    // make and place the source
    G4cout <<"DetectorConstruction::ConstructCollimatorSystem:: Source at "<<m_hSourcePosition/cm<<" cm" <<G4endl;
    G4Tubs *pSourceDisk          = new G4Tubs("SourceDisk", 0.*cm, GetGeometryParameter("SourceDisk_R"),
                                              GetGeometryParameter("SourceDisk_Z")/2,
                                              0.*deg, 360.*deg);
    m_pSourceDisk_LogicalVolume  = new G4LogicalVolume(pSourceDisk, PE, "SourceDisk", 0, 0, 0);
    m_pSourceDisk_PhysicalVolume = new G4PVPlacement(pRot, G4ThreeVector(m_hSourcePosition,0,0), m_pSourceDisk_LogicalVolume,
                                                     "SourceDisk", m_pMotherLogicalVolume, false, 0);
    
    G4Orb *pSourceCore        = new G4Orb("SourceCore", GetGeometryParameter("SourceCore_R"));
    m_pSourceCore_LogicalVolume  = new G4LogicalVolume(pSourceCore, PE, "SourceCore", 0, 0, 0);
    m_pSourceCore_PhysicalVolume = new G4PVPlacement(pRot, G4ThreeVector(0,0,0), m_pSourceCore_LogicalVolume,
                                                     "SourceCore", m_pSourceDisk_LogicalVolume, false, 0);
    
    // make and place the Pb collimator
    
    // visibility
    G4Colour hTitaniumColor(0.600, 0.600, 0.600, 0.1);
    G4VisAttributes *pTitaniumVisAtt = new G4VisAttributes(hTitaniumColor);
    pTitaniumVisAtt->SetVisibility(true);
    m_pNaI_crystal_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);
    m_pSourceDisk_LogicalVolume->SetVisAttributes(pTitaniumVisAtt);
}


void
DetectorConstruction::PrintGeometryInformation()
{
    //================================== Water =================================
    //	const G4double dWaterMass = m_pWaterLogicalVolume->GetMass(false, false)/(1000.*kg);
    //	G4cout << "Water Mass:                               " << dWaterMass << " ton" << G4endl << G4endl;
}

void DetectorConstruction::MakeDetectorPlots()
{
    _fGeom = new TFile(detRootFile,"RECREATE");
    _detector = _fGeom->mkdir("detector");
    
    // store properties of the materials that are used
    StoreMaterialParameters();
    // store geometry parameters
    StoreGeometryParameters();
    
    _fGeom->Write();
    _fGeom->Close();
    
}

void DetectorConstruction::StoreGeometryParameters()
{
    // TDirectory for storage of teh geometry parameters
    TDirectory *_geometry = _detector->mkdir("geometry");
    _geometry->cd();

    // XAMS cryostat - outer cryostat vessel
    TParameter<double> *OuterCryostat_Rout = new TParameter<double>("OuterCryostat_Rout",GetGeometryParameter("OuterCryostat_Rout")/mm);
    OuterCryostat_Rout->Write();
    
    TParameter<double> *OuterCryostat_dZ = new TParameter<double>("OuterCryostat_dZ",GetGeometryParameter("OuterCryostat_dZ")/mm);
    OuterCryostat_dZ->Write();
    
    TParameter<double> *OuterCryostat_Thickness = new TParameter<double>("OuterCryostat_Thickness",GetGeometryParameter("OuterCryostat_Thickness")/mm);
    OuterCryostat_Thickness->Write();
    
    // XAMS cryostat - inner cryostat vessel
    TParameter<double> *InnerCryostat_Rout = new TParameter<double>("InnerCryostat_Rout",GetGeometryParameter("InnerCryostat_Rout")/mm);
    InnerCryostat_Rout->Write();

    TParameter<double> *InnerCryostat_dZ = new TParameter<double>("InnerCryostat_dZ",GetGeometryParameter("InnerCryostat_dZ")/mm);
    InnerCryostat_dZ->Write();
    
    TParameter<double> *InnerCryostat_Thickness = new TParameter<double>("InnerCryostat_Thickness",GetGeometryParameter("InnerCryostat_Thickness")/mm);
    InnerCryostat_Thickness->Write();
    
    // XAMS  - inner cryostat vessel
    TParameter<double> *Teflon_Rout = new TParameter<double>("Teflon_Rout",GetGeometryParameter("Teflon_Rout")/mm);
    Teflon_Rout->Write();
    
    TParameter<double> *Teflon_Thickness = new TParameter<double>("Teflon_Thickness",GetGeometryParameter("Teflon_Thickness")/mm);
    Teflon_Thickness->Write();
    
    // NaI crystal
    TParameter<double> *NaI_R = new TParameter<double>("NaI_crystal_R",GetGeometryParameter("NaI_crystal_R")/mm);
    NaI_R->Write();
    TParameter<double> *NaI_Z = new TParameter<double>("NaI_crystal_Z",GetGeometryParameter("NaI_crystal_Z")/mm);
    NaI_Z->Write();
    TParameter<double> *NaI_pos = new TParameter<double>("NaI_crystal_Position",m_hNaIPosition/mm);
    NaI_pos->Write();

    // Source
    TParameter<double> *Source_R = new TParameter<double>("SourceDisk_R",GetGeometryParameter("SourceDisk_R")/mm);
    Source_R->Write();
    TParameter<double> *Source_Z = new TParameter<double>("SourceDisk_Z",GetGeometryParameter("SourceDisk_Z")/mm);
    Source_Z->Write();
    TParameter<double> *Source_pos = new TParameter<double>("SourceDisk_Position",m_hSourcePosition/mm);
    Source_pos->Write();
    
    _fGeom->cd();
}


void DetectorConstruction::StoreMaterialParameters()
{
    
    // make a list of materials for graphs
    G4int nmaterial = G4Material::GetNumberOfMaterials();
    G4cout <<"MakeDetectorPlots:: Number of materials = "<<nmaterial<<G4endl;
    
    TDirectory *_materials = _detector->mkdir("materials");
    _materials->cd();
    
    //  for(G4int imat=0; imat<(G4int)matNames.size(); imat++){
    vector<TDirectory*> matdirs;
    
    for(G4int imat=0; imat<nmaterial; imat++){
        G4Material *mat = G4NistManager::Instance()->GetMaterial(imat);
        G4String matname = mat->GetName();
        G4cout <<"MakeDetectorPlots:: "<<matname<<G4endl;
        G4double T   = mat->GetTemperature();
        G4double rho = mat->GetDensity();
        G4double P   = mat->GetPressure();
        
        matdirs.push_back(_materials->mkdir(matname));
        matdirs[imat]->cd();
        TParameter<double> *TemperaturePar = new TParameter<double>("Temperature",T);
        TemperaturePar->Write();
        TParameter<double> *DensityPar     = new TParameter<double>("Density",rho / (g/cm3));
        DensityPar->Write();
        TParameter<double> *PressurePar    = new TParameter<double>("Pressure",P / bar);
        PressurePar->Write();
        // disect the material
        size_t nele = mat->GetNumberOfElements();
        G4ElementVector *elems       = (G4ElementVector*)mat->GetElementVector();
        G4double        *fractionVec = (G4double*)mat->GetFractionVector();
        
        for(size_t iele=0; iele<nele; iele++){
            G4String elname = (*elems)[iele]->GetName();
            G4double frac   = fractionVec[iele];
            //      G4cout <<iele<<" elem = "<<(*elems)[iele]->GetName()<<" f = "<<fractionVec[iele]<<G4endl;
            char  elFrac[100];
            sprintf(elFrac,"f_%s",elname.c_str());
            TParameter<double> *_fracPar = new TParameter<double>((G4String)elFrac,frac);
            _fracPar->Write();
        }
        
        
        _materials->cd();
    }
    
    _fGeom->cd();
}
