/*
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D DIFFUSE1;
//uniform sampler2D SPECULAR1;

void main()
{    
    //colorD = texture(DIFFUSE1, TexCoords);
    //colorS = texture(SPECULAR1, TexCoords);

    //FragColor = colorD + colorS;

    FragColor = texture(DIFFUSE1, TexCoords);
}
*/

#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;

void main()
{    
    FragColor = texture(texture1, TexCoords);
}