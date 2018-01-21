#include "Vis3dActInits.h"

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

std::string angleDistFP = "";
std::string energyFP = "";

struct trackData{
    G4double logerat = 0; //kinetik enerji oranının logaritmalarının toplamı
    G4int attCount = 0;
    std::string toString(){
        if(this->attCount == 0)
            return "0";
        std::stringstream ss;
        ss << std::fixed << std::setprecision(4) << (this->logerat / attCount);
        return ss.str();
    }
};

int angleDistribution[91];
double firstTheta = 0;

struct currentIndex{
    G4int x = 0;
    G4int y = 0;
}indices;
G4double preEnergy = 0;
Matrix2D<trackData> datas(100, 100);

Vis3dActInits::Vis3dActInits(std::string angleDistFilePath, std::string energyFilePath)
{
    this->messenger = new Vis3dMessenger();
    angleDistFP = angleDistFilePath;
    energyFP = energyFilePath;
}

Vis3dActInits::~Vis3dActInits()
{
    delete this->messenger;
}

void Vis3dActInits::BuildForMaster() const
{
    this->SetUserAction(new Vis3dRunAct);

}

void Vis3dActInits::Build() const
{
    this->SetUserAction(new Vis3dGenAct);
    this->SetUserAction(new Vis3dRunAct);
    this->SetUserAction(new Vis3dTrackAct);
}

void Vis3dRunAct::BeginOfRunAction(const G4Run *)
{
    std::cout << "Event started.\n";
    preEnergy = 0;
    indices.x = 0;
    indices.y = 0;
    datas.Clear();
    for(int i = 0; i < 91; i++){
        angleDistribution[i] = 0;
    }
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void Vis3dRunAct::EndOfRunAction(const G4Run *)
{
    std::cout << "Event end.\n";
    std::cout << "\n";

    if(energyFP != ""){
        if(datas.SaveToFile(energyFP))
            std::cout << "Energy datas are saved. \n";
        else std::cout << "Failed to save energu datas. \n";
    }

    if(angleDistFP != ""){
        std::ofstream file(angleDistFP);
        if(file.is_open()){
            for(int i = 0; i < 91; i++){
                file << std::fixed << std::setprecision(2) << i << " " << log10(angleDistribution[i] + 1) << "\n";
            }
            file.close();
            std::cout << "Angle Distribution are saved. \n";
        } else {
            std::cout << "Failed to save angle distribution. \n";
        }
    }
}

Vis3dGenAct::Vis3dGenAct()
{
    this->_gun  = new G4ParticleGun();

    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    this->_gun->SetParticleDefinition(particle);
    this->_gun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0));
    this->_gun->SetParticleEnergy(2 * GeV);
}

void Vis3dGenAct::GeneratePrimaries(G4Event *anEvent)
{
    G4LogicalVolume *logicEnvelope = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");

    G4Orb *solidEnvelope = dynamic_cast<G4Orb*>(logicEnvelope->GetSolid());
    double r = solidEnvelope->GetRadius();
    double theta = 2 * G4UniformRand() * M_PI;
    double rR = r * G4UniformRand();
    double x = rR * std::cos(theta);
    double y = rR * std::sin(theta);
    G4ThreeVector throwPos = G4ThreeVector(x, y, -r);
    this->_gun->SetParticlePosition(throwPos);
    this->_gun->GeneratePrimaryVertex(anEvent);
}

Vis3dTrackAct::Vis3dTrackAct() : G4UserTrackingAction()
{
    this->_sci1 = Scintillator::Find("Scintillator1");
}

void Vis3dTrackAct::PreUserTrackingAction(const G4Track* track)
{
    if(track->GetDynamicParticle()->GetPDGcode() == 13){

        firstTheta = track->GetMomentumDirection().theta();

        G4ThreeVector pos = track->GetPosition();
        G4int ix, iy;
        if(this->_sci1 == nullptr) this->_sci1 = Scintillator::Find("Scintillator1");
        this->_sci1->PositionToIndex(pos.x(), pos.y(), &ix, &iy);
        indices.x = ix;
        indices.y = iy;
        preEnergy = track->GetKineticEnergy();
    }
}

void Vis3dTrackAct::PostUserTrackingAction(const G4Track *track)
{
    if(track->GetDynamicParticle()->GetPDGcode() == 13){

        double lastTheta = track->GetMomentumDirection().theta();
        int deltaTheta = (int)round((lastTheta - firstTheta) * 180 / M_PI);
        if(deltaTheta < 91) angleDistribution[deltaTheta]++;

        if(indices.x > 99 || indices.x < 0 || indices.y > 99 || indices.y < 0) return;
        G4double postEnergy = track->GetKineticEnergy();
        G4double rat = log(preEnergy / postEnergy);
        if(rat > 5)
            return;
        trackData data = datas.Item(indices.x, indices.y);
        data.attCount++;
        data.logerat += rat;
        datas.Item(indices.x, indices.y) = data;
    }
}

Vis3dMessenger::Vis3dMessenger() : G4UImessenger()
{
    this->dataDir = new G4UIdirectory("/data/");
    this->data_AngleDist_Dir = new G4UIdirectory("/data/angleDistribution/");
    this->data_Energy_Dir = new G4UIdirectory("/data/energy/");

    this->saveEnergyCommand = new G4UIcmdWithAString("/data/energy/saveFile", this);
    this->saveEnergyCommand->SetGuidance("Save Datas To File");
    this->saveEnergyCommand->SetParameterName("filePath", false);
    this->saveEnergyCommand->AvailableForStates(G4State_PreInit, G4State_Idle);

    this->saveAngleDistCommand = new G4UIcmdWithAString("/data/angleDistribution/saveFile", this);
    this->saveAngleDistCommand->SetGuidance("Save Datas To File");
    this->saveAngleDistCommand->SetParameterName("filePath", false);
    this->saveAngleDistCommand->AvailableForStates(G4State_PreInit, G4State_Idle);
}

Vis3dMessenger::~Vis3dMessenger()
{
    delete this->dataDir;
    delete this->data_AngleDist_Dir;
    delete this->saveAngleDistCommand;
    delete this->data_Energy_Dir;
    delete this->saveEnergyCommand;
}

void Vis3dMessenger::SetNewValue(G4UIcommand *command, G4String parameter)
{
    if(command == this->saveAngleDistCommand){
        std::ofstream file(parameter);
        if(file.is_open()){
            for(int i = 0; i < 91; i++){
                file << std::fixed << std::setprecision(2) << i << " " << log10(angleDistribution[i] + 1) << "\n";
            }
            file.close();
        }
        std::cout << "File saved.\n";
    }
    else if(command == this->saveEnergyCommand){
        if(datas.SaveToFile(parameter))
            std::cout << "File saved.\n";
        else std::cout << "Failed to save file.\n";
    }
}
