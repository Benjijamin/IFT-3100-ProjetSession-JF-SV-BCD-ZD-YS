// IFT3100H24 TP2 ~ blinn_phong.frag

#version 330

// attributs in (de .vert)
in vec3 surface_position;
in vec3 surface_normal;

// attribut en sortie
out vec4 fragment_color;

// Matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

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

void main() {
    vec3 n = normalize(surface_normal);
    vec3 v = normalize(-surface_position);

    // Ambient
    vec3 ambient = vec3(0.0);
    if(use_light_ambient) {
        ambient = color_ambient * light_ambient;
    }

    vec3 diffuse_sum = vec3(0.0);
    vec3 specular_sum = vec3(0.0);

    // Directionnelle
    if(use_light_directional) {
        vec3 L = normalize(-light_directional_direction);
        float NdotL = max(dot(n, L), 0.0);
        vec3 H = normalize(L + v);
        diffuse_sum  += color_diffuse * light_directional_diffuse * NdotL;
        specular_sum += color_specular * light_directional_diffuse * pow(max(dot(n, H), 0.0), brightness);
    }

    // Ponctuelle
    if(use_light_point) {
        vec3 Lp = light_point_position - surface_position;
        float d = length(Lp);
        Lp = normalize(Lp);
        float att = 1.0 / (light_point_constant + light_point_linear * d + light_point_quadratic * d * d);
        float NdotL = max(dot(n, Lp), 0.0);
        vec3 H = normalize(Lp + v);
        diffuse_sum  += color_diffuse * light_point_diffuse * NdotL * att;
        specular_sum += color_specular * light_point_diffuse * pow(max(dot(n, H), 0.0), brightness) * att;
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
        vec3 H = normalize(Ls + v);
        diffuse_sum  += color_diffuse * light_spot_diffuse * NdotL * att;
        specular_sum += color_specular * light_spot_diffuse * pow(max(dot(n, H), 0.0), brightness) * att;
    }

    // coulor finale
    fragment_color = vec4(ambient + diffuse_sum + specular_sum, 1.0);
}
