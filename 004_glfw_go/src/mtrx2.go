package main

import (
	"fmt"
	"math"
)

type mtrx2 [4]float32

func mtrx2Idtt() (rt mtrx2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j int32
	)

	for i = 0; i < mrange; i++ {
		for j = 0; j < mrange; j++ {
			if i == j {
				rt[idRw(i, j, mrange)] = 1.0
			} else {
				rt[idRw(i, j, mrange)] = 0.0
			}
		}
	}

	return rt
}

func mtrx2Set(m [4]float32) (rt mtrx2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j int32
	)

	for i = 0; i < mrange; i++ {
		for j = 0; j < mrange; j++ {
			rt[idRw(i, j, mrange)] = m[idRw(i, j, mrange)]
		}
	}

	return rt
}

func mtrx2SetFloat(a00, a01, a10, a11 float32) (rt mtrx2) {
	rt[0] = a00
	rt[1] = a01
	rt[2] = a10
	rt[3] = a11

	return rt
}

func mtrx2Rtn(phi float32) (rt mtrx2) {
	var (
		cosphi, sinphi float32
	)

	sinphi = sinf(phi)
	cosphi = cosf(phi)

	rt[0] = cosphi
	rt[1] = -sinphi
	rt[2] = -sinphi
	rt[3] = cosphi

	return rt
}

func mtrx2Show(m mtrx2) {
	fmt.Printf("%5.2f %5.2f\n", m[0], m[1])
	fmt.Printf("%5.2f %5.2f\n", m[2], m[3])
}

func mtrx2Det(m mtrx2) float32 {
	return m[0]*m[3] - m[1]*m[2]
}

func mtrx2Detlu(m mtrx2) (rt float32) {
	const (
		mrange int32 = 2
	)

	var (
		i          int32
		l, u       mtrx2
		lDet, uDet float32
	)

	l, u = mtrx2lu(m)

	lDet = l[0]
	uDet = u[0]

	for i = 1; i < mrange; i++ {
		lDet *= l[idRw(i, i, mrange)]
		uDet *= u[idRw(i, i, mrange)]
	}

	return lDet * uDet
}

func mtrx2Mult(a, b mtrx2) (rt mtrx2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j, k int32
		tmp     float32
	)

	for i = 0; i < mrange; i++ {
		for j = 0; j < mrange; j++ {
			tmp = 0.0
			for k = 0; k < mrange; k++ {
				tmp = tmp + a[idRw(k, j, mrange)]*b[idRw(i, k, mrange)]
			}
			rt[idRw(i, j, mrange)] = tmp
		}
	}

	return rt
}

func mtrx2MultVec(m mtrx2, v vec2) (rt vec2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j int32
		tmp  float32
	)

	for i = 0; i < mrange; i++ {
		tmp = 0
		for j = 0; j < mrange; j++ {
			tmp = tmp + m[idRw(i, j, mrange)]*v[j]
		}
		rt[i] = tmp
	}

	return rt
}

/*
	Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/

func mtrx2lu(m mtrx2) (lm, um mtrx2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j, k int32
		sum     float32
	)

	for i = 0; i < mrange; i++ {
		for k = i; k < mrange; k++ {
			sum = 0
			for j = 0; j < i; j++ {
				sum += (lm[idRw(i, j, mrange)] * um[idRw(j, k, mrange)])
			}
			um[idRw(i, k, mrange)] = m[idRw(i, k, mrange)] - sum
		}

		for k = i; k < mrange; k++ {
			if i == k {
				lm[idRw(i, i, mrange)] = 1.0
			} else {
				sum = 0
				for j = 0; j < i; j++ {
					sum += lm[idRw(k, j, mrange)] * um[idRw(j, i, mrange)]
				}
				lm[idRw(k, i, mrange)] = (m[idRw(k, i, mrange)] - sum) / um[idRw(i, i, mrange)]
			}
		}
	}

	return lm, um
}

func mtrx2ldlt(m mtrx2) (lm mtrx2, dv vec2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j, k int32
		sum     float32
	)

	for i = 0; i < mrange; i++ {
		for j = i; j < mrange; j++ {
			sum = m[idRw(j, i, mrange)]
			for k = 0; k < i; k++ {
				sum = sum - lm[idRw(i, k, mrange)]*dv[k]*lm[idRw(j, k, mrange)]
				if i == j {
					if sum <= 0 {
						fmt.Println("mtrx2ldlt(): mtrx is not positive deﬁnite")
						return mtrx2Idtt(), vec2Set(0.0, 0.0)
					}
					dv[i] = sum
					lm[idRw(i, i, mrange)] = 1.0
				} else {
					lm[idRw(j, i, mrange)] = sum / dv[i]
				}
			}
		}
	}

	return lm, dv
}

func mtrx2Transpose(m mtrx2) (rt mtrx2) {
	const (
		mrange int32 = 2
	)

	var (
		i, j int32
		tmp  float32
	)

	rt = m

	for i = 0; i < mrange; i++ {
		for j = 0; j < i; j++ {
			tmp = rt[idRw(i, i, mrange)]
			rt[idRw(i, j, mrange)] = rt[idRw(j, i, mrange)]
			rt[idRw(j, i, mrange)] = tmp
		}
	}

	return rt
}

func mtrx2Invert(m mtrx2) mtrx2 {
	var (
		det float32
	)

	det = mtrx2Det(m)

	if fabs(det) < f_eps {
		fmt.Println("mtrx2invert(): determinant is a zero!")
		return mtrx2Idtt()
	}

	return mtrx2SetFloat(m[3], -m[1]/det, -m[2]/det, m[0]/det)
}

func mtrx2SolveGauss(m mtrx2, v vec3) (rt vec3) {
	const (
		mrange int32 = 2
	)

	var (
		i, j, k int32
		t       float32
		a       [mrange][mrange + 1]float32
	)

	for i = 0; i < mrange; i++ { //было ++i
		for j = 0; j < mrange; j++ { //было ++j
			a[i][j] = m[idRw(i, j, mrange)]
			a[i][mrange] = v[i]
		}
	}

	/* Pivotisation */
	for i = 0; i < mrange; i++ {
		for k = i + 1; k < mrange; k++ {
			if math.Abs(float64(a[i][i])) < math.Abs(float64(a[k][i])) {
				for j = 0; j <= mrange; j++ {
					t = a[i][j]
					a[i][j] = a[k][j]
					a[k][j] = t
				}
			}
		}
	}

	/* прямой ход */
	for k = 1; k < mrange; k++ {
		for j = k; j < mrange; j++ {
			t = a[j][k-1] / a[k-1][k-1]
			for i = 0; i < mrange+1; i++ {
				a[j][i] = a[j][i] - t*a[k-1][i]
			}
		}
	}

	/* обратный ход */
	for i = mrange - 1; i >= 0; i-- {
		rt[i] = a[i][mrange] / a[i][i]
		for j = mrange - 1; j > i; j-- {
			rt[i] = rt[i] - a[i][j]*rt[j]/a[i][i]
		}
	}

	return rt
}

func mtrx2InsertCmn(m mtrx2, v vec3, cmn int32) (rt mtrx2) {
	const (
		mrange int32 = 2
	)

	var (
		i int32
		j int32 = 0
	)

	rt = m

	for i = cmn; i < mrange*mrange; i += mrange {
		rt[i] = v[j]
		j++
	}

	return rt
}

func mtrx2SolveKramer(m mtrx2, v vec3) (rt vec2) {
	const (
		mrange int32 = 2
	)

	var (
		i      int32
		det    float32
		mtrxKr mtrx2
	)

	det = mtrx2Det(m)

	if fabs(det) < f_eps {
		fmt.Println("mtrx2SolveKramer(): system has no solve")
		return vec2Set(0.0, 0.0)
	}

	for i = 0; i < mrange; i++ {
		mtrxKr = mtrx2InsertCmn(m, v, i)
		rt[i] = mtrx2Det(mtrxKr) / det
	}

	return rt
}
