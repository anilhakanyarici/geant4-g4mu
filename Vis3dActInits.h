#ifndef ACTIONINITIALIZATION_H
#define ACTIONINITIALIZATION_H

#include "Geant4/G4VUserActionInitialization.hh"
#include "Geant4/G4Run.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4UserRunAction.hh"
#include "Geant4/G4VUserPrimaryGeneratorAction.hh"
#include "Geant4/G4ParticleGun.hh"
#include "Geant4/G4UserEventAction.hh"
#include "Geant4/G4UserSteppingAction.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4UserTrackingAction.hh"
#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UImessenger.hh"
#include "Geant4/G4UIcmdWithAString.hh"
#include "Geant4/G4UIcmdWithoutParameter.hh"

#include "Scintillator.h"

class Vis3dMessenger;

class Vis3dActInits : public G4VUserActionInitialization
{
private:

    Vis3dMessenger *messenger = nullptr;

public:
    Vis3dActInits();
    virtual ~Vis3dActInits();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

class Vis3dRunAct : public G4UserRunAction {
public:
    Vis3dRunAct(){}

    virtual void BeginOfRunAction(const G4Run *aRun);
    virtual void EndOfRunAction(const G4Run *aRun);
};

class Vis3dGenAct : public G4VUserPrimaryGeneratorAction {
public:
    Vis3dGenAct();

    virtual void GeneratePrimaries(G4Event *anEvent);

private:
    G4ParticleGun *_gun;
};

class Vis3dTrackAct : public G4UserTrackingAction
{
    Scintillator *_sci1 = nullptr;
public:
    Vis3dTrackAct();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
};

class Vis3dMessenger : public G4UImessenger
{
public:
    Vis3dMessenger();
    virtual ~Vis3dMessenger();

    virtual void SetNewValue(G4UIcommand *command, G4String parameter);

private:
    G4UIdirectory *dataDir;
    G4UIdirectory *data_AngleDist_Dir;
    G4UIdirectory *data_Energy_Dir;

    G4UIcmdWithAString *saveEnergyCommand;
    G4UIcmdWithAString *saveAngleDistCommand;

    G4UIcmdWithoutParameter *clearCommand;
};

#endif // ACTIONINITIALIZATION_H
