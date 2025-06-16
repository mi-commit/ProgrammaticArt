#define FOV_CORRECT .5
#define GRASS_SCALE  1.5

//TODO MAYBE ADD WAY TO MOVE CAMERA FOR SECRET1???





//intresting visuals i discovered while messing around with values
//remove comment ("//" at start of line)to view
 //#define secret1

#define noiseFixes //comment this line out if yoou want to see how much i had to compensate for noise
                    //do the same in mainImage as well for the full experience


//******************************************************************
//colorpalette with a cosine function as developed by I. Quilez
// https://iquilezles.org/articles/palettes/

//specific palettes were made by me but the function itself was not
vec3 palette( in float t)
{
    vec3 a = vec3(.6, .5, .5);
    vec3 b = vec3(.5,.5,.6);
    vec3 c = vec3( .2,.5,0);
    vec3 d = vec3(1,0,1);
    return a + b*cos(/*(.11) */6.28318*(c*t+d) );
}
vec3 grassPallette(in float t){
     vec3 a = vec3(.1, .3, .1);
    vec3 b = vec3(.3,.5,.4);
    vec3 c = vec3( .2,.5,0);
    vec3 d = vec3(0,1,1);
    return a + b*cos(6.28318*(c*t+d) ); 
}
//************************************************************************
//Signed distance functions BY I.Quilez
// https://iquilezles.org/articles/distfunctions/
// take in a point in space and return a distance to shape
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
//***************************************************************************


// started as an attempt to create a more dynamic ground than a simple plane, 
//turned out quite intresting when i decided to try out what a multiplied fract would do to it
//looks kinda like a tall grass field blowing in the wind to me, a really nice effect
//massive pain as well as it then forced me to figure out a way to denoise it but oh well #worthit
float grass(vec3 p){
        return p.y +fract(p.x* GRASS_SCALE )*100. + .6; ;    
    //fract thing actually breaks up the ground    
}


float map(vec3 p){
    vec3 ufo_pos = vec3(1,-5,0);
    vec3 q = p; //save original ray pos
    
    p.z += iTime ; // forwards movement
    
    p.x -=5.+ .333 * sin(.001*float(iFrame));//extra swaying movement of ufos
    
    //*****************************
    //"space" repetitions
    //in reality just looping the ray in a set area
    p.x = mod(p.x, 10.)-5.; 
    p.z = mod(p.z, 8.)-4.;
    p.y = mod(p.y, 21.)-10.5;
    //******************************
    
    //ufo
    float torus = sdTorus(p-ufo_pos,vec2(1,.25 ));
    float sphere = sd_Sphere(p-ufo_pos, .7);
    float c_dist = sdCone(p - vec3(0, -10, 0), vec2(.5,.05), .01);
    float ufod = min(min(sphere, torus), c_dist); // return distance from unit sphere
    
    //applies all the space screwery also to the grass if enabled
    //under the levels of things going on in the grass by itself
    #ifdef secret1
    q = p;
    #endif
    
    float g = grass(q);
    
    return min(ufod, g);
    
}

//--------------------------------------------------------------------------
// Merge grass into the sky background for correct fog colouring...
//fog method is based on the one dave hoskins used here https://www.shadertoy.com/view/Xsf3zX
//although i also modified the arguments for my use, as well as blending into a purple that fades nicely into both the grass and the sky
//the purpose here was not as much to improve realism but instead to help reduce the noisyness of distant grass
//at this it helps a lot, but is not enough

vec3 ApplyFog( in vec3  rgb, in float dis, float y)
{
	float fogAmount = clamp(dis*dis* 0.00003 * y, 0.0, 1.0);
	return mix( rgb, vec3(.6,.41,.7), fogAmount );
}
//--------------------------------------------------------------------------


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
    if(uv.y > 0. && distanceTravelled > 10.){
    color = palette(distanceTravelled * .035 - i*0.05)-.1;// using i here helps bring some extra highlights on edges
    }
    else color = vec3(.5,0,.5) + distanceTravelled*.005;// reduces colors and simplifies the result to look nicer
    #ifndef noiseFixes
color = palette(distanceTravelled * .035 - i*0.05)-.1;//horrible, left here as comparison
    #endif
    vec3 grassC = grassPallette(distanceTravelled * .035 - i*0.05 - uv.y* .5);
    
    color = mix( grassC,color, clamp(uv.y+.5,0.,1.));
    
    #ifdef noiseFixes
    //fog to reduse noise in the distance
    float fogMask = clamp( .85 - abs(uv.y + .1),0.,2.);//mask for fog, helps main detail in sky
    color = ApplyFog(color, distanceTravelled, fogMask);
    #endif
    fragColor = vec4(color, 1);
}