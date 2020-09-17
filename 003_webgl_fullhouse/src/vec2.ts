
type vec2_t = Array<Float32Array>[3];

class vec2 {

	data: Float32Array;
	
	constructor() {
		this.data = new Float32Array(2);

		this.data[0] = 0.0;
		this.data[1] = 0.0;
	}

	at(i: number): number {
        return this.data[i];
	}
	
	set_at(i: number, n: number) {
        this.data[i] = n;
    }
}


function vec2_lenght(v: vec2): number {
	return Math.sqrt(v.data[0]*v.data[0] + v.data[1]*v.data[1]);
}    

function vec2_normalize(v: vec2) {
	let v2: vec2;
	let v3: vec3;

	v2 = v3;

	let len: number;
	
	len = vec2_lenght(v);

	if (len != 0.0) {
		v.data[0] = v.data[0] / len;
		v.data[1] = v.data[1] / len;
	}
}
