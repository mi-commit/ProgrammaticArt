#define FOV_CORRECT .5

//coordinates are x-right y-up z-forwards
//aligned to the canvas

//smoothed minimum function
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

float map(vec3 p){
    p = p - vec3((iMouse.xy / iResolution.xy)*20.- 10.,-tan(iTime) + 10.);
    
    float torus = sdTorus(p,vec2(1,.25 ));

    vec3 spherePos = p;
    float sphere = sd_Sphere(p, .7);
    
    
    return smin(sphere, torus, .2); // return distance from unit sphere
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
    for (int i = 0; i < 80; i++){
        vec3 pos = rayOrigin + rayDirection * distanceTravelled; //current position of ray
    
        float d = map(pos);
        distanceTravelled += d;

        //optimization, just make sure we dont do too many iterations or anything
        //color = vec3(i) / 80.;
        
        if (d < .01) break;
        if ( d > 50.) break;

    }
    
    
    //colorize
    color = vec3(distanceTravelled * .05);

    fragColor = vec4(color, 1);
}