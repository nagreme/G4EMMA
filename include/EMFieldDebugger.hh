// Use this routine to calculate E and B fields at certain locations
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EMFieldDebugger_h
#define EMFieldDebugger_h 1

#include "EMMADetectorConstruction.hh"


/// Global variables from SpectrometerConstruction.cc
extern G4double zQ1ends; //z location at beginning and end of element
extern G4double zQ2ends;
extern G4double zQ3ends;
extern G4double zQ4ends;
extern G4double zQ1begins;
extern G4double zQ2begins;
extern G4double zQ3begins;
extern G4double zQ4begins;
extern G4double zQ1fieldbegins; //z location at beginning and end of field (field extends beyond width of element)
extern G4double zQ2fieldbegins;
extern G4double zQ2fieldends;
extern G4double zQ3fieldbegins;
extern G4double zQ4fieldbegins;
extern G4double zQ4fieldends;
extern G4double zED1fieldbegins;
extern G4double xED1fieldends;
extern G4double zED1fieldends;
extern G4double xMDfieldbegins;
extern G4double zMDfieldbegins;
extern G4double xMDfieldends;
extern G4double zMDfieldends;
extern G4double xED2fieldbegins;
extern G4double zED2fieldbegins;
extern G4double zED2fieldends;
extern G4double xED1center; //center coordinates of circle that defines shape and position of dipole
extern G4double zED1center;
extern G4double xED2center;
extern G4double zED2center;
extern G4double rED; //radius of circle that defines shape and position of dipole
extern G4double xMDcenter;
extern G4double zMDcenter;
extern G4double rMD;
extern G4double Q1before; // Distance field extends before and after the quads and dipoles
extern G4double Q2before;
extern G4double ED1before;
extern G4double MDbefore;
extern G4double ED2before;
extern G4double Q3before;
extern G4double Q4before;
extern G4double Q1after;
extern G4double Q2after;
extern G4double ED1after;
extern G4double MDafter;
extern G4double ED2after;
extern G4double Q3after;
extern G4double Q4after;
extern G4double Pipe4z;
extern G4double Pipe4HL;
extern G4double Pipe7HL;

/// Global variables from EMMADetectorConstruction.cc
extern G4double magneticScaling;
extern G4double electricScaling;


/// Global variables from EMMAPrimaryGeneratorAction.cc
extern G4String fieldFileName;

#include "EMMAGlobalField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/*!
 \file
 \brief This header declares a class used in the corresponding source file that calculates the value of EM fields at different locations in the simulation.
*/

class EMFieldDebugger
{
public:
  EMFieldDebugger(int icomp);
  ~EMFieldDebugger();

private:

  EMMAGlobalField* GlobalField;
  G4double worldpos[4];
  G4double worldpos2[4];
  G4double field[6];
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
