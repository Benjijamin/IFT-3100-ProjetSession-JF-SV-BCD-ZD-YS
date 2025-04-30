#version 330

// attributs uniformes
uniform sampler2D skyboxTexture;

// attribut en entrée
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

void main()
{
    vec4 col = texture(skyboxTexture, surface_texcoord); 
    fragment_color = col;
}
