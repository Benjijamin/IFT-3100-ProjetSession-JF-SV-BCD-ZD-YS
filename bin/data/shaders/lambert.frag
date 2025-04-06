// IFT3100H24 TP2 ~ lambert.frag

#version 330

// attributs interpol�s � partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;

// attribut en sortie
out vec4 fragment_color;

// couleurs de r�flexion du mat�riau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;

// position d'une source de lumi�re
uniform vec3 light_position;

void main()
{
  // re-normaliser la normale apr�s interpolation (n)
  vec3 n = normalize(surface_normal);

  // calculer la direction de la surface vers la lumi�re (l)
  vec3 l = normalize(light_position - surface_position);

  // calculer le niveau de r�flexion diffuse (n � l)
  float reflection_diffuse = max(dot(n, l), 0.0);

  // d�terminer la couleur du fragment
  fragment_color = vec4(color_ambient + color_diffuse * reflection_diffuse, 1.0);
}
