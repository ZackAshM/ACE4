#pragma once

#include "G4SDManager.hh"
#include "G4AssemblyVolume.hh"
#include "G4NistManager.hh"

///
/// \brief An abstract, non-constructible class, used to consolidate
///        methods to create ACE waveguides.
///
class Waveguide final {

    ///
    /// \brief A private destructor so you cannot construct Waveguide instances.
    ///
    Waveguide() {};
    
public:

    // the size of each waveguide
    static constexpr G4double XWIDTH{15*CLHEP::mm}; ///< The full-width of the waveguide in the x-direction.
    static constexpr G4double YWIDTH{2*8.5*CLHEP::mm}; ///< The full-width of the waveguide in the y-direction.
    static constexpr G4double LENGTH{91.44*CLHEP::cm}; ///< The full-length of the waveguide in the z-direction.
    static constexpr G4double CuTHICKNESS{1.0*CLHEP::mm}; ///< The full-thickness of the outer copper layer

public:

    ///
    /// \brief Construct a logical ACE waveguide/element.
    ///
    static auto createWaveguide() -> G4AssemblyVolume*;


};
