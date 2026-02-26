#include "MRBDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4SDManager.hh"

#include "MRBDWCSD.hh"
#include "MRBCaloSD.hh"
#include "MRBWendiSD.hh"
#include "MRBEventAction.hh"

MRBDetectorConstruction::MRBDetectorConstruction()
: G4VUserDetectorConstruction(), fLogicMRB(nullptr), fSolidShield(nullptr), fMessenger(nullptr)
{ 
    // Initialize custom UI commands for the detector setup
    DefineCommands();
}

MRBDetectorConstruction::~MRBDetectorConstruction()
{ 
    delete fMessenger;
}

void MRBDetectorConstruction::DefineCommands()
{
    // Define the /MRB/setup/shieldConfig UI command for dynamic material swapping
    fMessenger = new G4GenericMessenger(this, "/MRB/setup/", "Target setup control");
    auto& configCmd = fMessenger->DeclareMethod("shieldConfig", &MRBDetectorConstruction::SetShieldConfig, "Select Material and auto-adjust thickness.");
    configCmd.SetParameterName("material", false);
    configCmd.SetStates(G4State_PreInit, G4State_Idle);
}

void MRBDetectorConstruction::SetShieldConfig(G4String materialName)
{
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* pttoMaterial = nist->FindOrBuildMaterial(materialName);
    
    if (pttoMaterial && fLogicMRB && fSolidShield) {
        // Swap the logical volume's material
        fLogicMRB->SetMaterial(pttoMaterial);
        
        // --- Areal Density Normalization ---
        // Dynamically recalculates the physical thickness of the target to maintain 
        // an exact areal density of 13.75 g/cm2, ensuring a mass-efficient comparison.
        G4double targetArealDensity = 13.75 * (g/cm2);
        G4double materialDensity = pttoMaterial->GetDensity();
        G4double newThickness = targetArealDensity / materialDensity;
        
        // Update the solid volume's half-length (Z-dimension)
        fSolidShield->SetZHalfLength(newThickness / 2.0);
        
        G4cout << "\n==================================================" << G4endl;
        G4cout << "--- SHIELD CONFIG UPDATED ---" << G4endl;
        G4cout << "Material: " << materialName << G4endl;
        G4cout << "Thickness updated to: " << newThickness / cm << " cm" << G4endl;
        G4cout << "==================================================\n" << G4endl;

        // Notify the RunManager that the geometry and physics need updating
        G4RunManager::GetRunManager()->GeometryHasBeenModified(); 
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    } else {
        G4cerr << "Error: Material '" << materialName << "' not found!" << G4endl;
    }
}

G4VPhysicalVolume* MRBDetectorConstruction::Construct()
{
    // Fetch standard materials from the NIST database
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* al_mat = nist->FindOrBuildMaterial("G4_Al");
    G4Material* pb_mat = nist->FindOrBuildMaterial("G4_Pb");
    G4Material* lead_glass = nist->FindOrBuildMaterial("G4_GLASS_LEAD");

    // Define custom gas mixtures for detectors
    G4Material* argon = nist->FindOrBuildMaterial("G4_Ar");
    G4Material* co2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    G4Material* dwc_gas = new G4Material("DWC_Gas", 1.55*mg/cm3, 2);
    dwc_gas->AddMaterial(argon, 70*perCent);
    dwc_gas->AddMaterial(co2, 30*perCent);

    // Fetch elements required for custom bio-composites
    G4Element* elO  = nist->FindOrBuildElement("O");
    G4Element* elSi = nist->FindOrBuildElement("Si");
    G4Element* elFe = nist->FindOrBuildElement("Fe");
    G4Element* elC  = nist->FindOrBuildElement("C");
    G4Element* elAl = nist->FindOrBuildElement("Al");
    G4Element* elCa = nist->FindOrBuildElement("Ca");
    G4Element* elMg = nist->FindOrBuildElement("Mg");
    G4Element* elH  = nist->FindOrBuildElement("H");
    G4Element* elN  = nist->FindOrBuildElement("N");

    // Define MRB 80/20 configuration (80% Regolith, 20% Biopolymer)
    G4Material* mrb_8020_mat = new G4Material("MRB_8020", 1.10 * g/cm3, 9);
    mrb_8020_mat->AddElement(elO, 43.6*perCent); mrb_8020_mat->AddElement(elSi, 16.8*perCent);
    mrb_8020_mat->AddElement(elFe, 10.4*perCent); mrb_8020_mat->AddElement(elC, 9.2*perCent);
    mrb_8020_mat->AddElement(elAl, 6.4*perCent); mrb_8020_mat->AddElement(elCa, 6.4*perCent);
    mrb_8020_mat->AddElement(elMg, 4.8*perCent); mrb_8020_mat->AddElement(elH, 1.3*perCent);
    mrb_8020_mat->AddElement(elN, 1.1*perCent);

    // Define MRB 60/40 configuration (60% Regolith, 40% Biopolymer)
    G4Material* mrb_6040_mat = new G4Material("MRB_6040", 0.87 * g/cm3, 9);
    mrb_6040_mat->AddElement(elO, 43.2*perCent); mrb_6040_mat->AddElement(elC, 18.4*perCent);
    mrb_6040_mat->AddElement(elSi, 12.6*perCent); mrb_6040_mat->AddElement(elFe, 7.8*perCent);
    mrb_6040_mat->AddElement(elAl, 4.8*perCent); mrb_6040_mat->AddElement(elCa, 4.8*perCent);
    mrb_6040_mat->AddElement(elMg, 3.6*perCent); mrb_6040_mat->AddElement(elH, 2.6*perCent);
    mrb_6040_mat->AddElement(elN, 2.2*perCent);

    // Define HDPE Control Material
    G4Material* hdpe_mat = new G4Material("HDPE_Control", 0.97 * g/cm3, 2);
    hdpe_mat->AddElement(elH, 14.37*perCent); hdpe_mat->AddElement(elC, 85.63*perCent);

    // Define Helium-3 Gas for the WENDI Neutron Detector
    G4Isotope* he3Iso = new G4Isotope("He3", 2, 3, 3.016*g/mole);
    G4Element* he3El = new G4Element("Helium3", "He3", 1);
    he3El->AddIsotope(he3Iso, 100.*perCent);
    G4Material* wendi_gas = new G4Material("He3_Gas", 0.0001346*g/cm3, 1, kStateGas, 293.15*kelvin, 1*atmosphere);
    wendi_gas->AddElement(he3El, 1);

    // --- Geometry Construction ---

    // 1. Experimental Hall (World Volume)
    G4double world_size = 3.0 * m;
    G4Box* solidWorld = new G4Box("World", world_size/2, world_size/2, world_size/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

    // 2. Spacecraft Aluminum Hull (Front of the target)
    G4double al_thickness = 10.0 * mm;
    G4Box* solidAl = new G4Box("AluminumHull", 10*cm, 10*cm, al_thickness/2);
    G4LogicalVolume* logicAl = new G4LogicalVolume(solidAl, al_mat, "AluminumHull");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAl, "AluminumHull", logicWorld, false, 0, true);

    // 3. Shielding Material (Dynamically sizable MRB/HDPE/Pb target)
    G4double initial_thickness = 12.5 * cm;
    G4double shield_z_pos = (al_thickness/2) + (initial_thickness/2); 
    fSolidShield = new G4Box("TestShield", 10*cm, 10*cm, initial_thickness/2);
    fLogicMRB = new G4LogicalVolume(fSolidShield, mrb_8020_mat, "TestShield");
    new G4PVPlacement(0, G4ThreeVector(0, 0, shield_z_pos), fLogicMRB, "TestShield", logicWorld, false, 0, true);

    G4double shield_back_face_z = shield_z_pos + (initial_thickness / 2.0);

    // 4. Delay Wire Chambers (DWCs) for on-axis charged particle tracking
    G4Box* solidDWC = new G4Box("DWC", 5.0*cm, 5.0*cm, 0.5*cm);
    G4LogicalVolume* logicDWC = new G4LogicalVolume(solidDWC, dwc_gas, "DWC");
    // Place 3 DWCs downstream of the target
    new G4PVPlacement(0, G4ThreeVector(0, 0, shield_back_face_z + 20*cm), logicDWC, "DWC0", logicWorld, false, 0, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, shield_back_face_z + 35*cm), logicDWC, "DWC1", logicWorld, false, 1, true);
    new G4PVPlacement(0, G4ThreeVector(0, 0, shield_back_face_z + 50*cm), logicDWC, "DWC2", logicWorld, false, 2, true);

    // 5. 4x4 Lead-Glass Calorimeter Array for energy deposition measurement
    G4Box* solidCalo = new G4Box("CaloBlock", 5.0*cm, 5.0*cm, 18.5*cm);
    G4LogicalVolume* logicCalo = new G4LogicalVolume(solidCalo, lead_glass, "CaloBlock");
    G4double calo_z_pos = shield_back_face_z + 80*cm + 18.5*cm; 
    G4int copyNo = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            new G4PVPlacement(0, G4ThreeVector((i - 1.5)*10*cm, (j - 1.5)*10*cm, calo_z_pos), logicCalo, "Calorimeter", logicWorld, false, copyNo++, true);
        }
    }

    // 6. WENDI Detector for off-axis neutral particle (neutron) counting
    G4Tubs* solidWendi = new G4Tubs("WENDI", 0., 10.0*cm, 12.5*cm, 0., 360.*deg);
    G4LogicalVolume* logicWendi = new G4LogicalVolume(solidWendi, wendi_gas, "WENDI");
    new G4PVPlacement(0, G4ThreeVector(25.*cm, 0, shield_back_face_z + 30.*cm), logicWendi, "WENDI_Phys", logicWorld, false, 0, true);

    return physWorld;
}

void MRBDetectorConstruction::ConstructSDandField()
{
    // Assign Sensitive Detectors to logical volumes to score hits
    
    // Assign DWC Sensitive Detector
    MRBDWCSD* dwcSector = new MRBDWCSD("DWC_SD");
    G4SDManager::GetSDMpointer()->AddNewDetector(dwcSector);
    SetSensitiveDetector("DWC", dwcSector);

    // Assign Calorimeter Sensitive Detector
    MRBCaloSD* caloSector = new MRBCaloSD("Calo_SD");
    G4SDManager::GetSDMpointer()->AddNewDetector(caloSector);
    SetSensitiveDetector("CaloBlock", caloSector); 

    // Assign WENDI Sensitive Detector, linked to the EventAction to register neutron counts
    const MRBEventAction* eventAction = static_cast<const MRBEventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
    MRBWendiSD* wendiSector = new MRBWendiSD("WENDI_SD", const_cast<MRBEventAction*>(eventAction));
    G4SDManager::GetSDMpointer()->AddNewDetector(wendiSector);
    SetSensitiveDetector("WENDI", wendiSector);
}

}
