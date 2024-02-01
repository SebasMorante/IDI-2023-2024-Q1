#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform vec3 colFocusC;
uniform vec3 posFocusC;

uniform vec3 colFocusM;
uniform vec3 posFocusM1;
uniform vec3 posFocusM2;

uniform mat4 View;

uniform mat4 TGFM1;
uniform mat4 TGFM2;

uniform bool Escena2;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  // Pasar pos de les 2 llums de model a SCO
  vec3 posFocusM1 = (View * TGFM1 * vec4(posFocusM1, 1.0)).xyz;
  vec3 posFocusM2 = (View * TGFM2 * vec4(posFocusM2, 1.0)).xyz;


  vec3 NM = normalize(fnormal);

  vec3 LCSO = normalize(posFocusC.xyz - fvertex.xyz);

  // Calcular L en SCO
  vec3 LM1 = normalize(posFocusM1 - fvertex.xyz);
  vec3 LM2 = normalize(posFocusM2 - fvertex.xyz);

  vec3 fcolor;

  if (Escena2){
    fcolor = Ambient() + Difus(NM, LM1, colFocusM) + Especular(NM, LM1, fvertex, colFocusM) 
                            + Difus(NM, LM2, colFocusM) + Especular(NM, LM2, fvertex, colFocusM);
  }
  else {
    fcolor = Ambient() + Difus(NM, LCSO, colFocusC) + Especular(NM, LCSO, fvertex, colFocusC) 
                          + Difus(NM, LM1, colFocusM) + Especular(NM, LM1, fvertex, colFocusM) 
                          + Difus(NM, LM2, colFocusM) + Especular(NM, LM2, fvertex, colFocusM);
  }

  FragColor = vec4(fcolor, 1);
}
