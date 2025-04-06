// IFT3100H24 TP2 ~ gouraud.vert

#version 330

// attributs de sommet
in vec4 position;
in vec4 normal;

// attributs en sortie
out vec3 surface_color;

// attributs uniformes
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

// couleurs de r�flexion du mat�riau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// facteur de brillance sp�culaire du mat�riau
uniform float brightness;

// position d'une source de lumi�re
uniform vec3 light_position;

void main()
{
  // calculer la matrice normale
  mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));

  // transformation de la normale du sommet dans l'espace de vue
  vec3 surface_normal = vec3(normalMatrix * normal);

  // transformation de la position du sommet dans l'espace de vue
  vec3 surface_position = vec3(modelViewMatrix * position);

  // re-normaliser la normale
  vec3 n = normalize(surface_normal);

  // calculer la direction de la surface vers la lumi�re (l)
  vec3 l = normalize(light_position - surface_position);

  // calculer le niveau de r�flexion diffuse (n � l)
  float reflection_diffuse = max(dot(n, l), 0.0);

  // r�flexion sp�culaire par d�faut
  float reflection_specular = 0.0;

  // calculer la r�flexion sp�culaire seulement s'il y a r�flexion diffuse
  if (reflection_diffuse > 0.0)
  {
    // calculer la direction de la surface vers la cam�ra (v)
    vec3 v = normalize(-surface_position);

    // calculer la direction de la r�flection (v) du rayon incident (-l) en fonction de la normale (n)
    vec3 r = reflect(-l, n);

    // calculer le niveau de r�flexion sp�culaire (r � v)
    reflection_specular = pow(max(dot(v, r), 0.0), brightness);
  }

  // calculer la couleur du fragment
  surface_color = vec3(
    color_ambient +
    color_diffuse * reflection_diffuse +
    color_specular * reflection_specular);

  // transformation de la position du sommet par les matrices de mod�le, vue et projection
  gl_Position = projectionMatrix * modelViewMatrix * position;
}
