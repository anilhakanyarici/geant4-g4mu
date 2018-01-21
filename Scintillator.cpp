#include "Scintillator.h"
#include "ScintillatorCell.h"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4VisAttributes.hh"

#include "math.h"

std::map<std::string, Scintillator*> Scintillator::_scintillators;

Scintillator::Scintillator(const G4String &name, G4double pX, G4double pY, G4double pZ, G4Material *material)
{
    G4NistManager *nist = G4NistManager::Instance();

    G4Box *solid= new G4Box(name, pX, pY, pZ);
    G4LogicalVolume *logic = new G4LogicalVolume(solid, nist->FindOrBuildMaterial("G4_AIR"), name);
    logic->SetVisAttributes(new G4VisAttributes(false));
    this->_solid = solid;
    this->_logic = logic;
    this->_halfSizes = G4ThreeVector(pX, pY, pZ);
    this->_material = material;
    this->_name = name;
    Scintillator::_scintillators[name] = this;
}

void Scintillator::ConstructCells(unsigned int rowLength, unsigned int columnLength)
{
    this->_cells = new ScintillatorCell*[rowLength * columnLength];
    this->_rowLength = rowLength;
    this->_columnLength = columnLength;

    G4ThreeVector cellsHalfSizes = G4ThreeVector(this->_halfSizes.x() / rowLength, this->_halfSizes.y() / columnLength, this->_halfSizes.z());

    G4ThreeVector startPos = G4ThreeVector(-this->_halfSizes.x() + cellsHalfSizes.x(), -this->_halfSizes.y() + cellsHalfSizes.y(), 0);
    G4double deltaPosX = cellsHalfSizes.x() * 2;
    G4double deltaPosY = cellsHalfSizes.y() * 2;

    for(unsigned int i = 0; i < rowLength; i++){
        for(unsigned int j = 0; j < columnLength; j++){
            unsigned int arrayPos = i * columnLength + j;
            ScintillatorCell *cell = new ScintillatorCell(this, "cellName", cellsHalfSizes, this->_material); //cellName lazım değil.
            this->_cells[arrayPos] = cell;
            G4ThreeVector cellPosition = G4ThreeVector(startPos.x() + i * deltaPosX, startPos.y() + j * deltaPosY, startPos.z());
            cell->SetPlacement(this->_rotation, cellPosition, i, j);

            if(((i + j) & 1) == 0)
                cell->GetLogic()->SetVisAttributes(new G4VisAttributes(G4Colour::White()));
            else
                cell->GetLogic()->SetVisAttributes(new G4VisAttributes(G4Colour::Red()));
        }
    }
}

void Scintillator::SetPlacement(G4RotationMatrix *pRot, const G4ThreeVector &tlate, G4LogicalVolume *mother)
{
    new G4PVPlacement(pRot, tlate, this->_logic, this->_name, mother, false, 0, false);
    this->_rotation = pRot;
}

void Scintillator::PositionToIndex(G4double pX, G4double pY, G4int *iX, G4int *iY)
{
    G4ThreeVector cellsHalfSizes = G4ThreeVector(this->_halfSizes.x() / this->_rowLength, this->_halfSizes.y() / this->_columnLength, this->_halfSizes.z());
    *iX = (G4int)floor(pX / (2 * cellsHalfSizes.x()) - 0.5) + (this->_columnLength / 2);
    *iY = (G4int)floor(pY / (2 * cellsHalfSizes.y()) - 0.5) + (this->_rowLength / 2);
}
