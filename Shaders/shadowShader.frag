#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

in vec2 TexCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float ambientAmount;

uniform vec3 cameraPos;

float calculateShadowAmount(vec4 fragPosLightSpace, vec3 lightDirection, vec3 normal){
    // Transform to range between -1 and 1.
    vec3 projectionCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Tranform to range between 0 and 1 for depthMap.
    projectionCoords = projectionCoords * 0.5 + 0.5;
    // Get closest depth from light's perspective.
    float closestDepth = texture(shadowMap, projectionCoords.xy).r;
    // Get closest depth from camera's perspective.
    float currentDepth = projectionCoords.z;

    // Define bias to prevent shadow acne.
    float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);  
    // Check if current is bigger and thereby in shadow.
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    // Ambient Light
    vec3 ambientLight = ambientAmount * color;
    // Diffuse Light
    vec3 lightDirection = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuseLight = diff * lightColor;
    // SpecularLight
    vec3 cameraDirection = normalize(cameraPos - fs_in.FragPos);
    vec3 midpoint = normalize(lightDirection + cameraDirection);
    float spec = pow(max(dot(normal, midpoint), 0.0), 64.0);
    vec3 specularLight = spec * lightColor;

    // Shadow
    float shadowAmount = calculateShadowAmount(fs_in.FragPosLightSpace, lightDirection, normal);

    vec3 lighting = ((1.0 - shadowAmount) * (diffuseLight + specularLight) + ambientLight) * color;
    FragColor = vec4(lighting, 1.0);
}
