//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: EMMAEventAction.cc,v 1.10 2007-05-17 09:55:14 duns Exp $
// --------------------------------------------------------------
//

#include "EMMAEventAction.hh"
#include "EMMAEventActionMessenger.hh"
#ifdef G4ANALYSIS_USE
#include "EMMAAnalysisManager.hh"
#endif // G4ANALYSIS_USE
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "EMMADriftChamberHit.hh"
#include "EMMAIonChamber.hh"
#include "EMMAIonChamberHit.hh"


#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


using namespace std;

//global variable
extern G4String UserDir;

/*! \file
 \brief Takes care of the interactions (events) of an object that was generated in the Primary Generator.
 Note: If you are getting error messages concerning this file while building EMMA it is likely an error or problem in your (CERN) ROOT installation, as this file calls upon ROOT functions.
 Reinstall or remake ROOT, or seek your local computer guru for help.
*/



EMMAEventAction::EMMAEventAction()
:fp_hitpos(0),fp_hitangle(0),fp_hitEkin(0),fp_hitEdep(0),fp_hitposX(0)
{
  G4String colName;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  DHC2ID = SDman->GetCollectionID(colName="detector/detectorColl");
  verboseLevel = 1;
  messenger = new EMMAEventActionMessenger(this);

#ifdef G4ANALYSIS_USE
 // Create root files for analysis

  //call root file
  EMMAAnalysisManager* analysisManager = EMMAAnalysisManager::getInstance();
  rootfile = analysisManager->getRootfile();

  //create root histograms
  fp_hitpos = new TH2F("hitpos","Focal plane hit position",160,-80,80,60,-30,30);
  fp_hitpos->GetXaxis()->SetTitle("X position (mm)");	//axis labels
  fp_hitpos->GetYaxis()->SetTitle("Y position (mm)");
  fp_hitposX = new TH1F("hitposX","Focal Plane X Position",160,-80,80);
  fp_hitposX->GetXaxis()->SetTitle("X position (mm)");	//axis labels
  fp_hitposX->GetYaxis()->SetTitle("Counts");
  fp_hitangle = new TH1F("hitangle","Focal plane hit angle",100,0,10);
  fp_hitangle->GetXaxis()->SetTitle("Theta (deg)");	//axis labels
  fp_hitangle->GetYaxis()->SetTitle("Counts");
  fp_hitEkin = new TH1F("hitEkin","Focal Plane Ekin",5000,0,200);
  fp_hitEkin->GetXaxis()->SetTitle("Energy (MeV)");
  fp_hitEkin->GetYaxis()->SetTitle("Counts");
  fp_hitEdep = new TH1F("hitEdep","Edep in Ion Chamber",5000,0,200);
  fp_hitEdep->GetXaxis()->SetTitle("Energy (MeV)");
  fp_hitEdep->GetYaxis()->SetTitle("Counts");
  fp_hitposEdep = new TH2F("hitposEdep","Position vs Edep",160,-80,80,2000,0,200);
  fp_hitposEdep->GetXaxis()->SetTitle("x Position (mm)");
  fp_hitposEdep->GetYaxis()->SetTitle("Edep (MeV)");

  //call root tree and creates branches to store event by event data
  fp_tree = analysisManager->getRoottree();
  fp_tree->Branch("fp_pos",&fp_pos,"fp_pos[2]/D");
  fp_tree->Branch("fp_posX",&fp_posX,"fp_posX/D");
  fp_tree->Branch("fp_theta",&fp_theta,"fp_theta/D");
  fp_tree->Branch("fp_Ekin",&fp_Edep,"fp_Ekin/D");
  fp_tree->Branch("fp_Edep",&fp_Edep,"fp_Edep/D");
  fp_tree->Branch("fp_posEdep",&fp_posEdep,"fp_posEdep/D");

#endif // G4ANALYSIS_USE
}

EMMAEventAction::~EMMAEventAction()
{
#ifdef G4ANALYSIS_USE
  EMMAAnalysisManager::dispose();
  rootfile->Write(); //writes all histograms and trees to file (also ones created in SteppingAction.cc
  rootfile->Close(); //must close root file
  delete rootfile;
#endif // G4ANALYSIS_USE
  delete messenger;
}

EMMAIonChamberHitsCollection*
EMMAEventAction::GetHitsCollection(const G4String& hcName,
				const G4Event* event) const
{
  G4int hcID
    = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
  EMMAIonChamberHitsCollection* hitsCollection
    = static_cast<EMMAIonChamberHitsCollection*>(
	event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection ) {
    G4cerr << "Cannot access hitsCollection" << hcName << G4endl;
    exit(1);
   }

   return hitsCollection;
}

void EMMAEventAction::PrintEventStatistics(
				G4double IonChamberEdep, G4double IonChamberTrackLength) const
{
  // print event statistics
  G4cout
     <<" Ion Chamber total energy: "
     << std::setw(7) << G4BestUnit(IonChamberEdep, "Energy")
     <<"	total track length: "
     << std::setw(7) << G4BestUnit(IonChamberTrackLength, "Length")
     <<G4endl;
}

void EMMAEventAction::BeginOfEventAction(const G4Event* event)
{

  G4cout<<"Start event: "<<event->GetEventID()<<G4endl;

}

void EMMAEventAction::EndOfEventAction(const G4Event* event)
{
  G4HCofThisEvent * HCE = event->GetHCofThisEvent();
  EMMADriftChamberHitsCollection* DHC2 = 0;
  if(HCE)
  {
    DHC2 = (EMMADriftChamberHitsCollection*)(HCE->GetHC(DHC2ID));
  }

  // Send a simulation event message to Django (modification for server)
  // Nadège Pulgar-Vidal, Nov 2017
  int ret_code;

  int pid = fork();
  if (pid == 0) // child process
  {
    // Build the cmd, must remove trailing '/' from UserDir
    string userdir(UserDir.c_str());
    userdir = userdir.substr(0, userdir.length()-1);

    string manage_cmd(getenv("DJANGO_MANAGEMENT_PATH"));
    manage_cmd += "/manage.py";

    G4cout << userdir << endl;
    G4cout << manage_cmd << endl;

    ret_code = execlp("python3", "python3", manage_cmd.c_str(), "send_sim_event_msg", userdir.c_str(), (char*)0);
    if (ret_code != 0)
    {
      G4cout << "Error sending sim event msg" << endl;
    }
    exit(0);
  }

  wait(NULL);

  // Diagnostics

  if (verboseLevel==0 || event->GetEventID() % verboseLevel != 0) return;

  G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);
  G4cout << G4endl
         << "\t>>> Event " << event->GetEventID() << " >>> Simulation truth : "
         << primary->GetG4code()->GetParticleName()
         << " " << primary->GetMomentum() << G4endl;

  //Get Hits Collection
  EMMAIonChamberHitsCollection* IonChamberHC
    = GetHitsCollection("IonChamberHitsCollection", event);

  //Get hit with total values
  EMMAIonChamberHit* IonChamberHit = (*IonChamberHC)[IonChamberHC->entries()-1];

  PrintEventStatistics(
	IonChamberHit->GetEdep(), IonChamberHit->GetTrackLength());

  if(DHC2)
  {
    int n_hit = DHC2->entries();
    G4cout << "Stopping Block has " << n_hit << " hits." << G4endl;
    for(int i2=0;i2<5;i2++)
    {
      for(int i1=0;i1<min(1,n_hit);i1++)
      {
        EMMADriftChamberHit* aHit = (*DHC2)[i1];
        if(aHit->GetLayerID()==i2){
          aHit->Print();

#ifdef G4ANALYSIS_USE
            localPos = aHit->GetLocalPos();	//local position at focal plane
            theta = aHit->GetTheta()/deg; //angle at focal place
	    Ekin = aHit->GetEkin()/MeV;
	    Edep = IonChamberHit->GetEdep();
            if (fp_hitpos) fp_hitpos->Fill(localPos.x()/mm, localPos.y()/mm); //fill histogram
	    if (fp_hitposX) fp_hitposX->Fill(localPos.x()/mm);
            if (fp_hitangle) fp_hitangle->Fill(theta); //fill histogram
	    if (fp_hitEkin) fp_hitEkin->Fill(Ekin);
	    if (fp_hitEdep) fp_hitEdep->Fill(Edep);
	    if (fp_hitposEdep) fp_hitposEdep->Fill(localPos.x()/mm,Edep);
            if (fp_tree){ //fill branch with position and angle for each event
              fp_pos[0]=localPos.x()/mm;
              fp_pos[1]=localPos.y()/mm;
              fp_theta=theta;
	      fp_Edep=Edep;
              fp_tree->Fill();
            }
#endif // G4ANALYSIS_USE
        }
      }
    }
  }
}
