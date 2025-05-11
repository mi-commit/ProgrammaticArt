// Cosine based palette, 4 vec3 params
vec3 palette( in float t)
{
    vec3 a = vec3(0.75, .1, 1.);
    vec3 b = vec3(3 , -2 , 2.9);
    vec3 c = vec3( .2,1,0);
    vec3 d = vec3(2,1,5);
    return a + b*cos( 6.28318*(c*t+d) );
    //t, a, b, c, d
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    //dont entirely understand how this transform works but ok
    vec2 uv = (fragCoord*2. - iResolution.xy)/iResolution.y;
    vec2 uv0 = uv; 
    vec3 finalCol = vec3(0);
        for( float i =  0.; i < 3.; i++){
        uv = fract(uv*1.2)-.5;
        
        float d = length (uv) - 0.5;
        
        vec3 col = palette(length(uv0) - 0.5*length(uv));
        
        d = sin(d* 5. -iTime*1.5+i)/ 5.;
        d=abs(d);
        //d = smoothstep(0.0, 0.1, d);
        d=0.005/d;
        finalCol += col*d;
    }
    fragColor = vec4(finalCol ,1);
    
}