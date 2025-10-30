#define FOV_CORRECT .5


float dot2( in vec2 v ) { return dot(v,v); }
float dot2( in vec3 v ) { return dot(v,v); }
float ndot( in vec2 a, in vec2 b ) { return a.x*b.x - a.y*b.y; }
float glintFactor = 0.;
vec4 lightcolor=vec4 (.6,0,.5,1);

vec3 SpherePos = vec3(0,0,1);
vec3 sunPos = vec3(0,0,0);

float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}
float map(vec3 p){
    return sdSphere(p-SpherePos, 2.);
}
float shadow(vec3 p){
    vec3 toPoint = normalize(p-SpherePos);
    //p = vector to point
    vec3 sunRay = p-sunPos;
    float lightfactor = -dot(sunRay,toPoint);
    lightfactor = clamp(lightfactor*.2,.1,.9);
    
    glintFactor = clamp(1.-length(cross(toPoint,sunRay)),0.,1.);
    
    float test = dot(-toPoint, p-vec3(0,0,-4));
    if(test > 1.) glintFactor = 0.;
    float test2 = clamp(dot(sunRay, p-vec3(0,0,-4)),0.,1.);
    
    glintFactor *= test2;
    return lightfactor;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    //tranform coordinates to clip space ( in a range of [-1, 1])
    vec2 uv = (fragCoord * 2.-iResolution.xy) / iResolution.y;

    //init ray
    vec3 rayOrigin = vec3(0,0,-4); //defines each rays starting point (camera position)
    vec3 rayDirection = normalize(vec3(uv.x * FOV_CORRECT, uv.y * FOV_CORRECT,  1.)); //initial direction of the ray
    float distanceTravelled = 0.;
    vec3 color = vec3(1);
    sunPos.x = sin(iTime);
    sunPos.z = cos(iTime);
    //sunPos.y = sin(.01*iTime+5.)*2.;
    sunPos = sunPos * 9.;
    float i;
    //march the ray
    for (i = 0.; i < 80.; i++){
        vec3 pos = rayOrigin + rayDirection * distanceTravelled; //current position of ray
        float d = map(pos); // distance to map scenery
        distanceTravelled += d;

        //optimization, just make sure we dont do too many iterations or anything;
        
        if (d < .01){
            color = vec3(shadow(pos));
            glintFactor += i*.01;
            break;
        }
        if(d>100.){
            color = vec3(clamp(dot(pos-rayOrigin, sunPos), 0., 1.));
            lightcolor=vec4(.1);glintFactor = 0.;
            
            break;
        }
    }
    
    fragColor = vec4(clamp(distanceTravelled,0.,1.))*vec4(color,1.)+vec4(.1)+vec4(glintFactor);
    fragColor = fragColor*lightcolor;
    
    
}