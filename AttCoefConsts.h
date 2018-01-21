#ifndef ATTCOEFCONSTS_H
#define ATTCOEFCONSTS_H

#include <Geant4/G4VUserDetectorConstruction.hh>
#include <Geant4/G4Material.hh>
#include <Geant4/G4NistManager.hh>

class AttCoefConsts : public G4VUserDetectorConstruction
{
public:
    AttCoefConsts(G4String material);
    AttCoefConsts(G4Material* material);

    virtual ~AttCoefConsts();
    virtual G4VPhysicalVolume *Construct();

private:
    G4Material *mat;
};

#endif // ATTCOEFCONSTS_H
