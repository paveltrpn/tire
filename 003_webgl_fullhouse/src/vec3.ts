
class vec3 {
	data: Float32Array;

	constructor () {
		this.data = new Float32Array(3);

		this.data[0] = 0.0;
		this.data[1] = 0.0;
		this.data[2] = 0.0;
	}

	at(i: number): number {
        return this.data[i];
	}
	
	set_at(i: number, n: number) {
        this.data[i] = n;
    }
}

type vec3_t = Array<Float32Array>[3];

function vec3_set(x: number, y: number, z: number): vec3_t {
    let rt: vec3_t = new Float32Array(3);

    rt[0] = x;
    rt[1] = y;
    rt[2] = z;

    return rt;
}

function vec3_lenght(v: vec3_t ): number {
	return Math.sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

function vec3_normalize(v: vec3_t): vec3_t {
	let rt: vec3_t = new Float32Array(3);
	let len: number;

	len = vec3_lenght(v);

	if (len > 0.000001) {
		rt[0] = v[0] / len;
		rt[1] = v[1] / len;
		rt[2] = v[2] / len;
	}

	return rt;
}

function vec3_scale(v: vec3_t, scale: number): vec3_t {
    let rt: vec3_t = new Float32Array(3);

	rt[0] = v[0] * scale;
	rt[1] = v[1] * scale;
	rt[2] = v[2] * scale;

	return rt;
}

function vec3_invert(v: vec3_t): vec3_t {
    let rt: vec3_t = new Float32Array(3);

	rt[0] = -v[0];
	rt[1] = -v[1];
	rt[2] = -v[2];

	return rt;
}

function vec3_dot(a : vec3_t, b: vec3_t): number {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

function vec3_sum(a: vec3_t, b: vec3_t): vec3_t {
    let rt: vec3_t = new Float32Array(3);

	rt[0] = a[0] + b[0];
	rt[1] = a[1] + b[1];
	rt[2] = a[2] + b[2];

	return rt;
}

function vec3_sub(a: vec3_t, b: vec3_t): vec3_t {
    let rt: vec3_t = new Float32Array(3);

	rt[0] = a[0] - b[0];
	rt[1] = a[1] - b[1];
	rt[2] = a[2] - b[2];

	return rt;
}

function vec3_cross(a: vec3_t, b: vec3_t): vec3_t {
    let rt: vec3_t = new Float32Array(3);

	rt[0] = a[1]*b[2] - a[2]*b[1];
	rt[1] = a[2]*b[0] - a[0]*b[2];
	rt[2] = a[0]*b[1] - a[1]*b[0];

	return rt;
}