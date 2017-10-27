#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT_HP.hh"
#//#include "TROOT.h"
#include "G4EmLivermorePhysics.hh"

#include "LouvainDetectorConstruction.hh"

#include "G4VModularPhysicsList.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "TrGEMPhysicsList.hh"


#include "LouvainPrimaryGeneratorAction.hh"

#include "TrGEMRunAction.hh"
#include "TrGEMEventAction.hh"
#include "TrGEMSteppingAction.hh"
#include "TrGEMStackingAction.hh"
//#include "TrGEMTrackingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char** argv) {

   // Set the Random engine
 CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
 CLHEP::HepRandom::setTheSeed(time(NULL)+38999008.);  
 
  //gROOT->ProcessLine("#include <vector>");

   // construct the default run manager
   G4RunManager* runManager = new G4RunManager ;
   // set mandatory initialization classes

  runManager->SetUserInitialization(new LouvainDetectorConstruction ) ;
   G4VUserPhysicsList* physics = new FTFP_BERT_HP();

  runManager->SetUserInitialization(physics);

  runManager->SetUserAction(new LouvainPrimaryGeneratorAction );



   // User Action classes
    runManager->SetUserAction(new TrGEMRunAction) ;
    runManager->SetUserAction(new TrGEMEventAction) ;
    //    runManager->SetUserAction(new TrGEMTrackingAction) ;
    runManager->SetUserAction(new TrGEMStackingAction) ;
    runManager->SetUserAction(new TrGEMSteppingAction) ;

 // get the pointer to the UI manager
   G4UImanager* UImanager = G4UImanager::GetUIpointer() ;

//UImanager->ApplyCommand("/cuts/setLowEdge 100 eV");
   
   

   // initialize G4 kernel
   runManager->Initialize() ;


#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif    
 
   // get the pointer to the UI manager
   //G4UImanager* UImanager = G4UImanager::GetUIpointer() ;

   if (argc!=1)   // batch mode  
   {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
   }
   else           // interactive mode : define UI session
   {
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif

#ifdef G4VIS_USE
      delete visManager;
#endif     
   }

   // job termination
   delete runManager ;
   return 0 ;
 

}
