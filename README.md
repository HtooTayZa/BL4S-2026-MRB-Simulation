# BL4S 2026: MRB Spallation & Space Shielding Simulation

## Overview
This repository contains the official Geant4 Monte Carlo simulation developed for our Beamline for Schools (BL4S) 2026 proposal. 

The simulation evaluates the radiation shielding efficacy of a novel Martian Regolith Biopolymer (MRB) against secondary spallation radiation (specifically neutrons and charged fragments) produced by high-energy protons. We compare two MRB configurations (80/20 and 60/40) against standard aerospace materials (Lead and HDPE) using a rigorous equal areal density methodology.

## Experimental Physics Methodology

### 1. Beam Realism
* **Particle:** Proton
* **Energy:** 5.0 GeV
* **Energy Spread:** 2%
* **Spot Size:** 2 cm diameter (Gaussian distribution)
* **Divergence:** 2 milliradians

### 2. Areal Density Normalization
To ensure a fair comparison for spaceflight mass-efficiency, the simulation utilizes a custom UI command. This command dynamically recalculates and adjusts the physical thickness of each material target to maintain an exact areal density of 13.75 g/cm². Any reduction in downstream particle multiplicity is therefore a direct result of the material's structural efficiency, not sheer mass.

### 3. Hybrid Matrix Detection
The simulation employs a hybrid detector matrix to capture a complete profile of the secondary radiation environment:
* **Charged Particles (On-Axis):** Tracked via three Delay Wire Chambers (DWCs) placed downstream of the target.
* **Neutral Particles (Off-Axis):** Tracked via a Helium-3 Proportional Counter (WENDI detector) specifically configured to capture secondary neutron spallation.
* **Energy Deposition:** Measured via a 4x4 Lead-Glass Calorimeter array at the end of the beamline.

## Repository Structure
* `include/`: C++ header files defining the simulation classes (Detector Construction, Event Action, WENDI SD, etc.).
* `src/`: C++ source files containing the implementation logic.
* `macs/`: Geant4 macro scripts for automated batch processing and material swapping.
* `CMakeLists.txt`: Build system configuration for compiling the simulation.

## Prerequisites
* Geant4 (v11.0 or higher) compiled with the High-Precision physics list libraries.
* CMake (v3.16+)
* A standard C++ Compiler (GCC, Clang, or MSVC)

## Build Instructions
To compile the simulation from source, clone the repository to your local machine and execute the following build sequence:

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make -j2`

## Execution and Usage
The simulation is fully automated via macro files. The primary macro will sequentially swap out the shielding materials, recalculate the geometry, and run 100,000 events per material. 

To run the full experimental suite, execute:
`./GCRSimulation macs/run_all.mac`

The data is output into four distinct comma-separated values (.csv) files corresponding to each material tested (MRB 80/20, MRB 60/40, Lead, and HDPE).

## Custom UI Commands
We implemented custom Geant4 Messenger classes to allow dynamic control of the simulation parameters without recompiling the C++ source:
* `/MRB/gun/energy [value] [unit]` - Adjusts the primary proton beam energy.
* `/MRB/setup/shieldConfig [Material_Name]` - Automatically swaps the target material and dynamically resizes the target depth to maintain the 13.75 g/cm² baseline.

## Authors
Developed for the 2026 CERN Beamline for Schools Competition.
