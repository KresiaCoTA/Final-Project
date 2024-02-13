#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 5



in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform sampler2D u_texture;
uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

// function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    //vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    vec3 result = vec3(0,0,0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    // vec3 test = vec3(texture(material.diffuse, TexCoords));
    // result = pointLights[0].ambient*vec3(1000,1000,1000);
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(pointLights[0].ambient * vec3(test),1);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    vec3 lightDir_1   = normalize(light.position - fragPos);
    vec3 viewDir_1    = normalize(viewPos - fragPos);
    vec3 halfwayDir_1 = normalize(lightDir_1 + viewDir_1);
    float spec = pow(max(dot(normal, halfwayDir_1), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_texture, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_texture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_texture, TexCoords));
    ambient *= attenuation;;
    diffuse *= attenuation;;
    specular *= attenuation;
    return (ambient+diffuse+specular);
}