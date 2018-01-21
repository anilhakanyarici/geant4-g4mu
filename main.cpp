#include <iostream>
#include <time.h>

#include "Vis3dConsts.h"
#include "Vis3dActInits.h"
#include "AttCoefActInits.h"
#include "AttCoefConsts.h"

#include "Geant4/QBBC.hh"
#include "Geant4/Randomize.hh"
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4VisExecutive.hh"
#include "Geant4/G4UIExecutive.hh"
#include "Geant4/G4Run.hh"
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4MTRunManager.hh"
#include "Geant4/G4Accumulable.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4SystemOfUnits.hh"


using namespace std;

void command();
void visCommands();
void beam(int beam);
void interpretCmdArgs(int argc, char **argv);

bool useGUI = false;

std::string element = "";

bool computeAttenuationCoefficient = false;
double cacGunEnergy = 0;

bool runAngleDistrubiton = false;
std::string adistFilePath = "";

bool runEnergy = false;
std::string logeratFilePath = "";

int beamCount = 1000;

std::vector<std::string> excludeCommands;

int main(int argc, char **argv)
{

    interpretCmdArgs(argc, argv);

    G4UIExecutive *ui =  new G4UIExecutive(argc, argv);

    CLHEP::RanecuEngine *randomEngine = new CLHEP::RanecuEngine();
    randomEngine->setSeed(clock());
    CLHEP::HepRandom::setTheEngine(randomEngine);
    G4VModularPhysicsList *physicsList = new QBBC();
    physicsList->SetVerboseLevel(0);

    G4RunManager *runManager = new G4RunManager();
    runManager->SetUserInitialization(physicsList);
    G4VUserDetectorConstruction *detconst = nullptr;
    G4VUserActionInitialization *actInit = nullptr;

    if(useGUI){
        detconst = new Vis3dConsts("G4_U");
        actInit = new Vis3dActInits();
    }
    if(computeAttenuationCoefficient){
        detconst = new AttCoefConsts(element);
        actInit = new AttCoefActInits(cacGunEnergy * MeV);
    } else {
        detconst = new Vis3dConsts(element);
        actInit = new Vis3dActInits(adistFilePath, logeratFilePath);
    }

    runManager->SetUserInitialization(detconst);
    runManager->SetUserInitialization(actInit);

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
    command();
    visCommands();
    if(useGUI){
        ui->SessionStart();
        delete ui;
        delete runManager;
        delete visManager;
        std::cout << "Visualization Manager deleted.\n";
    } else {
        beam(beamCount);
    }


    return 0;
}

void interpretCmdArgs(int argc, char **argv){
    for(int i = 1; i < argc; i++){
        std::string arg = argv[i];
        if(arg == "-vis"){
            if(argc > 2) {
                std::cout << "-vis parameter must use alone.\n";
                exit(127);
            }
            useGUI = true;
        } else if(arg == "-cac"){
            i++;
            if(i >= argc){
                std::cout << "Missing parameter in using -cac command.\n";
                exit(127);
            }
            element = argv[i++];
            if(i >= argc){
                std::cout << "Missing parameter in using -cac command.\n";
                exit(127);
            }
            cacGunEnergy = std::stod(argv[i]);
            if(cacGunEnergy <= 0){
                std::cout << "ParticleGun Energy must be greater than zero.\n";
                exit(127);
            }
            computeAttenuationCoefficient = true;
        } else if(arg == "-adist"){
            i++;
            if(i >= argc){
                std::cout << "Missing parameter in using -adist command.\n";
                exit(127);
            }
            element = argv[i++];
            if(i >= argc){
                std::cout << "Missing parameter in using -adist command.\n";
                exit(127);
            }
            adistFilePath = argv[i];
            runAngleDistrubiton = true;
        } else if(arg == "-logerat"){
            i++;
            if(i >= argc){
                std::cout << "Missing parameter in using -logerat command.\n";
                exit(127);
            }
            element = argv[i++];
            if(i >= argc){
                std::cout << "Missing parameter in using -logerat command.\n";
                exit(127);
            }
            logeratFilePath = argv[i];
            runEnergy = true;
        } else if(arg == "-beam"){
            i++;
            if(i >= argc){
                std::cout << "Missing parameter in using -beam command.\n";
                exit(127);
            }
            beamCount = std::stod(argv[i]);
            if(beamCount <= 0){
                std::cout << "Beam count must be greater than zero.\n";
                exit(127);
            }
        } else if(arg[0] == '/'){
            excludeCommands.push_back(arg);
        } else {
            std::cout << "Unknown parameter.\n";
            exit(127);
        }
    }
}
void visCommands(){
    G4UImanager *uim = G4UImanager::GetUIpointer();

    uim->ApplyCommand("/vis/open OGL 600x600-0+0");
    uim->ApplyCommand("/vis/viewer/set/autoRefresh false");
    uim->ApplyCommand("/vis/verbose errors");
    uim->ApplyCommand("/vis/drawVolume");
    uim->ApplyCommand("/vis/viewer/set/viewpointVector -1 0 0");
    uim->ApplyCommand("/vis/viewer/set/lightsVector -1 1 0");
    uim->ApplyCommand("/vis/viewer/set/style wireframe");
    uim->ApplyCommand("/vis/viewer/set/auxiliaryEdge true");
    uim->ApplyCommand("/vis/viewer/set/lineSegmentsPerCircle 100");
    uim->ApplyCommand("/vis/scene/add/trajectories smooth");
    uim->ApplyCommand("/vis/modeling/trajectories/create/drawByCharge");
    uim->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true");
    uim->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2");
    uim->ApplyCommand("/vis/scene/endOfEventAction accumulate");

    uim->ApplyCommand("/vis/set/textLayout");
    uim->ApplyCommand("/vis/set/textColour");
    uim->ApplyCommand("/vis/set/colour");
    uim->ApplyCommand("/vis/set/lineWidth");
    uim->ApplyCommand("/vis/geometry/set/visibility World 0 false");
    uim->ApplyCommand("/vis/geometry/set/colour Envelope 0 0 0 0.6 0.3");
    uim->ApplyCommand("/vis/viewer/set/background 0.6 0.6 0.7");

    uim->ApplyCommand("/vis/viewer/set/style surface");
    uim->ApplyCommand("/vis/viewer/set/hiddenMarker true");
    uim->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 120 150");
    uim->ApplyCommand("/vis/viewer/set/autoRefresh true");
    uim->ApplyCommand("/vis/verbose warnings");

    //uim->ApplyCommand("/vis/scene/add/axes");

    uim->ApplyCommand("/gui/addMenu beam Beam");
    uim->ApplyCommand("/gui/addButton beam \"1\" \"/run/beamOn 1\"");
    uim->ApplyCommand("/gui/addButton beam \"5\" \"/run/beamOn 5\"");
    uim->ApplyCommand("/gui/addButton beam \"10\" \"/run/beamOn 10\"");
    uim->ApplyCommand("/gui/addButton beam \"20\" \"/run/beamOn 20\"");
    uim->ApplyCommand("/gui/addButton beam \"50\" \"/run/beamOn 50\"");
    uim->ApplyCommand("/gui/addButton beam \"100\" \"/run/beamOn 100\"");
}
void command(){
    G4UImanager *uim = G4UImanager::GetUIpointer();
    uim->ApplyCommand("/run/initialize");

}
void beam(int beam)
{
    G4String cmd = "/run/beamOn " + std::to_string(beam);
    G4UImanager::GetUIpointer()->ApplyCommand(cmd);
}
