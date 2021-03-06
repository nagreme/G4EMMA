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
// $Id: EMMADriftChamberHit.hh,v 1.5 2006-06-29 16:30:55 gunter Exp $
// --------------------------------------------------------------
//
#ifndef EMMADriftChamberHit_h
#define EMMADriftChamberHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"


//! Global variables
extern G4String focalPlaneFileName;
extern G4String MotherDir; //! EMMAapp.cc


class G4AttDef;
class G4AttValue;


/*!
 \file
 \brief This class manages the hit object in regards to the drift chamber part of the simulation. It defines global variables and functions for the G4 code to run necessary hit simulations.
 The virtual methods draw() and print() are used. For further details about each part of this file, examine the accompanying explanations adjacent to the classes.
*/


class EMMADriftChamberHit : public G4VHit
{
public:
  //! Two Constructors and a Destructor
  EMMADriftChamberHit();
  EMMADriftChamberHit(G4int z);
  virtual ~EMMADriftChamberHit();
  //! Operators
  EMMADriftChamberHit(const EMMADriftChamberHit &right);
  const EMMADriftChamberHit& operator=(const EMMADriftChamberHit &right);
  int operator==(const EMMADriftChamberHit &right) const;

  //! Inline methods
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  //inline void operator delete(void *aHit);

  //! Inline methods x and y that return a float
  inline float x();
  inline float y();

  //! Virtual methods
  virtual void Draw();
  virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
  virtual std::vector<G4AttValue>* CreateAttValues() const;
  virtual void Print();

private:
  //! Private variables layerID of type G4int,
  //! time, Ekin, dirn, theta, Edep of type G4double,
  //! localPos, worldPos, Momentum of type G4ThreeVector
  G4int layerID;
  G4double time;
  G4ThreeVector localPos;
  G4ThreeVector worldPos;
  G4ThreeVector Momentum;
  G4double Ekin;
  G4double dirn;
  G4double theta;
  G4double Edep;


public:
  //! Public methods to get and set layerID, time,
  //! localPos, worldPos, theta, Momentum, and Ekin
  inline void SetLayerID(G4int z) { layerID = z; }
  inline G4int GetLayerID() const { return layerID; }
  inline void SetTime(G4double t) { time = t; }
  inline G4double GetTime() const { return time; }
  inline void SetLocalPos(G4ThreeVector xyz) { localPos = xyz; }
  inline G4ThreeVector GetLocalPos() const { return localPos; }
  inline void SetWorldPos(G4ThreeVector xyz) { worldPos = xyz; }
  inline G4ThreeVector GetWorldPos() const { return worldPos; }
  inline void SetTheta(G4double thet) { theta = thet; }
  inline G4double GetTheta() const { return theta; }

  inline void SetMomentum(G4ThreeVector xyz) { Momentum = xyz; }
  inline G4ThreeVector GetMomentum() const { return Momentum; }
  inline void SetEkin(G4double e) { Ekin = e; }
  inline G4double GetEkin() const { return Ekin; }

  //inline void Add(G4double de);
  //inline G4double GetEdep() const { return Edep; }

};

//! EMMADriftChamberHitsCollection counts the number of hits
typedef G4THitsCollection<EMMADriftChamberHit> EMMADriftChamberHitsCollection;
//! EMMADriftChamberHitAllocator allocates hits
extern G4Allocator<EMMADriftChamberHit> EMMADriftChamberHitAllocator;

inline void* EMMADriftChamberHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)EMMADriftChamberHitAllocator.MallocSingle();
  return aHit;
}

inline void EMMADriftChamberHit::operator delete(void* aHit)
{
  EMMADriftChamberHitAllocator.FreeSingle((EMMADriftChamberHit*) aHit);
}

//inline void EMMADriftChamberHit::Add(G4double de) {
//	Edep += de;
//}


#endif

