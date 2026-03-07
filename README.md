

# BL4S 2026: MRB Spallation & Space Shielding Simulation

## Overview

This repository contains the official Geant4 Monte Carlo simulation developed for our Beamline for Schools (BL4S) 2026 proposal.

The simulation evaluates the radiation shielding efficacy of a novel Martian Regolith Biopolymer (MRB) against secondary spallation radiation (specifically neutrons and charged fragments) produced by high-energy Galactic Cosmic Ray (GCR) analogs. We compare two MRB biological composites (80/20 and 60/40) against standard aerospace materials (Lead and HDPE) using a rigorous equal areal density methodology.

## Experimental Physics Methodology

### 1. Beam Realism

* **Particle:** Proton
* **Momentum:** 5.0 GeV/c (Monoenergetic to reflect standard test beam conditions)
* **Spot Size:** 2 mm diameter Gaussian distribution ($\sigma = 1.0$ mm)
* **Divergence:** 2 milliradians

### 2. Areal Density Normalization

To ensure a fair comparison for spaceflight mass-efficiency, the simulation utilizes a custom UI command. This command dynamically recalculates and adjusts the physical thickness of each material target to maintain an exact areal density of 13.75 g/cm². Any reduction in downstream particle multiplicity is therefore a direct result of the material's structural efficiency, not sheer mass.

### 3. Hybrid Matrix Detection & Hardware Triggers

The simulation employs a hybrid detector matrix to capture a complete profile of the secondary radiation environment:

* **Hardware Trigger Logic:** Utilizes an upstream (`S_in`) and downstream (`Veto`) plastic scintillator array to establish a 50 ns Time-of-Flight coincidence window for distinguishing charged vs. neutral events.
* **Charged Particles (On-Axis):** Tracked via three Delay Wire Chambers (DWCs) placed downstream of the target, operating with a 93% Argon / 7% CO2 gas mixture.
* **Neutral Particles (Off-Axis):** Tracked via a Helium-3 Proportional Counter (WENDI detector) specifically configured to capture secondary neutron spallation.
* **Energy Deposition:** Measured via a 4x4 Lead-Glass Calorimeter array at the endpoint of the beamline.

## Output & Data Structure

The simulation automatically outputs experimental data to `.root` files for high-performance analysis. The Ntuple `SpallationData` contains the following structured columns:

1. `EventID`
2. `DWC0_Hits`
3. `DWC1_Hits`
4. `DWC2_Hits`
5. `CaloEnergy_MeV`
6. `WENDI_Neutrons`
7. `T_charged` (Boolean 0/1 trigger flag)
8. `T_neutral` (Boolean 0/1 trigger flag)

## Prerequisites

* Geant4 (v11.0 or higher) compiled with the High-Precision physics list libraries.
* CMake (v3.16+)
* A standard C++ Compiler (GCC, Clang, or MSVC)
* ROOT (for analyzing the output `.root` trees)

## Build Instructions

To compile the simulation from source, clone the repository to your local machine and execute the following build sequence:

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make -j4` *(Adjust the thread count based on your CPU)*

## Execution and Usage

The simulation is fully automated via macro files. The primary macro will sequentially swap out the shielding materials, recalculate the geometry, and run 100,000 events per material across multiple CPU threads.

To run the full experimental suite, execute:
`./GCRSimulation macs/run_all.mac`

The macro outputs four distinct ROOT files (`SpallationData_MRB8020.root`, `SpallationData_MRB6040.root`, `SpallationData_Lead.root`, and `SpallationData_HDPE.root`).

## Custom UI Commands

We implemented custom Geant4 Messenger classes to allow dynamic control of the simulation parameters without recompiling the C++ source:

* `/MRB/gun/momentum [value] [unit]` - Adjusts the primary proton beam momentum (Default: 5.0 GeV).
* `/MRB/setup/shieldConfig [Material_Name]` - Automatically swaps the target material and dynamically resizes the target depth to maintain the 13.75 g/cm² baseline.

## Author

Htoo Tay Za

Developed for the 2026 CERN Beamline for Schools Competition.

