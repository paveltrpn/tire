package main

import (
	"fmt"
	"math"
)

type mtrx4 [16]float32

func mtrx4Idtt() (rt mtrx4) {
	var (
		i, j int32
		n    int32 = 4
	)

	for i = 0; i < n; i++ {
		for j = 0; j < n; j++ {
			if i == j {
				rt[idRw(i, j, n)] = 1.0
			} else {
				rt[idRw(i, j, n)] = 0.0
			}
		}
	}

	return rt
}

func mtrx4Set(m [16]float32) (rt mtrx4) {
	var (
		i, j int32
		n    int32 = 4
	)

	for i = 0; i < n; i++ {
		for j = 0; j < n; j++ {
			rt[idRw(i, j, n)] = m[idRw(i, j, n)]
		}
	}
	return rt
}

func mtrx4SetFloat(a00, a01, a02, a03,
	a10, a11, a12, a13,
	a20, a21, a22, a23,
	a30, a31, a32, a33 float32) (rt mtrx4) {

	rt[0] = a00
	rt[1] = a01
	rt[2] = a02
	rt[3] = a03

	rt[4] = a10
	rt[5] = a11
	rt[6] = a12
	rt[7] = a13

	rt[8] = a20
	rt[9] = a21
	rt[10] = a22
	rt[11] = a23

	rt[12] = a30
	rt[13] = a31
	rt[14] = a32
	rt[15] = a33

	return rt
}

func mtrx4SetEuler(yaw, pitch, roll float32) (rt mtrx4) {
	var (
		cosy, siny, cosp, sinp, cosr, sinr float32
	)

	cosy = cosf(yaw)
	siny = sinf(yaw)
	cosp = cosf(pitch)
	sinp = sinf(pitch)
	cosr = cosf(roll)
	sinr = sinf(roll)

	rt[0] = cosy*cosr - siny*cosp*sinr
	rt[1] = -cosy*sinr - siny*cosp*cosr
	rt[2] = siny * sinp
	rt[3] = 0.0

	rt[4] = siny*cosr + cosy*cosp*sinr
	rt[5] = -siny*sinr + cosy*cosp*cosr
	rt[6] = -cosy * sinp
	rt[7] = 0.0

	rt[8] = sinp * sinr
	rt[9] = sinp * cosr
	rt[10] = cosp
	rt[11] = 0.0

	rt[12] = 0.0
	rt[13] = 0.0
	rt[14] = 0.0
	rt[15] = 1.0

	return rt
}

func mtrx4SetAxisangl(ax vec3, phi float32) (rt mtrx4) {
	var (
		cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz float32
	)

	cosphi = cosf(phi)
	sinphi = sinf(phi)
	vxvy = ax[_XC] * ax[_YC]
	vxvz = ax[_XC] * ax[_ZC]
	vyvz = ax[_YC] * ax[_ZC]
	vx = ax[_XC]
	vy = ax[_YC]
	vz = ax[_ZC]

	rt[0] = cosphi + (1.0-cosphi)*vx*vx
	rt[1] = (1.0-cosphi)*vxvy - sinphi*vz
	rt[2] = (1.0-cosphi)*vxvz + sinphi*vy
	rt[3] = 0.0

	rt[4] = (1.0-cosphi)*vxvy + sinphi*vz
	rt[5] = cosphi + (1.0-cosphi)*vy*vy
	rt[6] = (1.0-cosphi)*vyvz - sinphi*vz
	rt[7] = 0.0

	rt[8] = (1.0-cosphi)*vxvz - sinphi*vy
	rt[9] = (1.0-cosphi)*vyvz + sinphi*vx
	rt[10] = cosphi + (1.0-cosphi)*vz*vz
	rt[11] = 0.0

	rt[12] = 0.0
	rt[13] = 0.0
	rt[14] = 0.0
	rt[15] = 1.0

	return rt
}

func mtrx4Show(m mtrx4) {
	fmt.Printf("%5.2f %5.2f %5.2f %5.2f\n", m[0], m[1], m[2], m[3])
	fmt.Printf("%5.2f %5.2f %5.2f %5.2f\n", m[4], m[5], m[6], m[7])
	fmt.Printf("%5.2f %5.2f %5.2f %5.2f\n", m[8], m[9], m[10], m[11])
	fmt.Printf("%5.2f %5.2f %5.2f %5.2f\n", m[12], m[13], m[14], m[15])
}

func mtrx4Det(m mtrx4) float32 {
	return 0.0
}

func mtrx4Detlu(m mtrx4) (rt float32) {
	const (
		mrange int32 = 4
	)

	var (
		i          int32
		l, u       mtrx4
		lDet, uDet float32
	)

	l, u = mtrx4LU(m)

	lDet = l[0]
	uDet = u[0]

	for i = 1; i < mrange; i++ {
		lDet *= l[idRw(i, i, mrange)]
		uDet *= u[idRw(i, i, mrange)]
	}

	return lDet * uDet
}

func mtrx4Mult(a, b mtrx4) (rt mtrx4) {
	const (
		mrange int32 = 4
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

func mtrx4MultVec(m mtrx4, v vec4) (rt vec4) {
	const (
		mrange int32 = 4
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

func mtrx4LU(m mtrx4) (lm, um mtrx4) {
	const (
		mrange int32 = 4
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

func mtrx4ldlt(m mtrx4) (lm mtrx4, dv vec4) {
	const (
		mrange int32 = 4
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
						fmt.Println("mtrx4ldlt(): mtrx is not positive deﬁnite")
						return mtrx4Idtt(), vec4Set(0.0, 0.0, 0.0, 0.0)
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

func mtrx4Invert(m mtrx4) (rt mtrx4) {
	return mtrx4Idtt()
}

func mtrx4SolveGauss(m mtrx4, v vec4) (rt vec4) {
	const (
		mrange int32 = 4
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

func mtrx4InsertCmn(m mtrx4, v vec4, cmn int32) (rt mtrx4) {
	const (
		mrange int32 = 4
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

func mtrx4SolveKramer(m mtrx4, v vec4) (rt vec4) {
	const (
		mrange int32 = 4
	)

	var (
		i      int32
		det    float32
		mtrxKr mtrx4
	)

	det = mtrx4Detlu(m)

	if fabs(det) < f_eps {
		fmt.Println("mtrx4SolveKramer(): system has no solve")
		return vec4Set(0.0, 0.0, 0.0, 0.0)
	}

	for i = 0; i < mrange; i++ {
		mtrxKr = mtrx4InsertCmn(m, v, i)
		rt[i] = mtrx4Detlu(mtrxKr) / det
	}

	return rt
}
