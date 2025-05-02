// IFT3100H24 TP2 ~ phong.vert

#version 330

// attributs de sommet
in vec4 position;
in vec4 normal;

// attributs en sortie
out vec3 surface_position;
out vec3 surface_normal;

// uniformes matrices
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

void main() {
    // calcul de la normale en eye space
    mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));
    surface_normal = vec3(normalMatrix * normal);

    // position en eye space
    surface_position = vec3(modelViewMatrix * position);

    // position écran
    gl_Position = projectionMatrix * modelViewMatrix * position;
}
