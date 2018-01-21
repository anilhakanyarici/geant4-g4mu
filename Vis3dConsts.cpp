#include "Vis3dConsts.h"

#include <Geant4/G4NistManager.hh>
#include <Geant4/G4Box.hh>
#include <Geant4/G4RunManager.hh>
#include <Geant4/G4Cons.hh>
#include <Geant4/G4Orb.hh>
#include <Geant4/G4Sphere.hh>
#include <Geant4/G4Trd.hh>
#include <Geant4/G4LogicalVolume.hh>
#include <Geant4/G4PVPlacement.hh>
#include <Geant4/G4SystemOfUnits.hh>
#include <Geant4/G4EllipticalTube.hh>

#include "time.h"

#include "Scintillator.h"

#include "iostream"

Vis3dConsts::Vis3dConsts()
{

}

Vis3dConsts::~Vis3dConsts()
{

}

G4VPhysicalVolume *Vis3dConsts::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4Box *solidWorld = new G4Box("World", 1200, 1200, 1800);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, nist->FindOrBuildMaterial("G4_AIR"), "World");
    G4VPhysicalVolume *physicalWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    G4Orb *solidEnvelope = new G4Orb("Envelope", 1800);
    G4LogicalVolume *logicEnvelope = new G4LogicalVolume(solidEnvelope, nist->FindOrBuildMaterial("G4_AIR"), "Envelope");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicEnvelope, "Envelope", logicWorld, false, 0, true);

    G4EllipticalTube *tubeSolid = new G4EllipticalTube("Tube", 500, 800, 500);
    G4LogicalVolume *tubeLogic = new G4LogicalVolume(tubeSolid, nist->FindOrBuildMaterial("G4_U"), "Tube");
    G4RotationMatrix *rot = nullptr;//new G4RotationMatrix(0, 90 * deg, 0);
    new G4PVPlacement(rot, G4ThreeVector(), tubeLogic, "Tube", logicEnvelope, false, 0, true);

    Scintillator *scintillator1 = new Scintillator("Scintillator1", 800, 800, 1, nist->FindOrBuildMaterial("G4_TEFLON"));
    scintillator1->SetPlacement(nullptr, G4ThreeVector(0, 0, -1200), logicEnvelope);
    scintillator1->ConstructCells(100, 100);

    Scintillator *scintillator2 = new Scintillator("Scintillator2", 800, 800, 1, nist->FindOrBuildMaterial("G4_TEFLON"));
    scintillator2->SetPlacement(nullptr, G4ThreeVector(0, 0, 1200), logicEnvelope);
    scintillator2->ConstructCells(100, 100);

    return physicalWorld;
}
