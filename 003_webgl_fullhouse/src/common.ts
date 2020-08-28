
function deg_to_rad(deg: number): number {
	return deg * Math.PI/180.0;
}

function id_rw(i: number, j: number, n: number): number {
	return (i*n + j);
};

function id_cw(i: number, j: number, n: number): number {
	return (j*n + i);
};
