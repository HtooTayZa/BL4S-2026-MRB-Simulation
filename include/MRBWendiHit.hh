#ifndef MRBWendiHit_h
#define MRBWendiHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class MRBWendiHit : public G4VHit
{
  public:
    MRBWendiHit() = default;
    ~MRBWendiHit() override = default;

    // Overrides for memory management
    inline void* operator new(size_t);
    inline void  operator delete(void*);
};

// Define the Hits Collection type for this specific hit
using MRBWendiHitsCollection = G4THitsCollection<MRBWendiHit>;

extern G4ThreadLocal G4Allocator<MRBWendiHit>* MRBWendiHitAllocator;

inline void* MRBWendiHit::operator new(size_t)
{
    if (!MRBWendiHitAllocator) MRBWendiHitAllocator = new G4Allocator<MRBWendiHit>;
    return (void*) MRBWendiHitAllocator->MallocSingle();
}

inline void MRBWendiHit::operator delete(void* hit)
{
    MRBWendiHitAllocator->FreeSingle((MRBWendiHit*) hit);
}

#endif
