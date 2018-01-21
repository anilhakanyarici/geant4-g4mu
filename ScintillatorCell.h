#ifndef SCINTILLATORCELL_H
#define SCINTILLATORCELL_H

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

#include "map"

class Scintillator;

class ScintillatorCell
{
    static std::map<G4LogicalVolume*, ScintillatorCell*> _logicToCell;

    G4String _name;
    Scintillator *_owner;
    G4LogicalVolume *_logic;
    G4VSolid *_solid;

    G4ThreeVector _position;

public:
    ScintillatorCell(Scintillator *owner, const G4String &name, const G4ThreeVector &halfSizes, G4Material *material);

    inline G4LogicalVolume *GetLogic() const { return this->_logic; }
    inline G4ThreeVector GetPosition() const { return this->_position; }
    inline Scintillator *GetOwner() const { return this->_owner; }

    static ScintillatorCell *FindCell(G4LogicalVolume *logic) { return ScintillatorCell::_logicToCell[logic]; }

    void SetPlacement(G4RotationMatrix *pRot, const G4ThreeVector &tlate, unsigned int posX, unsigned int posY);
};

#endif // SCINTILLATORCELL_H
