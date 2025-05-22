// Cosine based palette, 4 vec3 params
vec3 palette( in float t)
{
    vec3 a = vec3(0.8,0.5,0.4);
    vec3 b = vec3(0.2,0.4,0.2);
    vec3 c = vec3(2.0,1.0,1.0);
    vec3 d = vec3(0.0,0.25,0.25);
    return a + b*cos( 6.28318*(c*t+d) );
    //t, a, b, c, d
}


vec3 pattern1(vec2 uv){
    float stepValue = 0.1;
    float x  = smoothstep(0., stepValue, length(uv.x));
    float y = smoothstep(0., stepValue, length(uv.y));
    
    vec2 test = vec2(fract(-x),fract(-y*2.));
    
    return palette(length(test)*iTime*23.);
}
vec3 centerSquareMask(vec2 uv, vec2 uv0){
    vec3 color = vec3(0);
    float x = clamp (uv.x,-1.,1.);
    x = step(.75, .8 - abs(x));
    
    float y = clamp (uv.y, -1.,1.);
    y = step(.75, .8 - abs(y));
    uv = fract(uv*3.3)-.5;
    
    color += palette(5./(iTime*.0001*(length(uv))) ) *x*y;
    return color;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord*2. - iResolution.xy)/iResolution.y;
    vec2 uv0 = uv;
    uv *=.1;
    vec3 test = pattern1(uv);
    

    float r = clamp(test.r,0.,1.);
    vec3 square = centerSquareMask(uv, uv0);
    
    //fragColor.r = square.r;
    fragColor.rgb = test.rgb;


}