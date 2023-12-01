#version 330 core

out vec4 FragColor;
in vec3 TexCoords;
in vec3 world_position;

uniform vec3 camera;
uniform samplerCube skybox;

float compute_fog2()
{
    float fog_density = 0.1f ;
    float distance = 4*length(world_position - camera);
    float vertical_gradient = 1.0 - clamp(world_position.y, 0.0, 1.0) ;
    vertical_gradient = vertical_gradient * 0.8 ;
    float fog = exp(-pow(fog_density * distance * vertical_gradient, 2));
    fog = clamp(fog, 0.0, 1.0);
    return fog;
}
//float compute_fog()
//{
//    float fogDensity = 0.1f;
//    float z = gl_FragCoord.z / gl_FragCoord.w;  // Depth value
//    float fogFactor = exp(-pow(fogDensity * z, 2));
//    return clamp(fogFactor, 0.0, 1.0);
//}

//float compute_fog1()
//{
//    float fogDensity = 0.5f;
//    float height = 100*(- world_position.y + camera.y) ;
//    float fogFactor = exp(-pow(fogDensity * height, 2));
//    return clamp(fogFactor, 0.0, 1.0);
//}

float sampleFog(vec3 rayStart, vec3 rayDir)
{
    float totalFog = 0.0;
    float stepSize = 0.1 ;
    for (float t = 0.0; t < 1.0; t += stepSize){
        vec3 samplePoint = rayStart + rayDir * t;
        totalFog += 0.5 * stepSize;
    }
    return totalFog;
}

void main()
{
    vec3 rayStart = camera;
    vec3 rayDir = normalize(world_position - camera);
    float fog = sampleFog(rayStart, rayDir);
    vec4 color = texture(skybox, TexCoords);
//    color.rgb = color.rgb * pow(2.0, -2.0f);
//    color.rgb = color.rgb / (color.rgb + vec3(1.0));
//    color.rgb = pow(color.rgb, vec3(1.0/2.2));
    FragColor = mix(vec4(0.5f, 0.5f, 0.5f, 1.0),color,compute_fog2()) ;
//    FragColor = color ;
}
