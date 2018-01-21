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

bool useGUI = false;
bool computeAttenuationCoefficient = false;
bool runAngleDistrubiton = false;
bool runEnergy = false;
int beamCount = 1000;

int main(int argc, char **argv)
{
    useGUI = (argc > 1 && !strcmp("-vis", argv[1]));
    computeAttenuationCoefficient = (argc > 1 && !strcmp("-cac", argv[1]));

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

    if(computeAttenuationCoefficient){
        detconst = new AttCoefConsts(argv[2]);
        actInit = new AttCoefActInits(std::stod(argv[3]) * MeV);
        if(argc > 5 && !strcmp("-beam", argv[4])){
            beamCount = std::stoi(argv[5]);
        }
    } else {
        detconst = new Vis3dConsts();
        actInit = new Vis3dActInits();
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
