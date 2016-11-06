#version 150

in vec2 TexCoord;

out float outColor;

uniform sampler2D ssao;
const int blurSize = 4; 

void main() 
{
   vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));
   float result = 0.0;
   for (int x = 0; x < blurSize; ++x) 
   {
      for (int y = 0; y < blurSize; ++y) 
      {
         vec2 offset = (vec2(-2.0) + vec2(float(x), float(y))) * texelSize;
         result += texture(ssao, TexCoord + offset).r;
      }
   }
 
   outColor = result / float(blurSize * blurSize);
}