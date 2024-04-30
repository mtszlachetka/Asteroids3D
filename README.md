# CG course project: "Asteroids 3D"

3D arcade game written in C++ and OpenGL using glfw, glm, assimp, SOIL and FreeType.

## Game mechanics
The player pilots a spaceship near a space probe and must destroy incoming asteroids to defend it.
Collisions with other objects deal damage to both the probe and the player.

### Steering
- **W** - move forward
- **Mouse** - turn around
- **Q/E** - tilt left/right
- **LMB** - shoot
- **LSHIFT** - boost
- **LAlt** - evasion
- **LCtrl** - turn around&deg;

## Implemented CG methods:
- Quaternion rotations
- PBR with normal mapping
- HDR
- Shadow mapping
- Animations with Bezier curves and slerp
- Newton dynamics (damage from collisions depending on momentum)
- Geometry shader for explosions
- Particle system with instancing
- Bloom
- Collisions with bounding spheres and OBB
- Sprite rendering (HUD)

<br />

![image](screenshot.png "Title")
