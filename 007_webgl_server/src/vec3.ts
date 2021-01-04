
export class vec3 {
	data: Float32Array;
	private readonly order: number = 3;

	constructor();
	constructor(x: vec3);
	constructor(x: number, y: number, z: number)
	constructor(x?: number | vec3, y?: number, z?: number) {
		if (x instanceof vec3) {
			this.data = new Float32Array(3);
			this.fromVec3(x);
		} else {
			this.data = new Float32Array(3);

			this.data[0] = x || 0.0;
			this.data[1] = y || 0.0;
			this.data[2] = z || 0.0;
		}
	}

	fromVec3(src: vec3) {
		this.data[0] = src.data[0];
		this.data[1] = src.data[1];
		this.data[2] = src.data[2];
	}

	lenght(): number {
		return Math.sqrt(this.data[0]*this.data[0] + 
						 this.data[1]*this.data[1] + 
						 this.data[2]*this.data[2]);
	}

	normalize() {
		let len: number;
	
		len = this.lenght();

		if (len != 0.0) {
			this.data[0] = this.data[0] / len;
			this.data[1] = this.data[1] / len;
			this.data[2] = this.data[2] / len;
		}
	}

	scale(scale: number) {
		this.data[0] = this.data[0] * scale;
		this.data[1] = this.data[1] * scale;
		this.data[2] = this.data[2] * scale;
	}

	invert() {
		this.data[0] = -this.data[0];
		this.data[1] = -this.data[1];
		this.data[2] = -this.data[2];
	}
}

export function vec3Set(x: number, y: number, z:number): vec3 {
	let rt = new vec3;

	rt.data[0] = x;
	rt.data[1] = y;
	rt.data[2] = z;

	return rt;
}

export function vec3Cross(a: vec3, b: vec3): vec3 {
    let rt: vec3 = new vec3();

	rt.data[0] = a.data[1]*b.data[2] - a.data[2]*b.data[1];
	rt.data[1] = a.data[2]*b.data[0] - a.data[0]*b.data[2];
	rt.data[2] = a.data[0]*b.data[1] - a.data[1]*b.data[0];

	return rt;
}

export function vec3Normalize(v: vec3): vec3 {
	let rt = new vec3();
	let len: number;
	
	len = v.lenght();

	if (len > Number.EPSILON) {
		rt.data[0] = v.data[0] / len;
		rt.data[1] = v.data[1] / len;
		rt.data[2] = v.data[2] / len;
	}

	return rt;
}

export function vec3Scale(v: vec3, scale: number): vec3 {
    let rt: vec3 = new vec3();

	rt.data[0] = v.data[0] * scale;
	rt.data[1] = v.data[1] * scale;
	rt.data[2] = v.data[2] * scale;

	return rt;
}

export function vec3Invert(v: vec3): vec3 {
    let rt: vec3 = new vec3;

	rt.data[0] = -v.data[0];
	rt.data[1] = -v.data[1];
	rt.data[2] = -v.data[2];

	return rt;
}

export function vec3Dot(a : vec3, b: vec3): number {
	return a.data[0]*b.data[0] + a.data[1]*b.data[1] + a.data[2]*b.data[2];
}

export function vec3Sum(a: vec3, b: vec3): vec3 {
    let rt: vec3 = new vec3;

	rt.data[0] = a.data[0] + b.data[0];
	rt.data[1] = a.data[1] + b.data[1];
	rt.data[2] = a.data[2] + b.data[2];

	return rt;
}

export function vec3Sub(a: vec3, b: vec3): vec3 {
    let rt: vec3 = new vec3;

	rt.data[0] = a.data[0] - b.data[0];
	rt.data[1] = a.data[1] - b.data[1];
	rt.data[2] = a.data[2] - b.data[2];

	return rt;
}
