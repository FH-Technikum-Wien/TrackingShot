#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

in vec2 TexCoord;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;

uniform vec3 textureColor;


uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float focus;


uniform vec3 lightColor;
uniform float lightIntensity;
uniform float ambientLightAmount;

uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform float bumpiness;

float calculateShadowAmount(vec4 fragPosLightSpace, vec3 lightDirection, vec3 normal){
    // Transform to range between -1 and 1.
    vec3 projectionCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Tranform to range between 0 and 1 for depthMap.
    projectionCoords = projectionCoords * 0.5 + 0.5;
    
    // If outside of far plane -> no shadow
    if(projectionCoords.z > 1.0)
        return 0.0;

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
    // Clamp light
    float maxLight = ambientStrength + diffuseStrength + specularStrength;
    float ambient = ambientStrength / maxLight;
    float diffuse = diffuseStrength / maxLight;
    float specular = specularStrength / maxLight;

    // Calculate vectors
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDirection = normalize(lightPos - fs_in.FragPos);
    vec3 cameraDirection = normalize(cameraPos - fs_in.FragPos);
    vec3 midpoint = normalize(lightDirection + cameraDirection);

    // Shadow
    float shadowAmount = calculateShadowAmount(fs_in.FragPosLightSpace, lightDirection, normal);

    // Get normal from normalMap
    normal = (texture(normalMap, fs_in.TexCoords).rgb * 2.0 - 1.0);
    normal.xy *= bumpiness;
    normal = normalize(fs_in.TBN * normalize(normal));
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    // Main texture color
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb * textureColor;

    // Ambient Light
    vec3 ambientLight = ambient * ambientLightAmount * color;

    // Diffuse Light
    vec3 diffuseLight = max(0.0, dot(lightDirection, normal)) * diffuse * lightColor;

    // SpecularLight
    vec3 specularLight = (pow(max(0.0, dot(cameraDirection, reflectionDirection)), focus) * specular) * lightIntensity * lightColor;


    vec3 lighting = ((1.0 - shadowAmount) * (diffuseLight + specularLight) + ambientLight) * color;
    FragColor = vec4(lighting, 1.0);
}
