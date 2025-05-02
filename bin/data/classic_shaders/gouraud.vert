#version 330

// attributs de sommet
in vec4 position;
in vec4 normal;

// attribut en sortie pour le fragment
out vec3 surface_color;

// uniformes matrices
uniform mat4x4 modelViewMatrix;
uniform mat4x4 projectionMatrix;

// matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

// lumière ambiante
uniform bool use_light_ambient;
uniform vec3 light_ambient;

// directionnelle
uniform bool use_light_directional;
uniform vec3 light_directional_direction;
uniform vec3 light_directional_diffuse;

// ponctuelle
uniform bool use_light_point;
uniform vec3 light_point_position;
uniform vec3 light_point_diffuse;
uniform float light_point_constant;
uniform float light_point_linear;
uniform float light_point_quadratic;

// spot
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
    // calcul de la normale et de la position en espace vue
    mat4x4 normalMatrix = transpose(inverse(modelViewMatrix));
    vec3 n   = normalize(vec3(normalMatrix * normal));
    vec3 pos = vec3(modelViewMatrix * position);

    vec3 V = normalize(-pos);

    // accumulateur pour diffus + spéculaire
    vec3 diffuse_accum  = vec3(0.0);
    vec3 specular_accum = vec3(0.0);
    vec3 ambient = vec3(0.0);


    // - Ambient -
    if(use_light_ambient) {
        ambient = color_ambient * light_ambient;
    }

    // — Directionnelle —
    if(use_light_directional) {
        vec3 L = normalize(-light_directional_direction);
        float NdotL = max(dot(n, L), 0.0);
        diffuse_accum  += color_diffuse * light_directional_diffuse * NdotL;
        vec3 R = reflect(-L, n);
        specular_accum += color_specular * light_directional_diffuse * pow(max(dot(R, V), 0.0), brightness);
    }
    
    // — Ponctuelle —
    if(use_light_point) {
        vec3 Lp = light_point_position - pos;
        float d = length(Lp);
        Lp = normalize(Lp);
        float att = 1.0 / (light_point_constant + light_point_linear * d + light_point_quadratic * d * d);
        float NdotL = max(dot(n, Lp), 0.0);
        diffuse_accum  += color_diffuse * light_point_diffuse * NdotL * att;
        vec3 R = reflect(-Lp, n);
        specular_accum += color_specular * light_point_diffuse * pow(max(dot(R, V), 0.0), brightness) * att;
    }
    
    // — Spot —
    if(use_light_spot) {
        vec3 Ls = light_spot_position - pos;
        float d = length(Ls);
        Ls = normalize(Ls);
        float theta = dot(Ls, normalize(-light_spot_direction));
        float eps = light_spot_cutoff - light_spot_outerCutoff;
        float intensity = clamp((theta - light_spot_outerCutoff) / eps, 0.0, 1.0);
        float att = intensity / (light_spot_constant + light_spot_linear * d + light_spot_quadratic * d * d);
        float NdotL = max(dot(n, Ls), 0.0);
        diffuse_accum  += color_diffuse * light_spot_diffuse * NdotL * att;
        vec3 R = reflect(-Ls, n);
        specular_accum += color_specular * light_spot_diffuse * pow(max(dot(R, V), 0.0), brightness) * att;
    }

    // combinaison finale
    surface_color = ambient + diffuse_accum + specular_accum;

    // projection
    gl_Position = projectionMatrix * modelViewMatrix * position;
}
