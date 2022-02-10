#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;



void main()
{
       
        vec3 lightDir = vec3(100.0, 100.0, 100.0);
        vec3 L = normalize(lightDir - FragPos);
        float dotNL = max(dot(Normal, L), 0);
        FragColor = vec4(vec3((0.5 + dotNL)), 1.0);
        
}  