#version 400

layout (location = 0) in vec3 vPos;

uniform vec3 uCameraPos;
uniform float uSize;
uniform vec3 uOffset;
uniform vec4 uColor;
uniform mat4 uVP;
uniform bool isLine; 
uniform vec3 uUp;

out vec4 color;
out vec2 uv;

void main()
{
   color = uColor;
   if (isLine) // line drawing stuff
   {
      gl_Position = uVP * vec4(vPos, 1.0); 
   } else {
      uv = vPos.xy;

      // translate to center around origin
      vec3 moveToOrigin = vec3(-0.5, -0.5, 0.0);
      vec3 pos = vPos + moveToOrigin;

      // scale
      pos.x = pos.x * uSize;
      pos.y = pos.y * uSize;
      pos.z = pos.z * uSize;

      // rotate
      //vec3 up = vec3(0.0, 1.0, 0.0);
      vec3 rotZ = normalize(uCameraPos - pos);
      vec3 rotX = normalize(cross(uUp, rotZ));
      vec3 rotY = normalize(cross(rotZ, rotX));

      mat3 rotation = mat3(rotX, rotY, rotZ);
      pos = rotation * pos;

      // translate to center around uOffset
      pos = pos + uOffset;
      
      gl_Position = uVP * vec4(pos, 1.0);
   }
}
