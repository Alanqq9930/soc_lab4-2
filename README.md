This project integrates Caravel SoC with our designed user project.

The user project is dedicated for accelerating FIR filter
There are two main sections in the user project:
1.BRAM: For Code storage from the external SPI flash
2.AXI protocal FIR filter Engine: Provides an interface between Wishbone and AXI-Lite, AXI-Stream protocal to execute FIR filter by Hardware

The compiled firmware will be written into Caracel SoC via SPI flash, and then executed by the management area core. The pro


To run the simulation, please follow the following steps:
1. Download the Vivado project named Caravel_2.zip
2. unzip the project
3. Open the project by Vivado 2022.1, any other versions of Vivado will be read only
4. Press run simulation
5. Press run all  
