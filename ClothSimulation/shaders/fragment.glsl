#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_;

void main()
{
        vec4 tex = texture(texture_, TexCoords);
        vec3 lightDir = vec3(100.0, 100.0, 100.0);
        vec3 L = normalize(lightDir - FragPos);
        float dotNL = max(dot(Normal, L), 0);
        FragColor = vec4(vec3(tex*(0.5 + dotNL)), 1.0);
        
}  