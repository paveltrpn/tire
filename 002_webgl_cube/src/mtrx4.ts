
type mtrx4_t = Array<Float32Array>[16];

function mtrx4_set_idtt(): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);
    let i, j: number;

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			if (i == j) {
				rt[id_rw(i, j, 4)] = 1.0;
			} else {
				rt[id_rw(i, j, 4)] = 0.0;
			}
		}
    }
        
    return rt;
}

/*
function mtrx4_transpose(m: mtrx4_t): mtrx4_t {
	const mrange: number = 4;
	let i, j, tmp: number;
	let rt: mtrx4_t = new Float32Array(16);

	rt = m;

	for (i = 0; i < mrange; i++) {
		for (j = 0; j < i; j++) {
			tmp = rt[id_rw(i, i, mrange)];
			rt[id_rw(i, j, mrange)] = rt[id_rw(j, i, mrange)];
			rt[id_rw(j, i, mrange)] = tmp;
		}
	}

	return rt;
}
*/

function mtrx4_transpose(a: mtrx4_t): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);

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

function mtrx4_invert(m: mtrx4_t): mtrx4_t {
    let inv: mtrx4_t = new Float32Array(16);
    let rt: mtrx4_t = new Float32Array(16);
    let i, det: number;

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
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

function mtrx4_set_perspective(fovy: number, aspect: number, near: number, far: number): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);
    let f = 1.0 / Math.tan(fovy / 2)
    let nf: number;

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
    } else {
        rt[10] = -1;
        rt[14] = -2 * near;
    }

    return rt;
}

function mtrx4_set_ortho(left: number, right: number, bottom: number, top: number, near: number, far: number) {
    let out = new Float32Array(16);
    let lr = 1 / (left - right);
    let bt = 1 / (bottom - top);
    let nf = 1 / (near - far);
    out[0] = -2 * lr;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = -2 * bt;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = 2 * nf;
    out[11] = 0;
    out[12] = (left + right) * lr;
    out[13] = (top + bottom) * bt;
    out[14] = (far + near) * nf;
    out[15] = 1;
    return out;
}

function mtrx4_set_euler(yaw: number, pitch: number, roll: number): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);
	let cosy, siny, cosp, sinp, cosr, sinr: number;
	
	cosy = Math.cos(yaw);
	siny = Math.sin(yaw);
	cosp = Math.cos(pitch);
	sinp = Math.sin(pitch);
	cosr = Math.cos(roll);
	sinr = Math.sin(roll);

	rt[0]  = cosy*cosr - siny*cosp*sinr;
	rt[1]  = -cosy*sinr - siny*cosp*cosr;
	rt[2]  = siny * sinp;
	rt[3]  = 0.0;
	
	rt[4]  = siny*cosr + cosy*cosp*sinr;
	rt[5]  = -siny*sinr + cosy*cosp*cosr;
	rt[6]  = -cosy * sinp;
	rt[7]  = 0.0;
	
	rt[8]  = sinp * sinr;
	rt[9]  = sinp * cosr;
	rt[10] = cosp;
	rt[11] = 0.0;

	rt[12] = 0.0;
	rt[13] = 0.0;
	rt[14] = 0.0;
    rt[15] = 1.0;
    
    return rt;
}

function mtrx4_set_axisangl(axis: vec3_t, phi: number): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);
    let cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz: number;
    let ax: vec3_t = vec3_normalize(axis);

    cosphi = Math.cos(phi);
	sinphi = Math.sin(phi);
	vxvy = ax[0] * ax[1];
	vxvz = ax[0] * ax[2];
	vyvz = ax[1] * ax[2];
	vx = ax[0];
	vy = ax[1];
	vz = ax[2];

	rt[0]  = cosphi + (1.0-cosphi)*vx*vx;
	rt[1]  = (1.0-cosphi)*vxvy - sinphi*vz;
	rt[2]  = (1.0-cosphi)*vxvz + sinphi*vy;
	rt[3]  = 0.0;

	rt[4]  = (1.0-cosphi)*vxvy + sinphi*vz;
	rt[5]  = cosphi + (1.0-cosphi)*vy*vy;
	rt[6]  = (1.0-cosphi)*vyvz - sinphi*vx;
	rt[7]  = 0.0;

	rt[8]  = (1.0-cosphi)*vxvz - sinphi*vy;
	rt[9]  = (1.0-cosphi)*vyvz + sinphi*vx;
	rt[10] = cosphi + (1.0-cosphi)*vz*vz;
	rt[11] = 0.0;

	rt[12] = 0.0;
	rt[13] = 0.0;
	rt[14] = 0.0;
    rt[15] = 1.0;
    
    return rt;
}

function mtrx4_mult_axisangl(a: any, rad: any, axis: any): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);
    let x = axis[0], y = axis[1], z = axis[2];
    let len = Math.sqrt(x*x + y*y + z*z);
    let s, c, t;
    let a00, a01, a02, a03;
    let a10, a11, a12, a13;
    let a20, a21, a22, a23;
    let b00, b01, b02;
    let b10, b11, b12;
    let b20, b21, b22;
  
    // if (len < glMatrix.EPSILON) {
      // return null;
    // }
  
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
  
    // Construct the elements of the rotation matrix
    b00 = x * x * t + c;
    b01 = y * x * t + z * s;
    b02 = z * x * t - y * s;
    b10 = x * y * t - z * s;
    b11 = y * y * t + c;
    b12 = z * y * t + x * s;
    b20 = x * z * t + y * s;
    b21 = y * z * t - x * s;
    b22 = z * z * t + c;
  
    // Perform rotation-specific matrix multiplication
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
        // If the source and destination differ, copy the unchanged last row
        rt[12] = a[12];
        rt[13] = a[13];
        rt[14] = a[14];
        rt[15] = a[15];
    }

    return rt;
}

function mtrx4_mult_translate(a: any, v: any): mtrx4_t {
    let rt: mtrx4_t = new Float32Array(16);;
    let x = v[0], y = v[1], z = v[2];
    let a00, a01, a02, a03;
    let a10, a11, a12, a13;
    let a20, a21, a22, a23;

    if (a === rt) {
        rt[12] = a[0] * x + a[4] * y + a[8] * z + a[12];
        rt[13] = a[1] * x + a[5] * y + a[9] * z + a[13];
        rt[14] = a[2] * x + a[6] * y + a[10] * z + a[14];
        rt[15] = a[3] * x + a[7] * y + a[11] * z + a[15];
    } else {
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

function mtrx4_mult(a: mtrx4_t, b: mtrx4_t): mtrx4_t {
    const mrange = 4;
	let i, j, k, tmp: number;
    let rt: mtrx4_t = new Float32Array(16);

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