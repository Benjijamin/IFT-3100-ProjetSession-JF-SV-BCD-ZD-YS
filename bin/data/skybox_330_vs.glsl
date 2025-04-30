#version 330

// attribut uniforme
uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// attributs de sommet
in vec4 position;
in vec2 texcoord;

// attribut en sortie
out vec2 surface_texcoord;

void main()
{
    vec4 pos = projectionMatrix * viewMatrix * position;
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);

    // passer les coordonnées de texture au shader de fragment
    surface_texcoord = texcoord;
}
