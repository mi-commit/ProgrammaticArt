#define FOV_CORRECT .5



//colorpalette with a cosine function as developed by I. Quilez
// https://iquilezles.org/articles/palettes/
vec3 palette( in float t)
{
    vec3 a = vec3(.6, .5, .5);
    vec3 b = vec3(.5,.5,.6);
    vec3 c = vec3( .2,.5,0);
    vec3 d = vec3(1,0,1);
    return a + b*cos(/*(.11) */6.28318*(c*t+d) );
}

//SDF:S BY I.Quilez
// *****************
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
//*****************************
float ground(vec3 p){
    float h = -p.y + fract(p.y);
    


    return h+5.;
}



float map(vec3 p){
    //Previous setup to control UFO with mouse, doenst work as good due to space repetition
    //vec3 ufo_pos = vec3((iMouse.xy / iResolution.xy)*20.- 10.,sin(iTime));  
    vec3 ufo_pos = vec3(1,-5,0);
    vec3 q = p; //save original pos
    
    p.z += iTime ; // forwards movement
    
    p.x -=5.+ .333 * sin(.001*float(iFrame));//extra swaying movement of ufos
    //"space" repetitions
    //in reality just looping the ray in a set area
    p.x = mod(p.x, 10.)-5.; 
    p.z = mod(p.z, 8.)-4.;
    p.y = mod(p.y, 21.)-10.5;
    
    //ufo
    float torus = sdTorus(p-ufo_pos,vec2(1,.25 ));
    float sphere = sd_Sphere(p-ufo_pos, .7);
    float c_dist = sdCone(p - vec3(0, -10, 0), vec2(.5,.05), .01);
    float ufod = min(sphere, torus); // return distance from unit sphere
    
    //q.y + 4 is the floor
    float g = ground(q);
    return min(ufod, min(g, c_dist));
    
    
    
    
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
    
    float i;
    //march the ray
    for (i = 0.; i < 80.; i++){
        vec3 pos = rayOrigin + rayDirection * distanceTravelled; //current position of ray
        pos.xz += .1*sin(distanceTravelled);  // wiggle ray for extra variation

        float d = map(pos); // distance to map scenery
        distanceTravelled += d;

        //optimization, just make sure we dont do too many iterations or anything;
        if (d < .01 || distanceTravelled > 200.) break;
    }
    //colorize
    color = palette(distanceTravelled * .035 - i*0.05)-.1;
    //color = vec3(distanceTravelled*.01 + .005*i); // b&w version



    fragColor = vec4(color, 1);
}