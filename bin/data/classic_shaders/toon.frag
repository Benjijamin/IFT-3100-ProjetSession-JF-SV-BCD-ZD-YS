// IFT3100H24 TP2 ~ toon.frag

#version 330

in vec3 surface_position;
in vec3 surface_normal;
out vec4 fragColor;

// Matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;

// Lumière ambiante
uniform bool use_light_ambient;
uniform vec3 light_ambient;

// Directionnelle
uniform bool use_light_directional;
uniform vec3 light_directional_direction;
uniform vec3 light_directional_diffuse;

// Ponctuelle
uniform bool use_light_point;
uniform vec3 light_point_position;
uniform vec3 light_point_diffuse;
uniform float light_point_constant;
uniform float light_point_linear;
uniform float light_point_quadratic;

// Spot
uniform bool use_light_spot;
uniform vec3 light_spot_position;
uniform vec3 light_spot_direction;
uniform vec3 light_spot_diffuse;
uniform float light_spot_cutoff;
uniform float light_spot_outerCutoff;
uniform float light_spot_constant;
uniform float light_spot_linear;
uniform float light_spot_quadratic;

// Fonction d'intensite toon
float toonIntensity(float NdotL) {
    if(NdotL > 0.95) return 1.0;
    else if(NdotL > 0.5) return 0.7;
    else if(NdotL > 0.25) return 0.4;
    else return 0.1;
}

void main() {
    vec3 N = normalize(surface_normal);
    vec3 V = normalize(-surface_position); // pas utilisé ici, pas de speculaire

    vec3 result = vec3(0.0);

    // ambiante
    if(use_light_ambient) {
        result += color_ambient * light_ambient;
    }

    // directionnelle
    if(use_light_directional) {
        vec3 L = normalize(-light_directional_direction);
        float NdotL = max(dot(N, L), 0.0);
        float inten = toonIntensity(NdotL);
        result += color_diffuse * light_directional_diffuse * inten;
    }

    // ponctuelle
    if(use_light_point) {
        vec3 Lp = light_point_position - surface_position;
        float d = length(Lp);
        Lp = normalize(Lp);
        float att = 1.0 / (light_point_constant + light_point_linear * d + light_point_quadratic * d * d);
        float NdotL = max(dot(N, Lp), 0.0);
        float inten = toonIntensity(NdotL) * att;
        result += color_diffuse * light_point_diffuse * inten;
    }

    // spot
    if(use_light_spot) {
        vec3 Ls = light_spot_position - surface_position;
        float d = length(Ls);
        Ls = normalize(Ls);
        float theta = dot(Ls, normalize(-light_spot_direction));
        float eps = light_spot_cutoff - light_spot_outerCutoff;
        float intensity = clamp((theta - light_spot_outerCutoff) / eps, 0.0, 1.0);
        float att = intensity / (light_spot_constant + light_spot_linear * d + light_spot_quadratic * d * d);
        float NdotL = max(dot(N, Ls), 0.0);
        float inten = toonIntensity(NdotL) * att;
        result += color_diffuse * light_spot_diffuse * inten;
    }

    fragColor = vec4(result, 1.0);
}

