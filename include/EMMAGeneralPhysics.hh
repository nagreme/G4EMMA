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
// $Id: EMMAGeneralPhysics.hh,v 1.5 2006-06-29 16:31:08 gunter Exp $
// --------------------------------------------------------------
//

#ifndef EMMAGeneralPhysics_h
#define EMMAGeneralPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

class F04StepMax;


/*! \file
    \brief This header defines particles and processes required for other more specific physics processes.

    The G4 header "G4VPhysicsConstructor.hh" contains a virtual class that must be used to create concrete classes for specific applications (such as EMMA).
 The current header builds such a concrete class to include EMMA's specific particles and processes.
*/

class EMMAGeneralPhysics : public G4VPhysicsConstructor
{
  public:
    // Constructor
    EMMAGeneralPhysics(const G4String& name = "general");
    // Destructor
    virtual ~EMMAGeneralPhysics();

	  void SetStepMax(G4double);
    F04StepMax* GetStepMaxProcess();
    void AddStepMax();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();

    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
	  virtual void ConstructProcess();


private:
    G4double MaxChargedStep;
    F04StepMax* stepMaxProcess;

};


#endif
