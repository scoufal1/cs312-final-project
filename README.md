# cs312-final-project

# particles

Implements particle systems using openGL

TODO: Add a representative image for your project here

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake -G "Visual Studio 16 2019" ..
particles/build $ start CS312-Particles.sln
```

To run from the git bash command shell, 

```
particles/build $ ../bin/Debug/simple-demo
particles/build $ ../bin/Debug/demo
```

*macOS*

Open terminal to the directory containing this repository.

```
particles $ mkdir build
particles $ cd build
particles/build $ cmake ..
particles/build $ make
```

To run each program from build, you would type

```
particles/build $ ../bin/simple-demo
particles/build $ ../bin/demo
```
## Algorithms, Data structures, and Math

A spring is an example of an n-ary force, which means the force is applied to a fixed set of particles. The force exerted on the particle is calculated using Hooke’s law:

<img width="500" alt="Screen Shot 2021-05-09 at 9 43 54 PM" src="https://user-images.githubusercontent.com/48161551/117595762-abce9380-b10f-11eb-90b8-7e993755676b.png">
 
The force exerted on the mass depends on the spring elasticity constant (kspring), the length that spring is either compressed or stretched (||Δx||), the resting length of the spring at equilibrium (r). The damping force, which opposes the motion and is due to the energy lost due to resistive forces and is dependent on the damping coefficient (kdamp) the velocity of the particle dotted with the normalized distance vector (Δv • Δx). The whole term in the bracket represents the magnitude, which is multiplied by the normalized direction vector to determine the direction of the force. 

Similarly to the particle system demo, the particle is stored in a struct that contains information about position, velocity, force, and mass. Each frame, the position of the particle will be recalculated. In this case, however, the velocity of the particle is also changing each frame, which is determined by the force exerted on the particle and the mass of the particle. The Newton-Euler equations of motion are used to calculate the new velocities and positions:

```
vel = vel + Δt * (force/mass)

pos = pos + Δt * vel
```

For multiple springs, the particles are stored in a list, and there is also a list of springs, where each spring struct holds the indices of two particles in the particle list. First, the list of particles is iterated through and the force of gravity is applied to each particle, then the list of springs is iterated through and the spring force is applied on each of the two particles in the spring.

The calculateForces method does the spring force and gravity calculations, changes the values in the particle structs, and then the update method adjusts the velocity and position of each particle based on the forces acting on it.

## Results

### Simple Spring

https://user-images.githubusercontent.com/48161551/118842754-78ba9b80-b897-11eb-8c2c-04ee1f8d0264.mov



### Camera Motion and Particle-Mouse Interaction

The user can rotate the camera around the origin by dragging with the left mouse down. SHIFT+Drag with the left mouse button down moves the camera towards and away from the origin.

https://user-images.githubusercontent.com/48161551/118842463-398c4a80-b897-11eb-876e-9b4e46a5c0b8.mov


### Keyboard User Interaction


**Pause and play:**

**p:** pause/play

https://user-images.githubusercontent.com/48161551/118842859-8b34d500-b897-11eb-994e-5a70a007c6ce.mov

**Adjusting spring elasticity constant:**

**k:** increase kspring

**j:** decrease kspring

https://user-images.githubusercontent.com/48161551/118842918-94be3d00-b897-11eb-9fe9-5f371d54bbd0.mov

In this demo, the spring elasticity constant starts at a low value and is increased during the demo, resulting in stiffer spring movement.

**Adjusting Mass:**

**m:** increase particle masses

**n:** decrease particle masses

https://user-images.githubusercontent.com/48161551/118842965-9daf0e80-b897-11eb-979f-64d111e656d3.mov

In this demo, the mass is increased throughout the demo so the particles slow down and appear to be getting heavier.




