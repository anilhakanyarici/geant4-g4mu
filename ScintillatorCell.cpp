#include "ScintillatorCell.h"
#include "Scintillator.h"

std::map<G4LogicalVolume*, ScintillatorCell*> ScintillatorCell::_logicToCell;

ScintillatorCell::ScintillatorCell(Scintillator *owner, const G4String &name, const G4ThreeVector &halfSizes, G4Material *material)
{
    this->_owner = owner;
    G4Box *solid= new G4Box(name, halfSizes.x(), halfSizes.y(), halfSizes.z());
    G4LogicalVolume *logic = new G4LogicalVolume(solid, material, name);
    this->_logic = logic;
    this->_solid = solid;

    ScintillatorCell::_logicToCell[logic] = this;
}

void ScintillatorCell::SetPlacement(G4RotationMatrix *pRot, const G4ThreeVector &tlate, unsigned int posX, unsigned int posY)
{
    new G4PVPlacement(pRot, tlate, this->_logic, this->_name, this->_owner->GetLogic(), false, 0, false);
    this->_position = G4ThreeVector(posX, posY, 0);
}
