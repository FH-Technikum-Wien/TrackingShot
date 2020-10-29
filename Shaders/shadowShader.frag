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
uniform vec3 textureColor;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float focus;

uniform sampler2D shadowMap;

uniform vec3 lightColor;
uniform float lightIntensity;
uniform float ambientLightAmount;

uniform vec3 lightPos;
uniform vec3 cameraPos;

float calculateShadowAmount(vec4 fragPosLightSpace, vec3 lightDirection, vec3 normal){
    // Transform to range between -1 and 1.
    vec3 projectionCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Tranform to range between 0 and 1 for depthMap.
    projectionCoords = projectionCoords * 0.5 + 0.5;
    
    // If outside of far plane -> no shadow
    //if(projectionCoords.z > 1.0)
    //    return 0.0;

    // Get closest depth from light's perspective.
    float closestDepth = texture(shadowMap, projectionCoords.xy).r;
    // Get closest depth from camera's perspective.
    float currentDepth = projectionCoords.z;

    // Define bias to prevent shadow acne.
    float bias = max(0.05 * (1.0 - dot(normal, lightDirection)), 0.005);  
    // Check if current is bigger and thereby in shadow.
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    /*
    // PCF - Multiple shadow cehcks with offsets.
    float shadow = 0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projectionCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    */

    return shadow;
}

void main()
{
    float maxLight = ambientStrength + diffuseStrength + specularStrength;
    float ambient = ambientStrength / maxLight;
    float diffuse = diffuseStrength / maxLight;
    float specular = specularStrength / maxLight;

    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDirection = normalize(lightPos - fs_in.FragPos);
    vec3 cameraDirection = normalize(cameraPos - fs_in.FragPos);
    vec3 midpoint = normalize(lightDirection + cameraDirection);

    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb * textureColor;

    // Ambient Light
    vec3 ambientLight = ambient * ambientLightAmount * color;

    // Diffuse Light
    vec3 diffuseLight = max(0.0, dot(lightDirection, normal)) * diffuse * lightColor;
    // SpecularLight
    vec3 specularLight = (pow(max(0.0, dot(normal, midpoint)), focus) * specular) * lightIntensity * lightColor;

    // Shadow
    float shadowAmount = calculateShadowAmount(fs_in.FragPosLightSpace, lightDirection, normal);

    vec3 lighting = ((1.0 - shadowAmount) * (diffuseLight + specularLight) + ambientLight) * color;
    FragColor = vec4(lighting, 1.0);
}
