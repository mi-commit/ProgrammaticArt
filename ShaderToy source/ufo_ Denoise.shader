//BY LuckyBalla
// https://www.shadertoy.com/view/NlccR8

#define STEPS 26
#define DIFF 6.0
#define RADIUS 16.0

#define pi 3.14159265359
#define pi2 2.0 * pi

uint hash(inout uint x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    
    return x;
}

//https://www.shadertoy.com/view/WsBBR3 
float randomFloat(inout uint state) {
    return float(hash(state)) / 4294967296.0;
} 

vec2 randomDir(inout uint state) {
    float z = randomFloat(state) * 2.0 - 1.0;
    float a = randomFloat(state) * pi2;
    float r = sqrt(1.0f - z * z);
    float x = r * cos(a);
    float y = r * sin(a);
    return vec2(x, y);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    vec2 uv = fragCoord / iResolution.xy;
    //vec2 m = iMouse.xy / iResolution.xy;
    //if(m.x <= 0.0 &&  m.y <= 0.0) m.x = .5;

    vec2 radius = (RADIUS / iResolution.xy);
    float diff = DIFF / 255.0;
    vec3 pixel = texture(iChannel0, uv).xyz;                  
    vec3 result = vec3(0.0, 0.0, 0.0);
    float totalWeight = 0.0;
    uint seed = uint(54321);


    for(int i = 0; i < STEPS; i++) {
        vec2 dir = randomDir(seed).xy * radius;
        vec3 randomPixel = texture(iChannel0, uv + dir).xyz;
        vec3 delta = randomPixel - pixel;
        float weight = exp(-dot(delta, delta) / diff);
        result += randomPixel * weight;
        totalWeight += weight;
    }
    
    
    result = result / totalWeight;    
    fragColor = vec4( result, 1.0);  
    //fragColor += step(abs(m.x - uv.x), 0.001);
    if(uv.y > 0.5) fragColor = vec4(pixel,1);
}
   