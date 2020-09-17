
type mtrx4_t = Array<Float32Array>[16];


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

//////////////////////////////////////////////////

//////////////////////////////////////////////////

class mtrx4 {
    data: Float32Array;
    private readonly order: number = 4;

    constructor() {
        let i, j: number;

        this.data = new Float32Array(16);

	    for (i = 0; i < this.order; i++) {
	    	for (j = 0; j < this.order; j++) {
	    		if (i == j) {
	    			this.data[id_rw(i, j, this.order)] = 1.0;
	    		} else {
	    			this.data[id_rw(i, j, this.order)] = 0.0;
	    		}
	    	}
	    }
    }

    set_idtt() {
        let i, j: number;

        for (i = 0; i < this.order; i++) {
	    	for (j = 0; j < this.order; j++) {
	    		if (i == j) {
	    			this.data[id_rw(i, j, this.order)] = 1.0;
	    		} else {
	    			this.data[id_rw(i, j, this.order)] = 0.0;
	    		}
	    	}
	    }
    }

    from(src: mtrx4) {
        for (let i = 0; i < 16; i++) {
            this.data[i] = src.data[i];
        }
    }

    mult(a: mtrx4) {
        let i, j, k, tmp: number;
        let rt: mtrx4 = new mtrx4();
        
        for (i = 0; i < this.order; i++) {
            for (j = 0; j < this.order; j++) {
                tmp = 0.0;
                
                for (k = 0; k < this.order; k++) {
                    tmp = tmp + this.data[(id_rw(k, j, this.order))] * a.data[(id_rw(i, k, this.order))];
                }
                
                rt.data[(id_rw(i, j, this.order))] = tmp;
            }
        }
    
        this.from(rt);
    }

    set_perspective(fovy: number, aspect: number, near: number, far: number) {
        let f = 1.0 / Math.tan(fovy / 2)
        let nf: number;
    
        this.data[0] = f / aspect;
        this.data[1] = 0;
        this.data[2] = 0;
        this.data[3] = 0;
        this.data[4] = 0;
        this.data[5] = f;
        this.data[6] = 0;
        this.data[7] = 0;
        this.data[8] = 0;
        this.data[9] = 0;
        this.data[11] = -1;
        this.data[12] = 0;
        this.data[13] = 0;
        this.data[15] = 0;
    
        if (far != null && far !== Infinity) {
            nf = 1 / (near - far);
            this.data[10] = (far + near) * nf;
            this.data[14] = 2 * far * near * nf;
        } else {
            this.data[10] = -1;
            this.data[14] = -2 * near;
        }
    }

    invert() {
        let inv: mtrx4 = new mtrx4();
        let i, det: number;
    
        inv.data[0] = this.data[5]  * this.data[10] * this.data[15] - 
                 this.data[5]  * this.data[11] * this.data[14] - 
                 this.data[9]  * this.data[6]  * this.data[15] + 
                 this.data[9]  * this.data[7]  * this.data[14] +
                 this.data[13] * this.data[6]  * this.data[11] - 
                 this.data[13] * this.data[7]  * this.data[10];
    
        inv.data[4] = -this.data[4]  * this.data[10] * this.data[15] + 
                  this.data[4]  * this.data[11] * this.data[14] + 
                  this.data[8]  * this.data[6]  * this.data[15] - 
                  this.data[8]  * this.data[7]  * this.data[14] - 
                  this.data[12] * this.data[6]  * this.data[11] + 
                  this.data[12] * this.data[7]  * this.data[10];
    
        inv.data[8] = this.data[4]  * this.data[9] * this.data[15] - 
                 this.data[4]  * this.data[11] * this.data[13] - 
                 this.data[8]  * this.data[5] * this.data[15] + 
                 this.data[8]  * this.data[7] * this.data[13] + 
                 this.data[12] * this.data[5] * this.data[11] - 
                 this.data[12] * this.data[7] * this.data[9];
    
        inv.data[12] = -this.data[4]  * this.data[9] * this.data[14] + 
                   this.data[4]  * this.data[10] * this.data[13] +
                   this.data[8]  * this.data[5] * this.data[14] - 
                   this.data[8]  * this.data[6] * this.data[13] - 
                   this.data[12] * this.data[5] * this.data[10] + 
                   this.data[12] * this.data[6] * this.data[9];
    
        inv.data[1] = -this.data[1]  * this.data[10] * this.data[15] + 
                  this.data[1]  * this.data[11] * this.data[14] + 
                  this.data[9]  * this.data[2] * this.data[15] - 
                  this.data[9]  * this.data[3] * this.data[14] - 
                  this.data[13] * this.data[2] * this.data[11] + 
                  this.data[13] * this.data[3] * this.data[10];
    
        inv.data[5] = this.data[0]  * this.data[10] * this.data[15] - 
                 this.data[0]  * this.data[11] * this.data[14] - 
                 this.data[8]  * this.data[2] * this.data[15] + 
                 this.data[8]  * this.data[3] * this.data[14] + 
                 this.data[12] * this.data[2] * this.data[11] - 
                 this.data[12] * this.data[3] * this.data[10];
    
        inv.data[9] = -this.data[0]  * this.data[9] * this.data[15] + 
                  this.data[0]  * this.data[11] * this.data[13] + 
                  this.data[8]  * this.data[1] * this.data[15] - 
                  this.data[8]  * this.data[3] * this.data[13] - 
                  this.data[12] * this.data[1] * this.data[11] + 
                  this.data[12] * this.data[3] * this.data[9];
    
        inv.data[13] = this.data[0]  * this.data[9] * this.data[14] - 
                  this.data[0]  * this.data[10] * this.data[13] - 
                  this.data[8]  * this.data[1] * this.data[14] + 
                  this.data[8]  * this.data[2] * this.data[13] + 
                  this.data[12] * this.data[1] * this.data[10] - 
                  this.data[12] * this.data[2] * this.data[9];
    
        inv.data[2] = this.data[1]  * this.data[6] * this.data[15] - 
                 this.data[1]  * this.data[7] * this.data[14] - 
                 this.data[5]  * this.data[2] * this.data[15] + 
                 this.data[5]  * this.data[3] * this.data[14] + 
                 this.data[13] * this.data[2] * this.data[7] - 
                 this.data[13] * this.data[3] * this.data[6];
    
        inv.data[6] = -this.data[0]  * this.data[6] * this.data[15] + 
                  this.data[0]  * this.data[7] * this.data[14] + 
                  this.data[4]  * this.data[2] * this.data[15] - 
                  this.data[4]  * this.data[3] * this.data[14] - 
                  this.data[12] * this.data[2] * this.data[7] + 
                  this.data[12] * this.data[3] * this.data[6];
    
        inv.data[10] = this.data[0]  * this.data[5] * this.data[15] - 
                  this.data[0]  * this.data[7] * this.data[13] - 
                  this.data[4]  * this.data[1] * this.data[15] + 
                  this.data[4]  * this.data[3] * this.data[13] + 
                  this.data[12] * this.data[1] * this.data[7] - 
                  this.data[12] * this.data[3] * this.data[5];
    
        inv.data[14] = -this.data[0]  * this.data[5] * this.data[14] + 
                   this.data[0]  * this.data[6] * this.data[13] + 
                   this.data[4]  * this.data[1] * this.data[14] - 
                   this.data[4]  * this.data[2] * this.data[13] - 
                   this.data[12] * this.data[1] * this.data[6] + 
                   this.data[12] * this.data[2] * this.data[5];
    
        inv.data[3] = -this.data[1] * this.data[6] * this.data[11] + 
                  this.data[1] * this.data[7] * this.data[10] + 
                  this.data[5] * this.data[2] * this.data[11] - 
                  this.data[5] * this.data[3] * this.data[10] - 
                  this.data[9] * this.data[2] * this.data[7] + 
                  this.data[9] * this.data[3] * this.data[6];
    
        inv.data[7] = this.data[0] * this.data[6] * this.data[11] - 
                 this.data[0] * this.data[7] * this.data[10] - 
                 this.data[4] * this.data[2] * this.data[11] + 
                 this.data[4] * this.data[3] * this.data[10] + 
                 this.data[8] * this.data[2] * this.data[7] - 
                 this.data[8] * this.data[3] * this.data[6];
    
        inv.data[11] = -this.data[0] * this.data[5] * this.data[11] + 
                   this.data[0] * this.data[7] * this.data[9] + 
                   this.data[4] * this.data[1] * this.data[11] - 
                   this.data[4] * this.data[3] * this.data[9] - 
                   this.data[8] * this.data[1] * this.data[7] + 
                   this.data[8] * this.data[3] * this.data[5];
    
        inv.data[15] = this.data[0] * this.data[5] * this.data[10] - 
                  this.data[0] * this.data[6] * this.data[9] - 
                  this.data[4] * this.data[1] * this.data[10] + 
                  this.data[4] * this.data[2] * this.data[9] + 
                  this.data[8] * this.data[1] * this.data[6] - 
                  this.data[8] * this.data[2] * this.data[5];
    
        det = this.data[0] * inv.data[0] + this.data[1] * inv.data[4] + this.data[2] * inv.data[8] + this.data[3] * inv.data[12];
    
        if (det == 0) {
            this.set_idtt();
            return;
        }
    
        det = 1.0 / det;
    
        for (i = 0; i < 16; i++)
            this.data[i] = inv.data[i] * det;
    }

    transpose() {
        let tmp = new mtrx4;

        tmp.data[0] = this.data[0];
        tmp.data[1] = this.data[4];
        tmp.data[2] = this.data[8];
        tmp.data[3] = this.data[12];
        tmp.data[4] = this.data[1];
        tmp.data[5] = this.data[5];
        tmp.data[6] = this.data[9];
        tmp.data[7] = this.data[13];
        tmp.data[8] = this.data[2];
        tmp.data[9] = this.data[6];
        tmp.data[10] = this.data[10];
        tmp.data[11] = this.data[14];
        tmp.data[12] = this.data[3];
        tmp.data[13] = this.data[7];
        tmp.data[14] = this.data[11];
        tmp.data[15] = this.data[15];

        for (let i = 0; i < 16; i++) {
            this.data[i] = tmp.data[i];
        }
    }

    set_axisangl(axis: vec3_t, phi: number) {
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
    
        this.data[0]  = cosphi + (1.0-cosphi)*vx*vx;
        this.data[1]  = (1.0-cosphi)*vxvy - sinphi*vz;
        this.data[2]  = (1.0-cosphi)*vxvz + sinphi*vy;
        this.data[3]  = 0.0;
    
        this.data[4]  = (1.0-cosphi)*vxvy + sinphi*vz;
        this.data[5]  = cosphi + (1.0-cosphi)*vy*vy;
        this.data[6]  = (1.0-cosphi)*vyvz - sinphi*vx;
        this.data[7]  = 0.0;
    
        this.data[8]  = (1.0-cosphi)*vxvz - sinphi*vy;
        this.data[9]  = (1.0-cosphi)*vyvz + sinphi*vx;
        this.data[10] = cosphi + (1.0-cosphi)*vz*vz;
        this.data[11] = 0.0;
    
        this.data[12] = 0.0;
        this.data[13] = 0.0;
        this.data[14] = 0.0;
        this.data[15] = 1.0;
    }

    mult_translate(a: mtrx4, v: any) {
        let x = v[0], y = v[1], z = v[2];
        let a00, a01, a02, a03;
        let a10, a11, a12, a13;
        let a20, a21, a22, a23;
    
        if (a.data === this.data) {
            this.data[12] = a.data[0] * x + a.data[4] * y + a.data[8] * z + a.data[12];
            this.data[13] = a.data[1] * x + a.data[5] * y + a.data[9] * z + a.data[13];
            this.data[14] = a.data[2] * x + a.data[6] * y + a.data[10] * z + a.data[14];
            this.data[15] = a.data[3] * x + a.data[7] * y + a.data[11] * z + a.data[15];
        } else {
            a00 = a.data[0];
            a01 = a.data[1];
            a02 = a.data[2];
            a03 = a.data[3];
            a10 = a.data[4];
            a11 = a.data[5];
            a12 = a.data[6];
            a13 = a.data[7];
            a20 = a.data[8];
            a21 = a.data[9];
            a22 = a.data[10];
            a23 = a.data[11];
    
            this.data[0] = a00;
            this.data[1] = a01;
            this.data[2] = a02;
            this.data[3] = a03;
            this.data[4] = a10;
            this.data[5] = a11;
            this.data[6] = a12;
            this.data[7] = a13;
            this.data[8] = a20;
            this.data[9] = a21;
            this.data[10] = a22;
            this.data[11] = a23;
    
            this.data[12] = a00 * x + a10 * y + a20 * z + a.data[12];
            this.data[13] = a01 * x + a11 * y + a21 * z + a.data[13];
            this.data[14] = a02 * x + a12 * y + a22 * z + a.data[14];
            this.data[15] = a03 * x + a13 * y + a23 * z + a.data[15];
        }
    }
}
