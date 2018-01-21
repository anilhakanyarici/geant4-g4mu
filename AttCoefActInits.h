#ifndef ATTCOEFACTINITS_H
#define ATTCOEFACTINITS_H

#include "Geant4/G4VUserActionInitialization.hh"
#include "Geant4/G4Run.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4UserRunAction.hh"
#include "Geant4/G4VUserPrimaryGeneratorAction.hh"
#include "Geant4/G4ParticleGun.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4UserTrackingAction.hh"
#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UImessenger.hh"
#include "Geant4/G4UIcmdWithAString.hh"
#include "Geant4/G4UIcmdWithoutParameter.hh"
#include <Geant4/G4EllipticalTube.hh>
#include "Geant4/G4SystemOfUnits.hh"

#include "Scintillator.h"

class AttCoefActInits : public G4VUserActionInitialization
{
private:
    G4double _gunEnergy;
public:
    AttCoefActInits(G4double gunEnergy = 3 * GeV);
    virtual ~AttCoefActInits();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

class AttCoefRunAct : public G4UserRunAction {
public:
    AttCoefRunAct(){}

    virtual void BeginOfRunAction(const G4Run *aRun);
    virtual void EndOfRunAction(const G4Run *aRun);
};

class AttCoefGenAct : public G4VUserPrimaryGeneratorAction {
public:
    AttCoefGenAct(G4double gunEnergy);

    virtual void GeneratePrimaries(G4Event *anEvent);

private:
    G4ParticleGun *_gun;
};

class AttCoefTrackAct : public G4UserTrackingAction
{
private:
    G4EllipticalTube *tube = nullptr;
public:
    AttCoefTrackAct();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
};

#endif // ATTCOEFACTINITS_H
