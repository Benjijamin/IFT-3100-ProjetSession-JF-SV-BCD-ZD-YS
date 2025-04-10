#version 330

// attributs uniformes
uniform sampler2D image;
uniform float saturationValue;
uniform float brightnessValue;
uniform float contrastValue;


// attribut en entrée
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

mat4 saturationMat()
{
  //approx. luminance percue
  vec3 lum = vec3(0.33, 0.5, 0.16);

  float sat = 1.0 - saturationValue;

  //???
  vec3 red = vec3(lum.x * sat);
  red += vec3(saturationValue, 0, 0);
  vec3 green = vec3(lum.y * sat);
  green += vec3(0, saturationValue, 0);
  vec3 blue = vec3(lum.z * sat);
  blue += vec3(0, 0, saturationValue);

  return mat4(vec4(red, 0), vec4(green, 0), vec4(blue, 0), vec4(0,0,0,1));
}

mat4 brightnessMat()
{
  return mat4( 
    vec4( 1, 0, 0, 0 ),
    vec4( 0, 1, 0, 0 ),
    vec4( 0, 0, 1, 0 ),
    vec4( brightnessValue, brightnessValue, brightnessValue, 1)
  );
}

mat4 contrastMat()
{
  float t = (1.0 - contrastValue) / 2.0;

  return mat4(
    vec4( contrastValue, 0, 0, 0 ),
    vec4( 0, contrastValue, 0, 0 ),
    vec4( 0, 0, contrastValue, 0 ),
    vec4( t, t, t, 1 )
  );
}

void main()
{
  // échantillonner la texture
  vec4 texture_sample = texture(image, surface_texcoord);

  // couleur finale du fragment
  fragment_color = brightnessMat() * contrastMat() * saturationMat() * texture_sample;
}
