
    varying vec2 vTextureCoord;//Semana6
    uniform sampler2D uSampler;//Semana6
    
    void main(void) {
        gl_FragColor =  texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));//Semana6
    }