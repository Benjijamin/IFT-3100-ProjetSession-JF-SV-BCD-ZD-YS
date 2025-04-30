#version 330

// attributs uniformes
uniform vec4 skyColor;
uniform vec4 horizonColor;
uniform vec4 bottomColor;
uniform bool enableStars;
uniform float skyPower;
uniform float horizonPower;
uniform float bottomPower;

// attribut en entrée
in vec2 surface_texcoord;
in vec3 vDirection;

// attribut en sortie
out vec4 fragment_color;

float random(vec3 p)
{
    return fract(sin(dot(p, vec3(12.9898, 78.233, 37.719))) * 43758.5453);
}

void main()
{
    float h = normalize(vDirection).y;

    float top = clamp(h, 0, 1);
    float bot = abs(clamp(h, -1, 0));
    float horizon = 1 - (top + bot);

    float star = 0;

    if(enableStars){
        vec3 gridDir = floor(vDirection * 500) / 500;
        star = step(0.9995, random(gridDir));
    }

    fragment_color = 
        pow(top, skyPower) * skyColor + 
        pow(horizon, horizonPower) * horizonColor + 
        pow(bot, bottomPower) * bottomColor + star;
}
