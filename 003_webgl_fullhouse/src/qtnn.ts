
class qtnn {
    data: Float32Array;
    private readonly order: number = 4;

    constructor();
    constructor(x: qtnn);
    constructor(x: number, y: number, z: number, w: number);
    constructor(x?: number | qtnn, y?: number, z?: number, w?: number) {
		if (x instanceof qtnn) {
			this.data = new Float32Array(this.order);
			this.fromQtnn(x);
		} else {
			this.data = new Float32Array(this.order);

			this.data[0] = x || 0.0;
			this.data[1] = y || 0.0;
            this.data[2] = z || 0.0;
            this.data[3] = w || 1.0;
		}
    }
    
    fromQtnn(src: qtnn) {
        this.data[0] = src.data[0];
		this.data[1] = src.data[1];
        this.data[2] = src.data[2];
        this.data[3] = src.data[3];
    }

    lenght(): number {
		return Math.sqrt(this.data[0]*this.data[0] + 
						 this.data[1]*this.data[1] + 
                         this.data[2]*this.data[2] +
                         this.data[3]*this.data[3]);
    }
    
    normalize() {
		let len: number;
	
		len = this.lenght();

		if (len > fEPS) {
			this.data[0] = this.data[0] / len;
			this.data[1] = this.data[1] / len;
            this.data[2] = this.data[2] / len;
            this.data[3] = this.data[3] / len;
		}
	}

	scale(scale: number) {
		this.data[0] = this.data[0] * scale;
		this.data[1] = this.data[1] * scale;
        this.data[2] = this.data[2] * scale;
        this.data[3] = this.data[3] * scale;
	}

	invert() {
		this.data[0] = -this.data[0];
		this.data[1] = -this.data[1];
        this.data[2] = -this.data[2];
        this.data[3] =  this.data[3];
	}

	setRotX(phi: number) {
		let halfPhi: number = phi/2.0;

		this.data[3] = Math.cos(halfPhi);
		this.data[0] = Math.sin(halfPhi);
		this.data[1] = 0.0;
		this.data[2] = 0.0;
	}

	setRotY(phi: number) {
		let halfPhi: number = phi/2.0;

		this.data[3] = Math.cos(halfPhi);
		this.data[0] = 0.0;
		this.data[1] = Math.sin(halfPhi);
		this.data[2] = 0.0;
	}

	setRotZ(phi: number) {
		let halfPhi: number = phi/2.0;

		this.data[3] = Math.cos(halfPhi);
		this.data[0] = 0.0;
		this.data[1] = 0.0;
		this.data[2] = Math.sin(halfPhi);
	}

	setAxisAngl(axis: vec3, phi: number) {
		let halfPhiSin: number = Math.sin(phi/2.0);
		let ax: vec3 = new vec3();

		ax = vec3Normalize(axis);
		
		this.data[3] = Math.cos(phi/2.0);
		this.data[0] = ax.data[0] * halfPhiSin;
		this.data[1] = ax.data[1] * halfPhiSin;
		this.data[2] = ax.data[2] * halfPhiSin;
	}
}

function qtnnDot(a: qtnn, b: qtnn): number{
	return a.data[0]*b.data[0] + 
		   a.data[1]*b.data[1] + 
		   a.data[2]*b.data[2] + 
		   a.data[3]*b.data[3];
}

function qtnnMult(a: qtnn, b: qtnn): qtnn {
	let rt = new qtnn;

	rt.data[3] = a.data[3]*b.data[3] - a.data[0]*b.data[0] - a.data[1]*b.data[1] - a.data[2]*b.data[2];
	rt.data[0] = a.data[3]*b.data[0] + a.data[0]*b.data[3] + a.data[1]*b.data[2] - a.data[2]*b.data[1];
	rt.data[1] = a.data[3]*b.data[1] - a.data[0]*b.data[2] + a.data[1]*b.data[3] + a.data[2]*b.data[0];
	rt.data[2] = a.data[3]*b.data[2] + a.data[0]*b.data[1] - a.data[1]*b.data[0] + a.data[2]*b.data[3];

	return rt;
}

function qtnnSlerp(from: qtnn, to: qtnn, t:number): qtnn {
	let rt: qtnn;
	let p1: number[];
	let omega, cosom, sinom, scale0, scale1: number;

	// косинус угла
	cosom = qtnnDot(from, to);	

	if ( cosom <0.0 ) { 
	  cosom = -cosom;
	  p1[0] = -to.data[0];  p1[1] = -to.data[1];
	  p1[2] = -to.data[2];  p1[3] = -to.data[3];
	} else {
	  p1[0] = to.data[0];    p1[1] = to.data[1];
	  p1[2] = to.data[2];    p1[3] = to.data[3];
	}

	if ( (1.0 - cosom) > fEPS )	{
	  // стандартный случай (slerp)
	  omega  = Math.acos(cosom);
	  sinom  = Math.sin(omega);
	  scale0 = Math.sin((1.0 - t) * omega) / sinom;
	  scale1 = Math.sin(t * omega) / sinom;
	} else {        
	  // если маленький угол - линейная интерполяция
	  scale0 = 1.0 - t;
	  scale1 = t;
	}

	rt.data[0] = scale0 * from.data[0] + scale1 * p1[0];
	rt.data[1] = scale0 * from.data[1] + scale1 * p1[1];
	rt.data[2] = scale0 * from.data[2] + scale1 * p1[2];
	rt.data[3] = scale0 * from.data[3] + scale1 * p1[3];
	  
	return rt;
}
