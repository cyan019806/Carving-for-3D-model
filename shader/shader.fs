#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
	vec3 Normal;
	vec3 TexCoords;
    vec3 Color;
} fs_in;

uniform bool tex;
uniform int texType;
uniform sampler3D texturemap0;
uniform sampler3D texturemap1;
uniform sampler3D texturemap2;
uniform sampler3D texturemap3D;


void main()
{
    vec3 texcoord = fs_in.TexCoords;
    if(texcoord.x == 1) texcoord.x -= 0.0001;
    if(texcoord.y == 1) texcoord.y -= 0.0001;
    if(texcoord.z == 1) texcoord.z -= 0.0001;
    if(tex){
        if(texType == 0)
            FragColor = texture(texturemap0, texcoord);
        if(texType == 1)
            FragColor = texture(texturemap1, texcoord);
        if(texType == 2)
            FragColor = texture(texturemap2, texcoord);
        if(texType == 6)
            FragColor = texture(texturemap3D, texcoord);
        if(fs_in.TexCoords.x < 0.0 || fs_in.TexCoords.x > 1.0 || fs_in.TexCoords.y < 0.0 || fs_in.TexCoords.y > 1.0){
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    }else{
        FragColor = vec4(fs_in.Color, 1.0);
    }

}