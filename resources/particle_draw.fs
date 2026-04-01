#version 430

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

void main() {
    // Map texture coordinates from [0.0, 1.0] to [-1.0, 1.0]
    vec2 uv = fragTexCoord * 2.0 - 1.0;

    // Discard pixels outside particle radius
    float distSquared = dot(uv, uv);
    if (distSquared > 1.0) discard;

    finalColor = fragColor;
}
