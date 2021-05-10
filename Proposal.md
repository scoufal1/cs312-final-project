For my final project, I plan to implement a mass-spring physical simulation. In previous assignments, we have worked with user interaction, such as the ability to move the camera with the mouse. We have also done physical simulation, where we update the location of a particle based on its velocity. This demo will be unique because it will incorporate several features from existing demos, such as user interaction and physical simulation. It will also be more challenging because the position of the particle will also be dependent on other variables such as force and mass.

# Features

## Minimum required:
The minimum required features for this demo will be a basic physical simulation of a mass-spring system that allows the user to click and drag the mass in order to stretch or compress the spring, and then simulate the movement of the mass.

## Good-to-have:
Once the basic simulation is implemented, I plan to implement the movable camera, so that the user can rotate the camera around the origin and change the distance from the origin to the camera, in addition to being able to click and drag the mass. For this to be successful, the mass-spring system will have to be 3-dimensional. I also plan to add more user interaction, so that the user can change the spring elasticity, as well as increase or decrease the mass.

## Stretch:
Networks of masses connected by deformable springs are often used to model more complex structures such as hair and cloth. If I have time, I would like to implement one of these more complicated systems using springs.

# Algorithms, data structures and/or background math

A spring is an example of an n-ary force, which means the force is applied to a fixed set of particles. The force exerted on the particle is calculated using Hooke’s law:

<img width="500" alt="Screen Shot 2021-05-09 at 9 43 54 PM" src="https://user-images.githubusercontent.com/48161551/117595762-abce9380-b10f-11eb-90b8-7e993755676b.png">
 
The force exerted on the mass depends on the spring elasticity constant (kspring), the length that spring is either compressed or stretched (||Δx||), the resting length of the spring at equilibrium (r). The damping force, which opposes the motion and is due to the energy lost due to resistive forces and is dependent on the damping coefficient (kdamp) the velocity of the particle dotted with the normalized distance vector (Δv • Δx). The whole term in the bracket represents the displacement, which is multiplied by the normalized direction vector to determine the direction of the force. 

Similarly to our particle system demo, the particle will be stored in a struct that contains information about position, velocity, force, and mass. Each frame, the position of the particle will be recalculated. In this case, however, the velocity of the particle is also changing each frame, which is determined by the force exerted on the particle and the mass of the particle. The Newton-Euler equations of motion are used to calculate the new velocities and positions:

```
vel = vel + Δt * (force/mass)

pos = pos + Δt * vel
```

The update method will do these calculations, change the values in the particle struct, and then display the mass at its new position.

