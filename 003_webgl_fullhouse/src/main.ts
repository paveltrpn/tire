
const vsSource: string = `
attribute vec4 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec4 aVertexColor;

uniform mat4 uNormalMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;

varying highp vec3 vLighting;
varying lowp vec4 vColor;

void main(void) {
    gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;
    vColor = aVertexColor;

    highp vec3 ambientLight = vec3(0.3, 0.3, 0.3);
    highp vec3 directionalLightColor = vec3(1, 1, 1);
    highp vec3 directionalVector = normalize(vec3(0, 0, 1));

    highp vec4 transformedNormal = uNormalMatrix * vec4(aVertexNormal, 1.0);

    highp float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
    vLighting = ambientLight + (directionalLightColor * directional);
    //vLighting = vec3(0.5, 0.5, 0.5);
}
`;

const fsSource: string = `
varying highp vec3 vLighting;
varying lowp vec4 vColor;

void main(void) {
    gl_FragColor = vec4(vColor.rgb * vLighting, 1.0);
}
`;

const positions: number[] = [
    // top
     1.0,  1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,
    -1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0,
    // bottom
     1.0, -1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
    // front
    -1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,  1.0, -1.0,  1.0, -1.0, -1.0,  1.0,
    // back
    -1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
    // left
    -1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,
    // right
     1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0
];

const normals: number[] = [
    // top
    0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,
    0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,
    // bottom
    0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,
    0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,
    // front
    0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,
    0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,
    // back
    0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,
    0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,
    // left
   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,
   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,
    // right
    1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0
];

const colors: number[] = [
    // top (light green)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // bottom (dark green)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // front (light red)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // back  (dark red)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // left (light blue)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    // right (dark blue)
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
    0.5,  0.5,  0.5, 0.5,  0.5,  0.5, 0.5,  0.5,  0.5,
];

class cube_c {
    html_canvas: HTMLCanvasElement;
    gl: WebGLRenderingContext;

    wnd_width: number;
    wnd_height: number;
    
    gl_vert_buf: WebGLBuffer;
    gl_normal_buf: WebGLBuffer;
    gl_color_buf: WebGLBuffer;

    gl_shader: number;
    gl_programinfo: any;

    aspect: number;
    
    squareRotation: number = 0.0;
    
    constructor() {}

    setup(): void {
        this.html_canvas = document.querySelector('#glcanvas');
        this.gl = this.html_canvas.getContext('webgl',{antialias: true,
                                                       depth: true});
        
        this.wnd_width = this.gl.drawingBufferWidth;
        this.wnd_height = this.gl.drawingBufferHeight;
        this.aspect = this.wnd_width / this.wnd_height;

        if (!this.gl) {
            alert('Unable to initialize WebGL. Your browser or machine may not support it.');
            return;
        }

        this.gl_shader = this.initShaderProgram(vsSource, fsSource);

        this.gl_programinfo = {
            program: this.gl_shader,
            attribLocations: {
              vertexPosition: this.gl.getAttribLocation(this.gl_shader, 'aVertexPosition'),
              vertexNormal:   this.gl.getAttribLocation(this.gl_shader, 'aVertexNormal'),
              vertexColor:    this.gl.getAttribLocation(this.gl_shader, 'aVertexColor'),
            },
            uniformLocations: {
              projectionMatrix: this.gl.getUniformLocation(this.gl_shader, 'uProjectionMatrix'),
              modelViewMatrix:  this.gl.getUniformLocation(this.gl_shader, 'uModelViewMatrix'),
              normalMatrix:     this.gl.getUniformLocation(this.gl_shader, 'uNormalMatrix'),
            },
          };
        
        console.log(this.gl.getParameter(this.gl.VERSION));
        console.log(this.gl.getParameter(this.gl.SHADING_LANGUAGE_VERSION));
        console.log(this.gl.getParameter(this.gl.VENDOR));
        let gl_ext = this.gl.getSupportedExtensions();

        for (let i = 0; i < gl_ext.length; i++) {
            console.log(gl_ext[i]);
        }

        console.log("Message");

        this.initBuffers();
    }

    render(deltaTime: any): void {
        this.gl.viewport(0, 0, this.wnd_width, this.wnd_height);
        this.gl.clearColor(0.1, 0.1, 0.1, 1.0);  
        this.gl.clearDepth(1.0);                 
        this.gl.enable(this.gl.DEPTH_TEST);           
        this.gl.depthFunc(this.gl.LEQUAL);            

        this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);

        let projectionMatrix: mtrx4 = new mtrx4(); 
        projectionMatrix.set_perspective(deg_to_rad(45), this.aspect, 0.1,  100.0);

        let modelViewMatrix = new mtrx4();

        // Now move the drawing position a bit to where we want to
        // start drawing the square.
        modelViewMatrix.mult_translate(modelViewMatrix, [0.0, 0.0, -7.0]);

        let rot = new mtrx4();
        rot.set_axisangl(vec3_set(0.1, 0.4, 0.3), this.squareRotation);
        // let rot: mtrx4_t = mtrx4_set_euler(squareRotation, 0, squareRotation );

        modelViewMatrix.mult(rot);

        let normalMatrix = new mtrx4(); 
        normalMatrix.from(modelViewMatrix);
        normalMatrix.transpose();

        // Буфер вершин
        {
            const numComponents = 3;
            const type = this.gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_vert_buf);
            this.gl.vertexAttribPointer(
                this.gl_programinfo.attribLocations.vertexPosition,
                numComponents,
                type,
                normalize,
                stride,
                offset);
                this.gl.enableVertexAttribArray(
                    this.gl_programinfo.attribLocations.vertexPosition);
        }
        // Буфер нормалей вершин
        {
            const numComponents = 3;
            const type = this.gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_normal_buf);
            this.gl.vertexAttribPointer(
                1, // programInfo.attribLocations.vertexNormal
                numComponents,
                type,
                normalize,
                stride,
                offset);
                this.gl.enableVertexAttribArray(
                1); // programInfo.attribLocations.vertexNormal
        }
        // Буфер цвета вершин
        {
            const numComponents = 3;
            const type = this.gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_color_buf);
            this.gl.vertexAttribPointer(
                this.gl_programinfo.attribLocations.vertexColor,
                numComponents,
                type,
                normalize,
                stride,
                offset);
                this.gl.enableVertexAttribArray(
                this.gl_programinfo.attribLocations.vertexColor);
        }   

        this.gl.useProgram(this.gl_programinfo.program);

        this.gl.uniformMatrix4fv(
                this.gl_programinfo.uniformLocations.projectionMatrix,
                false,
                projectionMatrix.data);
        this.gl.uniformMatrix4fv(
                this.gl_programinfo.uniformLocations.modelViewMatrix,
                false,
                modelViewMatrix.data);
        this.gl.uniformMatrix4fv(
                this.gl_programinfo.uniformLocations.normalMatrix,
                false,
                normalMatrix.data);

        this.gl.drawArrays(this.gl.TRIANGLES, 0, 36);

        this.squareRotation += deltaTime;
    }

    private initBuffers(): void {
        this.gl_vert_buf = this.gl.createBuffer();
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_vert_buf);
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(positions), this.gl.STATIC_DRAW);

        // Normal buffer
        this.gl_normal_buf = this.gl.createBuffer();
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_normal_buf);
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(normals), this.gl.STATIC_DRAW);

        // Now set up the colors for the vertices
        this.gl_color_buf = this.gl.createBuffer();
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_color_buf);
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(colors), this.gl.STATIC_DRAW);
    }

    private initShaderProgram(vsSource: string, fsSource: string): any {
        const vertexShader = this.loadShader(this.gl.VERTEX_SHADER, vsSource);
        const fragmentShader = this.loadShader(this.gl.FRAGMENT_SHADER, fsSource);

        const shaderProgram = this.gl.createProgram();
        this.gl.attachShader(shaderProgram, vertexShader);
        this.gl.attachShader(shaderProgram, fragmentShader);
        this.gl.linkProgram(shaderProgram);

        if (!this.gl.getProgramParameter(shaderProgram, this.gl.LINK_STATUS)) {
            alert('Unable to initialize the shader program: ' + this.gl.getProgramInfoLog(shaderProgram));
            return null;
        }

        return shaderProgram;
    }

    private loadShader(type: any, source: string): any {
        const shader = this.gl.createShader(type);

        this.gl.shaderSource(shader, source);
        this.gl.compileShader(shader);

        if (!this.gl.getShaderParameter(shader, this.gl.COMPILE_STATUS)) {
          alert('An error occurred compiling the shaders: ' + this.gl.getShaderInfoLog(shader));
          this.gl.deleteShader(shader);
          return null;
        }

        return shader;
    }
}

function main() {
    let app: cube_c = new cube_c();
    let then: number = 0.0;

    app.setup();

    function render(now: any) {
        now *= 0.001;  // convert to seconds
        const deltaTime = now - then;
        then = now;
        
        app.render(deltaTime);
        
        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}

main()