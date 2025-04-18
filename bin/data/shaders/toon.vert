// IFT3100H24 TP2 ~ toon.vert

#version 330

// Attributs des sommets
in vec4 position;
in vec4 normal;

// Sortie vers le fragment shader
out vec3 surface_position;
out vec3 surface_normal;

// Uniformes matrices
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

void main() {
    // calcul de la matrice normale en eye space
    mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));
    surface_normal   = vec3(normalMatrix * normal);

    // position en eye space
    surface_position = vec3(modelViewMatrix * position);

    // projection finale
    gl_Position = projectionMatrix * modelViewMatrix * position;
}
