#define FOV_CORRECT .5

//coordinates are x-right y-up z-forwards
//aligned to the canvas


// Cosine based palette, 4 vec3 params
//by i quilez
vec3 palette( in float t)
{
    vec3 a = vec3(.5, .5, .5);
    vec3 b = vec3(.5,.5,.5);
    vec3 c = vec3( .2,.5,0);
    vec3 d = vec3(1,0,1);
    return a + b*cos(/*(.11) */6.28318*(c*t+d) );
    //t, a, b, c, d
}
//smoothed minimum function, also by i.quilez
float smin (float a, float b, float t){
    float h = max(t - abs (a-b), 0.)/t;
    return min(a,b) - h*h*h*t*(1./6.);

}
//sdf for a torus from https://iquilezles.org/articles/distfunctions/
float sdTorus( vec3 p ,vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float sd_Sphere(vec3 p, float s){
    return length(p) -s;
}

float sdCone( vec3 p, vec2 c, float h )
{
  float q = length(p.xz);
  return max(dot(c.xy,vec2(q,p.y)),-h-p.y);
}


float map(vec3 p){
    //vec3 ufo_pos = vec3((iMouse.xy / iResolution.xy)*20.- 10.,sin(iTime));
    vec3 q = p;
    vec3 ufo_pos = vec3(0,-5,0);
    p.z += iTime ;
    p.x -=5.+ .333 * sin(.001*float(iFrame));
    p.x = mod(p.x, 10.)-5.;

    p.z = mod(p.z, 8.)-4.;
    p.y = mod(p.y, 21.)-10.5;
    float torus = sdTorus(p-ufo_pos,vec2(1,.25 ));

    vec3 spherePos = p;
    float sphere = sd_Sphere(p-ufo_pos, .7);
    
    float c_dist = sdCone(p - vec3(0, -10, 0), vec2(.5,.05), .01);
    float ufod = smin(sphere, torus, .2); // return distance from unit sphere
    return min(ufod, smin(q.y+4., c_dist, .1));
}



void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    //tranform coordinates to clip space ( in a range of [-1, 1])
    vec2 uv = (fragCoord * 2.-iResolution.xy) / iResolution.y;

    //init ray
    vec3 rayOrigin = vec3(0,0,-4); //defines each rays starting point (camera position)
    vec3 rayDirection = normalize(vec3(uv.x * FOV_CORRECT, uv.y * FOV_CORRECT,  1.)); //initial direction of the ray
    float distanceTravelled = 0.;
    vec3 color = vec3(0);
    
    //march the ray
    float i;
    
    //march the ray
    for (i = 0.; i < 80.; i++){
        vec3 pos = rayOrigin + rayDirection * distanceTravelled; //current position of ray
        pos.xz += .1*sin(distanceTravelled);  // wiggle ray

        float d = map(pos);
        distanceTravelled += d;

        //optimization, just make sure we dont do too many iterations or anything;
        if (d < .01) break;
        if ( distanceTravelled > 200.) break;
    }
    //distanceTravelled = min(distanceTravelled, 2000.);
    distanceTravelled *= .7;
    //color = vec3(distanceTravelled*.01 + .005*i);
    color = palette(distanceTravelled * .05 - i*0.05)-.1;
    
    fragColor = vec4(color, 1);
}