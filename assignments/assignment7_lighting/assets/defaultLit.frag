//defaultLit.frag
#version 450
out vec4 FragColor;

uniform sampler2D _Texture;

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 3
uniform Light _Lights[MAX_LIGHTS];


struct Material
{
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
};
uniform Material _Material;

uniform vec3 _ViewLocation;

uniform int blinn;

vec3 calcLight(vec3 normal, Light _Light){
	float spec;

	vec3 lightDir = normalize(_Light.position - fs_in.WorldNormal);
	vec3 viewDir = normalize(_ViewLocation - fs_in.WorldNormal);

	vec3 ambient = _Material.ambientK * _Light.color;

	float diff = _Material.diffuseK * max(dot(lightDir, normal),0.0);
	vec3 diffuse = diff * _Light.color;

	

	if(blinn == 1)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), _Material.shininess);
	}
	else
	{
	    vec3 reflectDir = reflect(-lightDir, normal);
	    spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
		vec3 specular = _Light.color * spec;
		return ambient + diff + specular;
}

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 total;

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		total += calcLight(normal, _Lights[i]);
	}
	vec3 result = texture(_Texture,fs_in.UV).rgb * total;
	FragColor = vec4(result, 1.0);
}

