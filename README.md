# BL4S 2026: MRB Spallation & Space Shielding Simulation

## Overview
This repository contains the official Geant4 Monte Carlo simulation for our Beamline for Schools (BL4S) 2026 proposal. The simulation evaluates the radiation shielding efficacy of a novel Martian Regolith Biopolymer (MRB) against secondary spallation radiation (specifically neutrons and charged fragments) produced by high-energy protons. 

We compare two MRB configurations (80/20 and 60/40) against standard aerospace materials (Lead and HDPE) using a rigorous **equal areal density** methodology.

## Experimental Physics Methodology
* **Beam Realism:** 5.0 GeV proton beam with a 2% energy spread, 2mm spot size, and 2mrad divergence.
* **Areal Density Normalization:** To ensure a fair comparison for spaceflight mass-efficiency, the simulation utilizes a custom UI command (`/MRB/setup/shieldConfig`) that dynamically recalculates and adjusts the physical thickness of each material target to maintain an exact areal density of **13.75 g/cm²**.
* **Hybrid Matrix Detection:** * **Charged Particles:** Tracked via three on-axis Delay Wire Chambers (DWCs).
  * **Neutral Particles:** Tracked via an off-axis Helium-3 Proportional Counter (WENDI detector) specifically configured to capture secondary neutron spallation.
  * **Energy Deposition:** Measured via a 4x4 Lead-Glass Calorimeter array.

## Requirements
* Geant4 (v11.0 or higher) with the `QGSP_BERT_HP` High-Precision physics list enabled.
* CMake (v3.16+)
* C++ Compiler (GCC, Clang, or MSVC)

## Build Instructions
To compile the simulation from source, clone the repository and run the following commands:

```bash
mkdir build
cd build
cmake ..
make -j2
