High performance SDL based game engine written in c++ based on the entity-component-system model. 

In order to use the engine, import the necessary GameEngine_ headers. See the included example GameApp files for an example of how the engine can be used and how to create and add your own component files. By querying the static component methods you only have to loop through the smallest set of entities possible, and it should allow for parallelisation if that is desired.

Authors: Adrian Martvall and Ida Laaksonen.
