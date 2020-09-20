
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
        this.data[3] = -this.data[3];
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

// Не работает пока что
function qtnnMultVec3(a: qtnn, b: vec3): qtnn {
	let rt = new qtnn;

	rt.data[3] = -a.data[3]*b.data[0] - a.data[1]*b.data[1] - a.data[2]*b.data[2];
	rt.data[0] =  a.data[3]*b.data[0] + a.data[1]*b.data[2] - a.data[2]*b.data[1];
	rt.data[1] =  a.data[3]*b.data[1] - a.data[0]*b.data[2] + a.data[2]*b.data[0];
	rt.data[2] =  a.data[3]*b.data[2] + a.data[0]*b.data[1] - a.data[1]*b.data[0];

	return rt;
}