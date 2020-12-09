
// Глобальный WebGL контекст
let gl: WebGL2RenderingContext;

class cube_c {
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

    setup(canvas: string, text_field: string): void {
        let html_canvas: HTMLCanvasElement = document.querySelector(canvas);
        gl = html_canvas.getContext('webgl2',{antialias: true, depth: true});
        
        this.wnd_width = gl.drawingBufferWidth;
        this.wnd_height = gl.drawingBufferHeight;
        this.aspect = this.wnd_width / this.wnd_height;

        if (!gl) {
            alert('cube_c::setup(): Unable to initialize WebGL. Your browser or machine may not support it.');
            return;
        }

        this.gl_shader = this.initShaderProgram(vsSource, fsSource);

        this.gl_programinfo = {
            program: this.gl_shader,
            attribLocations: {
              vertexPosition: gl.getAttribLocation(this.gl_shader, 'aVertexPosition'),
              vertexNormal:   gl.getAttribLocation(this.gl_shader, 'aVertexNormal'),
              vertexColor:    gl.getAttribLocation(this.gl_shader, 'aVertexColor'),
            },
            uniformLocations: {
              projectionMatrix: gl.getUniformLocation(this.gl_shader, 'uProjectionMatrix'),
              modelViewMatrix:  gl.getUniformLocation(this.gl_shader, 'uModelViewMatrix'),
              normalMatrix:     gl.getUniformLocation(this.gl_shader, 'uNormalMatrix'),
            },
          };
        
        let log_out = document.getElementById(text_field);
        log_out.innerText = gl.getParameter(gl.VERSION) + "; " + 
                            gl.getParameter(gl.SHADING_LANGUAGE_VERSION) + "; " +  
                            gl.getParameter(gl.VENDOR);

        let gl_ext = gl.getSupportedExtensions();

        for (let i = 0; i < gl_ext.length; i++) {
            log_out.innerText = log_out.innerText + (gl_ext[i]) + " ;";
        }

        this.initBuffers();
    }

    render(deltaTime: any): void {
        gl.viewport(0, 0, this.wnd_width, this.wnd_height);
        gl.clearColor(0.1, 0.1, 0.1, 1.0);  
        gl.clearDepth(1.0);                 
        gl.enable(gl.DEPTH_TEST);           
        gl.depthFunc(gl.LEQUAL);            

        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

        let projectionMatrix: mtrx4 = new mtrx4(); 
        projectionMatrix.setPerspective(degToRad(45), this.aspect, 0.1,  100.0);

        let modelViewMatrix = new mtrx4();

        let transMtrx = new mtrx4;
        transMtrx.setTranslate(new vec3(0.0, 0.0, -7.0));
        modelViewMatrix.mult(transMtrx);

        let fooQtnn = new qtnn();
        fooQtnn.setAxisAngl(new vec3(0.1, 0.4, 0.3), this.squareRotation);
        let rot = new mtrx4();
        rot.fromQtnn(fooQtnn);

        modelViewMatrix.mult(rot);

        let normalMatrix = new mtrx4(modelViewMatrix); 
        normalMatrix.transpose();

        // Буфер вершин
        {
            const numComponents = 3;
            const type = gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            gl.bindBuffer(gl.ARRAY_BUFFER, this.gl_vert_buf);
            gl.vertexAttribPointer(
                this.gl_programinfo.attribLocations.vertexPosition,
                numComponents,
                type,
                normalize,
                stride,
                offset);
                gl.enableVertexAttribArray(
                    this.gl_programinfo.attribLocations.vertexPosition);
        }
        // Буфер нормалей вершин
        {
            const numComponents = 3;
            const type = gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            gl.bindBuffer(gl.ARRAY_BUFFER, this.gl_normal_buf);
            gl.vertexAttribPointer(
                1, // programInfo.attribLocations.vertexNormal
                numComponents,
                type,
                normalize,
                stride,
                offset);
                gl.enableVertexAttribArray(
                1); // programInfo.attribLocations.vertexNormal
        }
        // Буфер цвета вершин
        {
            const numComponents = 3;
            const type = gl.FLOAT;
            const normalize = false;
            const stride = 0;
            const offset = 0;
            gl.bindBuffer(gl.ARRAY_BUFFER, this.gl_color_buf);
            gl.vertexAttribPointer(
                this.gl_programinfo.attribLocations.vertexColor,
                numComponents,
                type,
                normalize,
                stride,
                offset);
                gl.enableVertexAttribArray(
                this.gl_programinfo.attribLocations.vertexColor);
        }   

        gl.useProgram(this.gl_programinfo.program);

        gl.uniformMatrix4fv(
                this.gl_programinfo.uniformLocations.projectionMatrix,
                false,
                projectionMatrix.data);
        gl.uniformMatrix4fv(
                this.gl_programinfo.uniformLocations.modelViewMatrix,
                false,
                modelViewMatrix.data);
        gl.uniformMatrix4fv(
                this.gl_programinfo.uniformLocations.normalMatrix,
                false,
                normalMatrix.data);

        gl.drawArrays(gl.TRIANGLES, 0, 36);

        this.squareRotation += deltaTime;
    }

    private initBuffers(): void {
        this.gl_vert_buf = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.gl_vert_buf);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

        // Normal buffer
        this.gl_normal_buf = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.gl_normal_buf);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);

        // Now set up the colors for the vertices
        this.gl_color_buf = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, this.gl_color_buf);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    }

    private initShaderProgram(vsSource: string, fsSource: string): any {
        const vertexShader = this.loadShader(gl.VERTEX_SHADER, vsSource);
        const fragmentShader = this.loadShader(gl.FRAGMENT_SHADER, fsSource);

        const shaderProgram = gl.createProgram();
        gl.attachShader(shaderProgram, vertexShader);
        gl.attachShader(shaderProgram, fragmentShader);
        gl.linkProgram(shaderProgram);

        if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
            alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
            return null;
        }

        return shaderProgram;
    }

    private loadShader(type: any, source: string): any {
        const shader = gl.createShader(type);

        gl.shaderSource(shader, source);
        gl.compileShader(shader);

        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
          alert('An error occurred compiling the shaders: ' + gl.getShaderInfoLog(shader));
          gl.deleteShader(shader);
          return null;
        }

        return shader;
    }
}

function onLoad(rq: string) {
    console.log("Nice load")
}

function loadWorld() {
    var request = new XMLHttpRequest();
    request.open("GET", "README.md");
    request.onreadystatechange = function() {
      if (request.readyState == 4) {
        onLoad(request.responseText);
      }
    }
    request.send();
  }

function loadWorldFetch() {
    fetch('../README.md', {mode: 'no-cors'})
    .then(response => response.text())
    .then(data => console.log(data))
    .catch(error => console.error(error));
}

function main() {
    let app: cube_c = new cube_c();
    let then: number = 0.0;

    app.setup("#glcanvas", "log_out");

    loadWorldFetch()

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