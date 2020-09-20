
class vec2 {

	data: Float32Array;
	private readonly order: number = 2;

	constructor();
	constructor(x: vec2);
	constructor(x: number, y: number);
	constructor(x?: number | vec2, y?: number) {
		if (x instanceof vec2) {
			this.data = new Float32Array(2);
			this.fromVec2(x);
		} else {
			this.data = new Float32Array(2);

			this.data[0] = x || 0.0;
			this.data[1] = y || 0.0;
		}
	}

	fromVec2(src: vec2) {
		this.data[0] = src.data[0];
		this.data[1] = src.data[1];
	}

	lenght(): number {
		return Math.sqrt(this.data[0]*this.data[0] + 
						 this.data[1]*this.data[1]);
	}

	normalize() {
		let len: number;
	
		len = this.lenght();

		if (len != 0.0) {
			this.data[0] = this.data[0] / len;
			this.data[1] = this.data[1] / len;
		}
	}

	scale(scale: number) {
		this.data[0] = this.data[0] * scale;
		this.data[1] = this.data[1] * scale;
	}

	invert() {
		this.data[0] = -this.data[0];
		this.data[1] = -this.data[1];
	}
};

function vec2Normalize(v: vec2): vec2 {
	let rt = new vec2();
	let len: number;
	
	len = v.lenght();

	if (len > fEPS) {
		rt.data[0] = v.data[0] / len;
		rt.data[1] = v.data[1] / len;
	}

	return rt;
}

function vec2Scale(v: vec2, scale: number): vec2 {
    let rt: vec2 = new vec2();

	rt.data[0] = v.data[0] * scale;
	rt.data[1] = v.data[1] * scale;

	return rt;
}

function vec2Invert(v: vec2): vec2 {
    let rt: vec2 = new vec2;

	rt.data[0] = -v.data[0];
	rt.data[1] = -v.data[1];

	return rt;
}

function vec2Dot(a : vec2, b: vec2): number {
	return a.data[0]*b.data[0] + a.data[1]*b.data[1] + a.data[2]*b.data[2];
}

function vec2Sum(a: vec2, b: vec2): vec2 {
    let rt: vec2 = new vec2;

	rt.data[0] = a.data[0] + b.data[0];
	rt.data[1] = a.data[1] + b.data[1];

	return rt;
}

function vec2Sub(a: vec2, b: vec2): vec2 {
    let rt: vec2 = new vec2;

	rt.data[0] = a.data[0] - b.data[0];
	rt.data[1] = a.data[1] - b.data[1];

	return rt;
}
