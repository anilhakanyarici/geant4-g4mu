#ifndef SCINTILLATOR_H
#define SCINTILLATOR_H

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

#include "ScintillatorCell.h"
#include "map"

class Scintillator
{
private:
    G4String _name;
    G4VSolid *_solid;
    G4LogicalVolume *_logic;
    G4Material *_material;
    G4RotationMatrix *_rotation;

    G4ThreeVector _halfSizes;

    ScintillatorCell **_cells;

    unsigned int _rowLength;
    unsigned int _columnLength;

    static std::map<std::string, Scintillator*> _scintillators;

public:
    Scintillator(const G4String &name, G4double pX, G4double pY, G4double pZ, G4Material *material);

    inline G4String GetName() const { return this->_name; }
    inline G4VSolid *GetSolid() const { return this->_solid; }
    inline G4LogicalVolume *GetLogic() const { return this->_logic; }
    inline G4Material *GetMaterial() const { return this->_material; }

    static Scintillator *Find(std::string name) { return Scintillator::_scintillators[name]; }

    ScintillatorCell *GetCell(unsigned int row, unsigned int column) { return this->_cells[row * this->_columnLength + column]; }

    void ConstructCells(unsigned int rowLength, unsigned int columnLength);

    void SetPlacement(G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *mother = nullptr);

    void PositionToIndex(G4double pX, G4double pY, G4int *iX, G4int *iY);
};

#endif // SCINTILLATOR_H
