# GeneticVehicleDrift
Self-Driving Vehicle Setup <br>
Using Genetic Algorithm to evolve a Neural Network Vehicle to drift around in an enclosed circuit.\
<br/><br/>

The image below is a link to the following youtube video: https://www.youtube.com/watch?v=Yx6JVTKVl0M
[![Watch the video](https://img.youtube.com/vi/Yx6JVTKVl0M/maxresdefault.jpg)](https://www.youtube.com/watch?v=Yx6JVTKVl0M)
<br/>
<hr/>

## How does it work?
<ol>
  <li>The God.c script decides the population size and mutation probabilities</b></li>
  <li>When the sim starts, N individuals are spawned with a randomly initialized Neural Network (NN)</b></li>
  <li>For every Update() cycle, the individual calculates Steering & Throttle inputs from the NN</b></li>
  <li>If the individual crashes into the wall, it stops and waits for all other individuals to crash</b></li>
  <li>Once all the individuals crash, random pairs are picked from the population, with the one gone farthest, having the highest probability</b></li>
  <li>Then iterate through each NN Weight and if a random float is greater than pCrossing, exchange the Weights</b></li>
  <li>Do this till the population of the new individuals is again N</b></li>
  <li>Run the next generation</b></li>
</ol>


## Files of interest
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
