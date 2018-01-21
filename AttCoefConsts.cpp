#include "AttCoefConsts.h"

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

AttCoefConsts::AttCoefConsts(G4String material)
{
    G4NistManager *nist = G4NistManager::Instance();
    this->mat = nist->FindOrBuildMaterial(material);
}

AttCoefConsts::AttCoefConsts(G4Material *material)
{
    this->mat = material;
}

AttCoefConsts::~AttCoefConsts()
{

}

G4VPhysicalVolume *AttCoefConsts::Construct()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4Box *solidWorld = new G4Box("World", 1200, 1200, 1800);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, nist->FindOrBuildMaterial("G4_AIR"), "World");
    G4VPhysicalVolume *physicalWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    G4Orb *solidEnvelope = new G4Orb("Envelope", 1800);
    G4LogicalVolume *logicEnvelope = new G4LogicalVolume(solidEnvelope, nist->FindOrBuildMaterial("G4_AIR"), "Envelope");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicEnvelope, "Envelope", logicWorld, false, 0, true);

    G4EllipticalTube *tubeSolid = new G4EllipticalTube("Tube", 500, 800, 500);
    G4LogicalVolume *tubeLogic = new G4LogicalVolume(tubeSolid, this->mat, "Tube");
    new G4PVPlacement(nullptr, G4ThreeVector(), tubeLogic, "Tube", logicEnvelope, false, 0, true);

    return physicalWorld;
}
