
// 
// Конец секции с математикой
// 

var squareRotation = 0.0;

function main() {
    const canvas: HTMLCanvasElement = document.querySelector('#glcanvas');
    const gl: WebGLRenderingContext = canvas.getContext('webgl',{ antialias: true,
                                                                  depth: true,
                                                                  preserveDrawingBuffer: true});

    if (!gl) {
        alert('Unable to initialize WebGL. Your browser or machine may not support it.');
        return;
    }

    const vsSource = `
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

    const fsSource = `
        varying highp vec3 vLighting;
        varying lowp vec4 vColor;

        void main(void) {
            gl_FragColor = vec4(vColor.rgb * vLighting, 1.0);
        }
    `;

    // Initialize a shader program; this is where all the lighting
    // for the vertices and so forth is established.
    const shaderProgram = initShaderProgram(gl, vsSource, fsSource);

    // Collect all the info needed to use the shader program.
    // Look up which attributes our shader program is using
    // for aVertexPosition, aVevrtexColor and also
    // look up uniform locations.
    
    const programInfo = {
        program: shaderProgram,
        attribLocations: {
          vertexPosition: gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
          vertexNormal: gl.getAttribLocation(shaderProgram, 'aVertexNormal'),
          vertexColor: gl.getAttribLocation(shaderProgram, 'aVertexColor'),
        },
        uniformLocations: {
          projectionMatrix: gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
          modelViewMatrix: gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
          normalMatrix: gl.getUniformLocation(shaderProgram, 'uNormalMatrix'),
        },
      };

  // Here's where we call the routine that builds all the
  // objects we'll be drawing.
  const buffers = initBuffers(gl);

  var then = 0;

    // Draw the scene repeatedly
    function render(now) {
        now *= 0.001;  // convert to seconds
        const deltaTime = now - then;
        then = now;
        
        drawScene(gl, programInfo, buffers, deltaTime);
        
        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}

function initBuffers(gl) {
    // Create a buffer for the square's positions.
    const positionBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    const positions = [
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
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

    // Normal buffer
    const normalBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);
    const normals = [
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
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);

    // Now set up the colors for the vertices
    const colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    const colors = [
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
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);

    return {
        position: positionBuffer,
        normal: normalBuffer,
        color: colorBuffer,
    };
}

function drawScene(gl, programInfo, buffers, deltaTime) {
    gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);
    gl.clearColor(1.0, 1.0, 1.0, 1.0);  
    gl.clearDepth(1.0);                 
    gl.enable(gl.DEPTH_TEST);           
    gl.depthFunc(gl.LEQUAL);            

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
    let projectionMatrix = mtrx4_set_perspective(deg_to_rad(45), aspect, 0.1,  100.0);
    let modelViewMatrix = mtrx4_set_idtt();

    // Now move the drawing position a bit to where we want to
    // start drawing the square.
    modelViewMatrix = mtrx4_mult_translate(modelViewMatrix, [0.0, 0.0, -7.0]);

    let rot: mtrx4_t = mtrx4_set_axisangl(vec3_set(0.1, 0.4, 0.3), squareRotation);
    // let rot: mtrx4_t = mtrx4_set_euler(squareRotation, 0, squareRotation );
    modelViewMatrix = mtrx4_mult(modelViewMatrix, rot);

    let normalMatrix = mtrx4_invert(modelViewMatrix);
    normalMatrix = mtrx4_transpose(normalMatrix);

    // Буфер вершин
    {
        const numComponents = 3;
        const type = gl.FLOAT;
        const normalize = false;
        const stride = 0;
        const offset = 0;
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
        gl.vertexAttribPointer(
            programInfo.attribLocations.vertexPosition,
            numComponents,
            type,
            normalize,
            stride,
            offset);
        gl.enableVertexAttribArray(
            programInfo.attribLocations.vertexPosition);
    }
    // Буфер нормалей вершин
    {
        const numComponents = 3;
        const type = gl.FLOAT;
        const normalize = false;
        const stride = 0;
        const offset = 0;
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.normal);
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
        gl.bindBuffer(gl.ARRAY_BUFFER, buffers.color);
        gl.vertexAttribPointer(
            programInfo.attribLocations.vertexColor,
            numComponents,
            type,
            normalize,
            stride,
            offset);
        gl.enableVertexAttribArray(
            programInfo.attribLocations.vertexColor);
    }   

    gl.useProgram(programInfo.program);

    gl.uniformMatrix4fv(
        programInfo.uniformLocations.projectionMatrix,
        false,
        projectionMatrix);
    gl.uniformMatrix4fv(
        programInfo.uniformLocations.modelViewMatrix,
        false,
        modelViewMatrix);
    gl.uniformMatrix4fv(
        programInfo.uniformLocations.normalMatrix,
        false,
        normalMatrix);

    gl.drawArrays(gl.TRIANGLES, 0, 36);

    squareRotation += deltaTime;
}

function initShaderProgram(gl, vsSource, fsSource) {
    const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
    const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

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

function loadShader(gl, type, source) {
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

main();