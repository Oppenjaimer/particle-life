#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;

struct Particle {
    vec2 position;
    vec2 previous_position;
    uint type;
    uint padding;
};

// Particles array (binding 0)
layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

// Uniforms
uniform mat4 mvp;
uniform float particle_radius;
uniform vec4 colors[8];

out vec2 fragTexCoord;
out vec4 fragColor;

void main() {
    Particle p = particles[gl_InstanceID];
    vec2 local_pos = vertexPosition.xy * particle_radius * 2.0;
    vec2 world_pos = p.position + local_pos;

    // Apply camera perspective (mvp)
    gl_Position = mvp * vec4(world_pos, 0.0, 1.0);

    // Pass data to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = colors[p.type];
}
