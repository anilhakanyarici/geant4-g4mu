#ifndef VIS3DCONSTS_H
#define VIS3DCONSTS_H

#include <Geant4/G4VUserDetectorConstruction.hh>

class Vis3dConsts : public G4VUserDetectorConstruction
{
public:
    Vis3dConsts();

    virtual ~Vis3dConsts();
    virtual G4VPhysicalVolume *Construct();
};

#endif // VIS3DCONSTS_H
