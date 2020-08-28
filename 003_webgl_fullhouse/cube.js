var vsSource = "\nattribute vec4 aVertexPosition;\nattribute vec3 aVertexNormal;\nattribute vec4 aVertexColor;\n\nuniform mat4 uNormalMatrix;\nuniform mat4 uModelViewMatrix;\nuniform mat4 uProjectionMatrix;\n\nvarying highp vec3 vLighting;\nvarying lowp vec4 vColor;\n\nvoid main(void) {\n    gl_Position = uProjectionMatrix * uModelViewMatrix * aVertexPosition;\n    vColor = aVertexColor;\n\n    highp vec3 ambientLight = vec3(0.3, 0.3, 0.3);\n    highp vec3 directionalLightColor = vec3(1, 1, 1);\n    highp vec3 directionalVector = normalize(vec3(0, 0, 1));\n\n    highp vec4 transformedNormal = uNormalMatrix * vec4(aVertexNormal, 1.0);\n\n    highp float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);\n    vLighting = ambientLight + (directionalLightColor * directional);\n    //vLighting = vec3(0.5, 0.5, 0.5);\n}\n";
var fsSource = "\nvarying highp vec3 vLighting;\nvarying lowp vec4 vColor;\n\nvoid main(void) {\n    gl_FragColor = vec4(vColor.rgb * vLighting, 1.0);\n}\n";
var positions = [
    1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0,
    -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0,
    1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0,
    -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
    -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0,
    -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0,
    1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
    -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0,
    -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0,
    1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
    1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0
];
var normals = [
    0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
    0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
    0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
    0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0,
    -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
    1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
    1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0
];
var colors = [
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
];
var cube_c = (function () {
    function cube_c() {
        this.squareRotation = 0.0;
    }
    cube_c.prototype.setup = function () {
        this.html_canvas = document.querySelector('#glcanvas');
        this.gl = this.html_canvas.getContext('webgl', { antialias: true,
            depth: true });
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
                vertexNormal: this.gl.getAttribLocation(this.gl_shader, 'aVertexNormal'),
                vertexColor: this.gl.getAttribLocation(this.gl_shader, 'aVertexColor')
            },
            uniformLocations: {
                projectionMatrix: this.gl.getUniformLocation(this.gl_shader, 'uProjectionMatrix'),
                modelViewMatrix: this.gl.getUniformLocation(this.gl_shader, 'uModelViewMatrix'),
                normalMatrix: this.gl.getUniformLocation(this.gl_shader, 'uNormalMatrix')
            }
        };
        console.log(this.gl.getParameter(this.gl.VERSION));
        console.log(this.gl.getParameter(this.gl.SHADING_LANGUAGE_VERSION));
        console.log(this.gl.getParameter(this.gl.VENDOR));
        var gl_ext = this.gl.getSupportedExtensions();
        for (var i = 0; i < gl_ext.length; i++) {
            console.log(gl_ext[i]);
        }
        console.log("Message");
        this.initBuffers();
    };
    cube_c.prototype.render = function (deltaTime) {
        this.gl.viewport(0, 0, this.wnd_width, this.wnd_height);
        this.gl.clearColor(0.1, 0.1, 0.1, 1.0);
        this.gl.clearDepth(1.0);
        this.gl.enable(this.gl.DEPTH_TEST);
        this.gl.depthFunc(this.gl.LEQUAL);
        this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);
        var projectionMatrix = mtrx4_set_perspective(deg_to_rad(45), this.aspect, 0.1, 100.0);
        var modelViewMatrix = mtrx4_set_idtt();
        modelViewMatrix = mtrx4_mult_translate(modelViewMatrix, [0.0, 0.0, -7.0]);
        var rot = mtrx4_set_axisangl(vec3_set(0.1, 0.4, 0.3), this.squareRotation);
        modelViewMatrix = mtrx4_mult(modelViewMatrix, rot);
        var normalMatrix = mtrx4_invert(modelViewMatrix);
        normalMatrix = mtrx4_transpose(normalMatrix);
        {
            var numComponents = 3;
            var type = this.gl.FLOAT;
            var normalize = false;
            var stride = 0;
            var offset = 0;
            this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_vert_buf);
            this.gl.vertexAttribPointer(this.gl_programinfo.attribLocations.vertexPosition, numComponents, type, normalize, stride, offset);
            this.gl.enableVertexAttribArray(this.gl_programinfo.attribLocations.vertexPosition);
        }
        {
            var numComponents = 3;
            var type = this.gl.FLOAT;
            var normalize = false;
            var stride = 0;
            var offset = 0;
            this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_normal_buf);
            this.gl.vertexAttribPointer(1, numComponents, type, normalize, stride, offset);
            this.gl.enableVertexAttribArray(1);
        }
        {
            var numComponents = 3;
            var type = this.gl.FLOAT;
            var normalize = false;
            var stride = 0;
            var offset = 0;
            this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_color_buf);
            this.gl.vertexAttribPointer(this.gl_programinfo.attribLocations.vertexColor, numComponents, type, normalize, stride, offset);
            this.gl.enableVertexAttribArray(this.gl_programinfo.attribLocations.vertexColor);
        }
        this.gl.useProgram(this.gl_programinfo.program);
        this.gl.uniformMatrix4fv(this.gl_programinfo.uniformLocations.projectionMatrix, false, projectionMatrix);
        this.gl.uniformMatrix4fv(this.gl_programinfo.uniformLocations.modelViewMatrix, false, modelViewMatrix);
        this.gl.uniformMatrix4fv(this.gl_programinfo.uniformLocations.normalMatrix, false, normalMatrix);
        this.gl.drawArrays(this.gl.TRIANGLES, 0, 36);
        this.squareRotation += deltaTime;
    };
    cube_c.prototype.initBuffers = function () {
        this.gl_vert_buf = this.gl.createBuffer();
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_vert_buf);
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(positions), this.gl.STATIC_DRAW);
        this.gl_normal_buf = this.gl.createBuffer();
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_normal_buf);
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(normals), this.gl.STATIC_DRAW);
        this.gl_color_buf = this.gl.createBuffer();
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.gl_color_buf);
        this.gl.bufferData(this.gl.ARRAY_BUFFER, new Float32Array(colors), this.gl.STATIC_DRAW);
    };
    cube_c.prototype.initShaderProgram = function (vsSource, fsSource) {
        var vertexShader = this.loadShader(this.gl.VERTEX_SHADER, vsSource);
        var fragmentShader = this.loadShader(this.gl.FRAGMENT_SHADER, fsSource);
        var shaderProgram = this.gl.createProgram();
        this.gl.attachShader(shaderProgram, vertexShader);
        this.gl.attachShader(shaderProgram, fragmentShader);
        this.gl.linkProgram(shaderProgram);
        if (!this.gl.getProgramParameter(shaderProgram, this.gl.LINK_STATUS)) {
            alert('Unable to initialize the shader program: ' + this.gl.getProgramInfoLog(shaderProgram));
            return null;
        }
        return shaderProgram;
    };
    cube_c.prototype.loadShader = function (type, source) {
        var shader = this.gl.createShader(type);
        this.gl.shaderSource(shader, source);
        this.gl.compileShader(shader);
        if (!this.gl.getShaderParameter(shader, this.gl.COMPILE_STATUS)) {
            alert('An error occurred compiling the shaders: ' + this.gl.getShaderInfoLog(shader));
            this.gl.deleteShader(shader);
            return null;
        }
        return shader;
    };
    return cube_c;
}());
function main() {
    var app = new cube_c();
    var then = 0.0;
    app.setup();
    function render(now) {
        now *= 0.001;
        var deltaTime = now - then;
        then = now;
        app.render(deltaTime);
        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}
main();
function mtrx4_set_idtt() {
    var rt = new Float32Array(16);
    var i, j;
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            if (i == j) {
                rt[id_rw(i, j, 4)] = 1.0;
            }
            else {
                rt[id_rw(i, j, 4)] = 0.0;
            }
        }
    }
    return rt;
}
function mtrx4_transpose(a) {
    var rt = new Float32Array(16);
    rt[0] = a[0];
    rt[1] = a[4];
    rt[2] = a[8];
    rt[3] = a[12];
    rt[4] = a[1];
    rt[5] = a[5];
    rt[6] = a[9];
    rt[7] = a[13];
    rt[8] = a[2];
    rt[9] = a[6];
    rt[10] = a[10];
    rt[11] = a[14];
    rt[12] = a[3];
    rt[13] = a[7];
    rt[14] = a[11];
    rt[15] = a[15];
    return rt;
}
function mtrx4_invert(m) {
    var inv = new Float32Array(16);
    var rt = new Float32Array(16);
    var i, det;
    inv[0] = m[5] * m[10] * m[15] -
        m[5] * m[11] * m[14] -
        m[9] * m[6] * m[15] +
        m[9] * m[7] * m[14] +
        m[13] * m[6] * m[11] -
        m[13] * m[7] * m[10];
    inv[4] = -m[4] * m[10] * m[15] +
        m[4] * m[11] * m[14] +
        m[8] * m[6] * m[15] -
        m[8] * m[7] * m[14] -
        m[12] * m[6] * m[11] +
        m[12] * m[7] * m[10];
    inv[8] = m[4] * m[9] * m[15] -
        m[4] * m[11] * m[13] -
        m[8] * m[5] * m[15] +
        m[8] * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9] * m[14] +
        m[4] * m[10] * m[13] +
        m[8] * m[5] * m[14] -
        m[8] * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];
    inv[1] = -m[1] * m[10] * m[15] +
        m[1] * m[11] * m[14] +
        m[9] * m[2] * m[15] -
        m[9] * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];
    inv[5] = m[0] * m[10] * m[15] -
        m[0] * m[11] * m[14] -
        m[8] * m[2] * m[15] +
        m[8] * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];
    inv[9] = -m[0] * m[9] * m[15] +
        m[0] * m[11] * m[13] +
        m[8] * m[1] * m[15] -
        m[8] * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];
    inv[13] = m[0] * m[9] * m[14] -
        m[0] * m[10] * m[13] -
        m[8] * m[1] * m[14] +
        m[8] * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];
    inv[2] = m[1] * m[6] * m[15] -
        m[1] * m[7] * m[14] -
        m[5] * m[2] * m[15] +
        m[5] * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];
    inv[6] = -m[0] * m[6] * m[15] +
        m[0] * m[7] * m[14] +
        m[4] * m[2] * m[15] -
        m[4] * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];
    inv[10] = m[0] * m[5] * m[15] -
        m[0] * m[7] * m[13] -
        m[4] * m[1] * m[15] +
        m[4] * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5] * m[14] +
        m[0] * m[6] * m[13] +
        m[4] * m[1] * m[14] -
        m[4] * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];
    inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];
    inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];
    inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];
    inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    if (det == 0)
        return mtrx4_set_idtt();
    det = 1.0 / det;
    for (i = 0; i < 16; i++)
        rt[i] = inv[i] * det;
    return rt;
}
function mtrx4_set_perspective(fovy, aspect, near, far) {
    var rt = new Float32Array(16);
    var f = 1.0 / Math.tan(fovy / 2);
    var nf;
    rt[0] = f / aspect;
    rt[1] = 0;
    rt[2] = 0;
    rt[3] = 0;
    rt[4] = 0;
    rt[5] = f;
    rt[6] = 0;
    rt[7] = 0;
    rt[8] = 0;
    rt[9] = 0;
    rt[11] = -1;
    rt[12] = 0;
    rt[13] = 0;
    rt[15] = 0;
    if (far != null && far !== Infinity) {
        nf = 1 / (near - far);
        rt[10] = (far + near) * nf;
        rt[14] = 2 * far * near * nf;
    }
    else {
        rt[10] = -1;
        rt[14] = -2 * near;
    }
    return rt;
}
function mtrx4_set_euler(yaw, pitch, roll) {
    var rt = new Float32Array(16);
    var cosy, siny, cosp, sinp, cosr, sinr;
    cosy = Math.cos(yaw);
    siny = Math.sin(yaw);
    cosp = Math.cos(pitch);
    sinp = Math.sin(pitch);
    cosr = Math.cos(roll);
    sinr = Math.sin(roll);
    rt[0] = cosy * cosr - siny * cosp * sinr;
    rt[1] = -cosy * sinr - siny * cosp * cosr;
    rt[2] = siny * sinp;
    rt[3] = 0.0;
    rt[4] = siny * cosr + cosy * cosp * sinr;
    rt[5] = -siny * sinr + cosy * cosp * cosr;
    rt[6] = -cosy * sinp;
    rt[7] = 0.0;
    rt[8] = sinp * sinr;
    rt[9] = sinp * cosr;
    rt[10] = cosp;
    rt[11] = 0.0;
    rt[12] = 0.0;
    rt[13] = 0.0;
    rt[14] = 0.0;
    rt[15] = 1.0;
    return rt;
}
function mtrx4_set_axisangl(axis, phi) {
    var rt = new Float32Array(16);
    var cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;
    var ax = vec3_normalize(axis);
    cosphi = Math.cos(phi);
    sinphi = Math.sin(phi);
    vxvy = ax[0] * ax[1];
    vxvz = ax[0] * ax[2];
    vyvz = ax[1] * ax[2];
    vx = ax[0];
    vy = ax[1];
    vz = ax[2];
    rt[0] = cosphi + (1.0 - cosphi) * vx * vx;
    rt[1] = (1.0 - cosphi) * vxvy - sinphi * vz;
    rt[2] = (1.0 - cosphi) * vxvz + sinphi * vy;
    rt[3] = 0.0;
    rt[4] = (1.0 - cosphi) * vxvy + sinphi * vz;
    rt[5] = cosphi + (1.0 - cosphi) * vy * vy;
    rt[6] = (1.0 - cosphi) * vyvz - sinphi * vx;
    rt[7] = 0.0;
    rt[8] = (1.0 - cosphi) * vxvz - sinphi * vy;
    rt[9] = (1.0 - cosphi) * vyvz + sinphi * vx;
    rt[10] = cosphi + (1.0 - cosphi) * vz * vz;
    rt[11] = 0.0;
    rt[12] = 0.0;
    rt[13] = 0.0;
    rt[14] = 0.0;
    rt[15] = 1.0;
    return rt;
}
function mtrx4_mult_axisangl(a, rad, axis) {
    var rt = new Float32Array(16);
    var x = axis[0], y = axis[1], z = axis[2];
    var len = Math.sqrt(x * x + y * y + z * z);
    var s, c, t;
    var a00, a01, a02, a03;
    var a10, a11, a12, a13;
    var a20, a21, a22, a23;
    var b00, b01, b02;
    var b10, b11, b12;
    var b20, b21, b22;
    len = 1 / len;
    x *= len;
    y *= len;
    z *= len;
    s = Math.sin(rad);
    c = Math.cos(rad);
    t = 1 - c;
    a00 = a[0];
    a01 = a[1];
    a02 = a[2];
    a03 = a[3];
    a10 = a[4];
    a11 = a[5];
    a12 = a[6];
    a13 = a[7];
    a20 = a[8];
    a21 = a[9];
    a22 = a[10];
    a23 = a[11];
    b00 = x * x * t + c;
    b01 = y * x * t + z * s;
    b02 = z * x * t - y * s;
    b10 = x * y * t - z * s;
    b11 = y * y * t + c;
    b12 = z * y * t + x * s;
    b20 = x * z * t + y * s;
    b21 = y * z * t - x * s;
    b22 = z * z * t + c;
    rt[0] = a00 * b00 + a10 * b01 + a20 * b02;
    rt[1] = a01 * b00 + a11 * b01 + a21 * b02;
    rt[2] = a02 * b00 + a12 * b01 + a22 * b02;
    rt[3] = a03 * b00 + a13 * b01 + a23 * b02;
    rt[4] = a00 * b10 + a10 * b11 + a20 * b12;
    rt[5] = a01 * b10 + a11 * b11 + a21 * b12;
    rt[6] = a02 * b10 + a12 * b11 + a22 * b12;
    rt[7] = a03 * b10 + a13 * b11 + a23 * b12;
    rt[8] = a00 * b20 + a10 * b21 + a20 * b22;
    rt[9] = a01 * b20 + a11 * b21 + a21 * b22;
    rt[10] = a02 * b20 + a12 * b21 + a22 * b22;
    rt[11] = a03 * b20 + a13 * b21 + a23 * b22;
    if (a !== rt) {
        rt[12] = a[12];
        rt[13] = a[13];
        rt[14] = a[14];
        rt[15] = a[15];
    }
    return rt;
}
function mtrx4_mult_translate(a, v) {
    var rt = new Float32Array(16);
    ;
    var x = v[0], y = v[1], z = v[2];
    var a00, a01, a02, a03;
    var a10, a11, a12, a13;
    var a20, a21, a22, a23;
    if (a === rt) {
        rt[12] = a[0] * x + a[4] * y + a[8] * z + a[12];
        rt[13] = a[1] * x + a[5] * y + a[9] * z + a[13];
        rt[14] = a[2] * x + a[6] * y + a[10] * z + a[14];
        rt[15] = a[3] * x + a[7] * y + a[11] * z + a[15];
    }
    else {
        a00 = a[0];
        a01 = a[1];
        a02 = a[2];
        a03 = a[3];
        a10 = a[4];
        a11 = a[5];
        a12 = a[6];
        a13 = a[7];
        a20 = a[8];
        a21 = a[9];
        a22 = a[10];
        a23 = a[11];
        rt[0] = a00;
        rt[1] = a01;
        rt[2] = a02;
        rt[3] = a03;
        rt[4] = a10;
        rt[5] = a11;
        rt[6] = a12;
        rt[7] = a13;
        rt[8] = a20;
        rt[9] = a21;
        rt[10] = a22;
        rt[11] = a23;
        rt[12] = a00 * x + a10 * y + a20 * z + a[12];
        rt[13] = a01 * x + a11 * y + a21 * z + a[13];
        rt[14] = a02 * x + a12 * y + a22 * z + a[14];
        rt[15] = a03 * x + a13 * y + a23 * z + a[15];
    }
    return rt;
}
function mtrx4_mult(a, b) {
    var mrange = 4;
    var i, j, k, tmp;
    var rt = new Float32Array(16);
    for (i = 0; i < mrange; i++) {
        for (j = 0; j < mrange; j++) {
            tmp = 0.0;
            for (k = 0; k < mrange; k++) {
                tmp = tmp + a[(id_rw(k, j, mrange))] * b[(id_rw(i, k, mrange))];
            }
            rt[(id_rw(i, j, mrange))] = tmp;
        }
    }
    return rt;
}
function vec3_set(x, y, z) {
    var rt = new Float32Array(3);
    rt[0] = x;
    rt[1] = y;
    rt[2] = z;
    return rt;
}
function vec3_lenght(v) {
    return Math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
function vec3_normalize(v) {
    var rt = new Float32Array(3);
    var len;
    len = vec3_lenght(v);
    if (len > 0.000001) {
        rt[0] = v[0] / len;
        rt[1] = v[1] / len;
        rt[2] = v[2] / len;
    }
    return rt;
}
function vec3_scale(v, scale) {
    var rt = new Float32Array(3);
    rt[0] = v[0] * scale;
    rt[1] = v[1] * scale;
    rt[2] = v[2] * scale;
    return rt;
}
function vec3_invert(v) {
    var rt = new Float32Array(3);
    rt[0] = -v[0];
    rt[1] = -v[1];
    rt[2] = -v[2];
    return rt;
}
function vec3_dot(a, b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
function vec3_sum(a, b) {
    var rt = new Float32Array(3);
    rt[0] = a[0] + b[0];
    rt[1] = a[1] + b[1];
    rt[2] = a[2] + b[2];
    return rt;
}
function vec3_sub(a, b) {
    var rt = new Float32Array(3);
    rt[0] = a[0] - b[0];
    rt[1] = a[1] - b[1];
    rt[2] = a[2] - b[2];
    return rt;
}
function vec3_cross(a, b) {
    var rt = new Float32Array(3);
    rt[0] = a[1] * b[2] - a[2] * b[1];
    rt[1] = a[2] * b[0] - a[0] * b[2];
    rt[2] = a[0] * b[1] - a[1] * b[0];
    return rt;
}
function deg_to_rad(deg) {
    return deg * Math.PI / 180.0;
}
function id_rw(i, j, n) {
    return (i * n + j);
}
;
function id_cw(i, j, n) {
    return (j * n + i);
}
;
