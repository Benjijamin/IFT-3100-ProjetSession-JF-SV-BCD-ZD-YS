#version 330

// attributs uniformes
uniform sampler2D image;
uniform vec2 resolution;
uniform vec4 tint;
uniform float saturationValue;
uniform float brightnessValue;
uniform float contrastValue;
uniform float blurValue;

// attribut en entrée
in vec2 surface_texcoord;

// attribut en sortie
out vec4 fragment_color;

mat4 saturationMat()
{
  //approx. luminance percue
  vec3 lum = vec3(0.33, 0.5, 0.16);

  float sat = 1.0 - saturationValue;

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
  float TAU = 6.2831;

  vec4 texture_sample = texture(image, surface_texcoord);

  vec4 filtered_color = mix(texture_sample, tint, 0.25);

  vec2 radius = blurValue / resolution;

  for(float d = 0.0; d < TAU; d += TAU/16.0)
  {
    for(float i = 0.0 ; i < 1; i += 0.25)
    {
      filtered_color += texture(image, surface_texcoord + vec2(cos(d), sin(d)) * radius * i);
    }
  }

  filtered_color /= 49.0;

  fragment_color = brightnessMat() * contrastMat() * saturationMat() * filtered_color;
}
