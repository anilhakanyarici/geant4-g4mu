#include "AttCoefActInits.h"

#include "Geant4/G4RunManager.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4Box.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4Orb.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4Track.hh"

#include "iostream"
#include "Matrix2D.h"
#include "Scintillator.h"
#include "math.h"
#include "sstream"

double attCoefFirstTheta = 0;
double attCoefPreEnergy = 0;
std::vector<double> attenuationCoefficients;

AttCoefActInits::AttCoefActInits(G4double gunEnergy)
{
    this->_gunEnergy = gunEnergy;
}

AttCoefActInits::~AttCoefActInits()
{
}

void AttCoefActInits::BuildForMaster() const
{
    this->SetUserAction(new AttCoefRunAct);

}

void AttCoefActInits::Build() const
{
    this->SetUserAction(new AttCoefGenAct(this->_gunEnergy));
    this->SetUserAction(new AttCoefRunAct);
    this->SetUserAction(new AttCoefTrackAct);
}

void AttCoefRunAct::BeginOfRunAction(const G4Run *)
{
    attenuationCoefficients.clear();
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void AttCoefRunAct::EndOfRunAction(const G4Run *)
{
    double attCoef = 0;
    int l = (int)attenuationCoefficients.size();
    for(int i = 0; i < l; i++)
        attCoef += attenuationCoefficients[i];
    attCoef /= attenuationCoefficients.size();

    std::cout << "Attenuation Coefficient: " << attCoef << " m^(-1)\n";
    std::cout << "\n";
}

AttCoefGenAct::AttCoefGenAct(G4double gunEnergy)
{
    this->_gun  = new G4ParticleGun();

    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    this->_gun->SetParticleDefinition(particle);
    this->_gun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0));
    this->_gun->SetParticleEnergy(gunEnergy);
}

void AttCoefGenAct::GeneratePrimaries(G4Event *anEvent)
{
    G4LogicalVolume *logicEnvelope = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");

    G4Orb *solidEnvelope = dynamic_cast<G4Orb*>(logicEnvelope->GetSolid());
    double r = solidEnvelope->GetRadius();
    G4ThreeVector throwPos = G4ThreeVector(0, 0, -r);
    this->_gun->SetParticlePosition(throwPos);
    this->_gun->GeneratePrimaryVertex(anEvent);
}

AttCoefTrackAct::AttCoefTrackAct() : G4UserTrackingAction()
{

}

void AttCoefTrackAct::PreUserTrackingAction(const G4Track* track)
{
    if(track->GetDynamicParticle()->GetPDGcode() == 13){
        attCoefFirstTheta = track->GetMomentumDirection().theta();
        attCoefPreEnergy = track->GetKineticEnergy();
    }
}

void AttCoefTrackAct::PostUserTrackingAction(const G4Track *track)
{
    if(this->tube == nullptr){
        G4LogicalVolume *logicTube = G4LogicalVolumeStore::GetInstance()->GetVolume("Tube");
        this->tube = dynamic_cast<G4EllipticalTube*>(logicTube->GetSolid());
    }

    if(track->GetDynamicParticle()->GetPDGcode() == 13){
        double lastTheta = track->GetMomentumDirection().theta();
        G4double postEnergy = track->GetKineticEnergy();
        G4double logRat = log(attCoefPreEnergy / postEnergy);
        if(logRat == INFINITY)
            return;
        double deltaTheta = abs(lastTheta - attCoefFirstTheta) * 180 / M_PI;

        if(deltaTheta < 5){
            double z = this->tube->GetDz();
            attenuationCoefficients.push_back(logRat / z);
        }
    }
}
