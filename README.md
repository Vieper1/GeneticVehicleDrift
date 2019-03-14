# GeneticVehicleDrift
Self-Driving Vehicle Setup <br>
Using Genetic Algorithm to evolve a Neural Network Vehicle to drift around in an enclosed circuit.

### The files you might be looking for
<ul>
  <li>GeneticVehicleDrift/Source/Genetic_DriftCar/<b>God.cpp</b></li>
  <li>GeneticVehicleDrift/Source/Genetic_DriftCar/GeneticAlgorithm/<b>DriftPawn_1HL.cpp</b></li>
</ul>


# Features & Config
<ul>
  <li>AdvVehicle from UE4 starter content <b>tuned for high-slide-angle drifts</b></li>
  <li>Custom Neural Network</li>
  <li>
    Genetic Algorithm Simulation's configuration modifiable (in God.cpp):
    <ol type="1">
      <li>Initial Population</li>
      <li>Number of distance-sensor-units (Input Layer)</li>
      <li>Crossing Probability</li>
      <li>Mutation Probability</li>
      <li>Number of nodes in Hidden Layer</li>
      <li>Drift score multiplier (to +/- the importance of drifting in the simulation)</li>
    </ol>
  </li>
</ul>


# How to run the project
<ol>
  <li>Have UE4 (4.21) installed</li>
  <li>Clone/Download the repository</li>
  <li>Go to directory</li>
  <li>Open GeneticVehicleDrift.uproject</li>
</ol>
