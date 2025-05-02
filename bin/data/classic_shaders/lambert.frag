// IFT3100H24 TP2 ~ lambert.frag

#version 330

// attributs in (de .vert)
in vec3 surface_position;
in vec3 surface_normal;

// attribut en sortie
out vec4 fragment_color;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;

// lumière ambiante (couleur)
uniform bool use_light_ambient;
uniform vec3 light_ambient;

// lumière directionnelle
uniform bool use_light_directional;
uniform vec3 light_directional_direction;
uniform vec3 light_directional_diffuse;

// lumière ponctuelle
uniform bool use_light_point;
uniform vec3 light_point_position;
uniform vec3 light_point_diffuse;
uniform float light_point_constant;
uniform float light_point_linear;
uniform float light_point_quadratic;

// lumière spot
uniform bool use_light_spot;
uniform vec3 light_spot_position;
uniform vec3 light_spot_direction;
uniform vec3 light_spot_diffuse;
uniform float light_spot_cutoff;
uniform float light_spot_outerCutoff;
uniform float light_spot_constant;
uniform float light_spot_linear;
uniform float light_spot_quadratic;

void main() {
    vec3 n = normalize(surface_normal);

    // accumulateur pour diffus et ambient
    vec3 ambient = vec3(0.0);
    vec3 diffuse_sum = vec3(0.0);

    // ambiant
    if(use_light_ambient) {
        ambient = color_ambient * light_ambient;
    }

    // directionnelle
    if(use_light_directional) {
        vec3 L = normalize(-light_directional_direction);
        float NdotL = max(dot(n, L), 0.0);
        diffuse_sum += color_diffuse * light_directional_diffuse * NdotL;
    }
    // ponctuelle
    if(use_light_point) {
        vec3 Lp = light_point_position - surface_position;
        float d = length(Lp);
        Lp = normalize(Lp);
        float att = 1.0 / (light_point_constant + (light_point_linear * d) + (light_point_quadratic * d * d));
        float NdotL = max(dot(n, Lp), 0.0);
        diffuse_sum += color_diffuse * light_point_diffuse * NdotL * att;
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
        float NdotL = max(dot(n, Ls), 0.0);
        diffuse_sum += color_diffuse * light_spot_diffuse * NdotL * att;
    }

    fragment_color = vec4(ambient + diffuse_sum, 1.0);
}
