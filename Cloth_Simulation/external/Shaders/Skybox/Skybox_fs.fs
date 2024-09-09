#version 330 core

out vec4 FragColor;
in vec3 TexCoords;
in vec3 world_position;

uniform vec3 camera;
uniform samplerCube skybox;
uniform mat4 view ;


float compute_fog()
{
    vec3 viewSpacePosition = vec3(view * vec4(world_position, 1.0f));
    float base_fog_density = 0.5f;
    float baseHeight = 0.5 ;
    float heightAboveBase = max(0.0, world_position.y - baseHeight);
    float heightFactor = exp(-heightAboveBase * 2.0f );
    float distance = 2.2 * length(vec2(viewSpacePosition.x, viewSpacePosition.z));
    float fog = exp(-base_fog_density * distance * distance * heightFactor);
    fog = clamp(fog, 0.0, 1.0);
    return fog;
}



//float compute_fog()
//{
//    vec3 viewSpacePosition = vec3(view * vec4(world_position, 1.0f)) ;
//    float base_fog_density = 0.5f ;
//    float distance = 2.2*length(vec2(viewSpacePosition.x, viewSpacePosition.z));
//    float fog = exp(-base_fog_density * distance * distance);
//    fog = clamp(fog, 0.0, 1.0);
//    return fog;
//}

//float compute_fog()
//{
//    float fog_density = 0.5f ;
//    float distance = length(camera) ;
////    float vertical_gradient = 1.0 - clamp(world_position.y, 0.0, 1.0) ;
////    vertical_gradient = vertical_gradient * 0.8 ;
////    float fog = exp(-pow(fog_density * distance * vertical_gradient, 2));
//    float fog = exp(-pow(fog_density * distance, 2));
//    fog = clamp(fog, 0.0, 1.0);
//    return fog;
//}

//float compute_fog()
//{
//    float fog_density = 0.5f ;
//    float distance = length(world_position - camera);
//    float fog = exp(-pow(fog_density * distance, 2)) ;
//    fog = clamp(fog, 0.0, 1.0);
//    return fog;
//}

//float compute_fog()
//{
//    float fog_density = 0.5f ;
//    float distance = length(world_position - camera);
//    float fog = exp(-pow(fog_density * distance, 2));
//    fog = clamp(fog, 0.0, 1.0);
//    return fog;
//}


// Example adjustment function that reduces fog density with distance to simulate a lighter horizon.
// This function can be refined based on your specific visual requirements.
//float someAdjustmentFunction(float distance)
//{
//    // Parameters to control the adjustment curve.
//    float startFalloff = 100.0; // Start reducing density after this distance.
//    float endFalloff = 500.0; // End reducing density by this distance.
//
//    // Linearly interpolate the fog density reduction factor between startFalloff and endFalloff.
//    float factor = clamp((distance - startFalloff) / (endFalloff - startFalloff), 0.0, 1.0);
//
//    // Adjust the factor or return a modified density based on your needs.
//    // Here, we simply return a linear interpolation between full density and a reduced density.
//    return mix(1.0, 0.5, factor); // Adjust the second parameter to control the minimum density.
//}
//
//
//float compute_fog()
//{
//    // Base fog density. Consider making this a variable that can be adjusted outside the function for more control.
//    float base_fog_density = 0.5f;
//    
//    // Calculate the distance from the camera to the current fragment's world position.
//    float distance = length(world_position - camera);
//
//    // Optional: Adjust fog density based on distance or other factors.
//    // This could include reducing density at further distances or applying a gradient.
//    float adjusted_fog_density = base_fog_density * someAdjustmentFunction(distance);
//
//    // Compute the fog factor using the adjusted density.
//    // The exponential function can be tuned to change how quickly the fog intensity increases with distance.
//    float fog = exp(-pow(adjusted_fog_density * distance, 2.0));
//
//    // Clamp the fog value to ensure it remains within the [0, 1] range.
//    fog = clamp(fog, 0.0, 1.0);
//
//    return fog;
//}
//

void main()
{
    vec4 color = texture(skybox, TexCoords);
    color.rgb = color.rgb * pow(2.0, -2.0f);
    color.rgb = color.rgb / (color.rgb + vec3(1.0));
    color.rgb = pow(color.rgb, vec3(1.0/2.2));
    FragColor = color ;
//    FragColor = mix(vec4(0.5f, 0.5f, 0.5f, 1.0),color,compute_fog()) ;
}
