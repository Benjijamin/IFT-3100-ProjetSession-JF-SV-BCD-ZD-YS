// IFT3100H24 TP2 ~ toon.frag

#version 330

// Entrée depuis le vertex shader
in vec3 surface_normal;
in vec3 surface_position;

// position d'une source de lumière
uniform vec3 light_position;

// couleurs de réflexion du matériau
uniform vec3 color_diffuse;
uniform vec3 color_ambient;

// Sortie fragment
out vec4 fragColor;

void main() {

    // re-normaliser la normale après interpolation (n)
    vec3 N = normalize(surface_normal);

    // calculer la direction de la surface vers la lumière (l)
    vec3 L = normalize(light_position - surface_position);

    // calculer le niveau de réflexion diffuse (n • l)
    float NdotL = max(dot(N, L), 0.0);

    // Intensité de la lumière
    float intensity;

    // atténuation des intensités selon la valeur de (N • L)

    // cas 1
    if (NdotL > 0.95) intensity = 1.0;

    // cas 2
    else if (NdotL > 0.5) intensity = 0.7;
    
    // cas 3
    else if (NdotL > 0.25) intensity = 0.4;
    
    else
        intensity = 0.1;

    vec3 toonColor = (color_ambient + color_diffuse) * intensity;
    fragColor = vec4(toonColor, 1.0);
}
