// IFT3100H24 TP2 ~ gouraud.frag

#version 330

// attribut interpol� � partir des valeurs en sortie du shader de sommet
in vec3 surface_color;

// attribut en sortie
out vec4 fragment_color;

void main()
{
  // calculer la couleur du fragment
  fragment_color = vec4(surface_color, 1.0);
}
