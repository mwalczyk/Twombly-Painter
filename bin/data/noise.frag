#version 150

uniform float time;
out vec4 outputColor;

float random(float p) {
    return fract(sin(p)*10000.);
}

float noise(vec2 p) {
    return random(p.x + p.y*10000.);
}

vec2 sw(vec2 p) {return vec2( floor(p.x) , floor(p.y) );}
vec2 se(vec2 p) {return vec2( ceil(p.x)  , floor(p.y) );}
vec2 nw(vec2 p) {return vec2( floor(p.x) , ceil(p.y)  );}
vec2 ne(vec2 p) {return vec2( ceil(p.x)  , ceil(p.y)  );}

float smoothNoise(vec2 p) {
    vec2 inter = smoothstep(0., 1., fract(p));
    float s = mix(noise(sw(p)), noise(se(p)), inter.x);
    float n = mix(noise(nw(p)), noise(ne(p)), inter.x);
    return mix(s, n, inter.y);
    return noise(nw(p));
}

float movingNoise(vec2 p) {
    float total = 0.0;
    total += smoothNoise(p     - time * 2.); //Multiplying time by a float speeds up the animation
    total += smoothNoise(p*2.  + time * 2.) / 2.;
    total += smoothNoise(p*4.  - time * 2.) / 4.;
    total += smoothNoise(p*8.  + time * 2.) / 8.;
    total += smoothNoise(p*16. - time * 2.) / 16.;
    total /= 1. + 1./2. + 1./4. + 1./8. + 1./16.;
    return total;
}

float nestedNoise(vec2 p) {
    float x = movingNoise(p);
    float y = movingNoise(p + 100.);
    return movingNoise(p + vec2(x, y));
}

void main() {
    vec2 p = (gl_FragCoord.xy / 500.) * 60.; //This number "zooms" in/out of the noise
    float brightness = nestedNoise(p);
    outputColor.rgb = vec3(brightness);
    outputColor.a = 1.;
}