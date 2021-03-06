//
// Created from BuildGeometry program
//

/**
 \file
 \brief BGField1-7 are 7 almost identical headers that define 7 similar classes derived from "EMMAElementField.hh."
 They each descibe an EM field that is part of EMMA.

 There are 7 header files to reflect the 7 fields used in EMMA: the Q Q E M E Q Q set up.
 This is evident in the history of the document; before "EMMAElementField.hh" was developed for general EM fields,
 old G4 headers like "G4MagneticField" and "G4ElectricField" were used.
 */
#ifndef BGField4_H
#define BGField4_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"

#include "EMMAElementField.hh"

#include <iomanip>

// global variables
extern G4double currentCharge; // EMMASteppingAction.cc
extern G4double userCharge; // EMMAPrimaryGeneratorAction.cc


class BGField4 : public EMMAElementField
//class BGField4 : public G4MagneticField
{
public:

  // Constructor
  // offset coordinates are of the x and z location of where the field begins in reference to
  // the world logical volume
  
  // BGField4(G4double xoffset, G4double zoffset,G4double zbefore,G4double zafter);

 BGField4(G4double xoffset, G4double zoffset, G4double zbefore, G4double zafter, G4LogicalVolume*, G4ThreeVector);

  ~BGField4();

  // instead you can limit the size
  // Retrieves length, width, and height (of type G4double) and returns the maximum representable finite double number

  virtual G4double GetLength() { return DBL_MAX; }
  virtual G4double GetWidth() { return DBL_MAX; }
  virtual G4double GetHeight() { return DBL_MAX; }

  // AddFieldValue() adds the field for this element into the field[].
  // Point() is in global coordinates.

  virtual void AddFieldValue( const G4double Point[3], G4double field[6]) const;
 // virtual void GetFieldValue( const G4double Point[3], G4double *Bfield) const;

private:

  double data[75], offset[3];

  G4int i;
  G4double Pi;
  G4double FieldStrength_0;

public:

  G4double GetFieldStrength() { return data[14]; };
  void ScaleFieldStrength( G4double msf ) { G4cout << "msf: " << msf << G4endl; data[14] = msf * FieldStrength_0 ; };

};
#endif
