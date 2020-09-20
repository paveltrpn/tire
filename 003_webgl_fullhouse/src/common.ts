
const fEPS: number = 0.000001;

function degToRad(deg: number): number {
	return deg * Math.PI/180.0;
}

function idRw(i: number, j: number, n: number): number {
	return (i*n + j);
};

function idCw(i: number, j: number, n: number): number {
	return (j*n + i);
};
