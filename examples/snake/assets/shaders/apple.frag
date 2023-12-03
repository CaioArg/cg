#version 300 es

precision mediump float;

in vec2 fragTexCoord;

in vec3 fragTObj;
in vec3 fragBObj;
in vec3 fragNObj;
in vec3 fragLEye;
in vec3 fragVEye;

uniform mat3 normalMatrix;

uniform vec4 Ia, Id, Is;
uniform vec4 Ka, Kd, Ks;
uniform float shininess;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

out vec4 outColor;

mat3 ComputeTBN(vec3 TObj, vec3 BObj, vec3 NObj) {
  vec3 TEye = normalMatrix * normalize(TObj);
  vec3 BEye = normalMatrix * normalize(BObj);
  vec3 NEye = normalMatrix * normalize(NObj);

  return mat3(TEye.x, BEye.x, NEye.x,
              TEye.y, BEye.y, NEye.y,
              TEye.z, BEye.z, NEye.z);
}

vec4 BlinnPhong(vec3 N, vec3 L, vec3 V, vec2 texCoord) {
  N = normalize(N);
  L = normalize(L);

  float lambertian = max(dot(N, L), 0.0);

  float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    vec3 H = normalize(L + V);
    float angle = max(dot(H, N), 0.0);
    specular = pow(angle, shininess);
  }

  vec4 map_Kd = texture(diffuseTex, texCoord);
  vec4 map_Ka = map_Kd;

  vec4 diffuseColor = map_Kd * Kd * Id * lambertian;
  vec4 specularColor = Ks * Is * specular;
  vec4 ambientColor = map_Ka * Ka * Ia;

  return ambientColor + diffuseColor + specularColor;
}

void main() {
  mat3 TBN = ComputeTBN(fragTObj, fragBObj, fragNObj);

  vec3 LTan = TBN * normalize(fragLEye);
  vec3 VTan = TBN * normalize(fragVEye);
  vec3 NTan = texture(normalTex, fragTexCoord).xyz;
  NTan = normalize(NTan * 2.0 - 1.0);

  vec4 color = BlinnPhong(NTan, LTan, VTan, fragTexCoord);

  if (gl_FrontFacing) {
    outColor = color;
  } else {
    float i = (color.r + color.g + color.b) / 3.0;
    outColor = vec4(i, 0, 0, 1.0);
  }
}
