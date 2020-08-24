
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