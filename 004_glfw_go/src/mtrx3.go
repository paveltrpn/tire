package main

import (
	"fmt"
	"math"
)

type mtrx3 [9]float32

func mtrx3Idtt() (rt mtrx3) {
	var (
		i, j   int32
		mrange int32 = 3
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

func mtrx3Set(m [9]float32) (rt mtrx3) {
	var (
		i, j   int32
		mrange int32 = 3
	)

	for i = 0; i < mrange; i++ {
		for j = 0; j < mrange; j++ {
			rt[idRw(i, j, mrange)] = m[idRw(i, j, mrange)]
		}
	}

	return rt
}

func mtrx3SetFloat(a00, a01, a02,
	a10, a11, a12,
	a20, a21, a22 float32) (rt mtrx3) {

	rt[0] = a00
	rt[1] = a01
	rt[2] = a02

	rt[3] = a10
	rt[4] = a11
	rt[5] = a12

	rt[6] = a20
	rt[7] = a21
	rt[8] = a22

	return rt
}

func mtrx3SetEuler(yaw, pitch, roll float32) (rt mtrx3) {
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

	rt[3] = siny*cosr + cosy*cosp*sinr
	rt[4] = -siny*sinr + cosy*cosp*cosr
	rt[5] = -cosy * sinp

	rt[6] = sinp * sinr
	rt[7] = sinp * cosr
	rt[8] = cosp

	return rt
}

func mtrx3SetAxisAngl(ax vec3, phi float32) (rt mtrx3) {
	var (
		cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz float32
		axis                                         vec3
	)

	axis = vec3Normalize(ax)

	cosphi = cosf(phi)
	sinphi = sinf(phi)
	vxvy = axis[_XC] * axis[_YC]
	vxvz = axis[_XC] * axis[_ZC]
	vyvz = axis[_YC] * axis[_ZC]
	vx = ax[_XC]
	vy = ax[_YC]
	vz = ax[_ZC]

	rt[0] = cosphi + (1.0-cosphi)*vx*vx
	rt[1] = (1.0-cosphi)*vxvy - sinphi*vz
	rt[2] = (1.0-cosphi)*vxvz + sinphi*vy

	rt[3] = (1.0-cosphi)*vxvy + sinphi*vz
	rt[4] = cosphi + (1.0-cosphi)*vy*vy
	rt[5] = (1.0-cosphi)*vyvz - sinphi*vx

	rt[6] = (1.0-cosphi)*vxvz - sinphi*vy
	rt[7] = (1.0-cosphi)*vyvz + sinphi*vx
	rt[8] = cosphi + (1.0-cosphi)*vz*vz

	return rt
}

func mtrx3SetYaw(angl float32) (rt mtrx3) {
	var (
		sa, ca float32
	)

	sa = sinf(degToRad(angl))
	ca = cosf(degToRad(angl))

	rt[0] = ca
	rt[1] = -sa
	rt[2] = 0.0

	rt[3] = sa
	rt[4] = ca
	rt[5] = 0.0

	rt[6] = 0.0
	rt[7] = 0.0
	rt[8] = 1.0

	return rt
}

func mtrx3SetPitch(angl float32) (rt mtrx3) {
	var (
		sa, ca float32
	)

	sa = sinf(degToRad(angl))
	ca = cosf(degToRad(angl))

	rt[0] = 1.0
	rt[1] = 0.0
	rt[2] = 0.0

	rt[3] = 0.0
	rt[4] = ca
	rt[5] = -sa

	rt[6] = 0.0
	rt[7] = sa
	rt[8] = ca

	return rt
}

func mtrx3SetRoll(angl float32) (rt mtrx3) {
	var (
		sa, ca float32
	)

	sa = sinf(degToRad(angl))
	ca = cosf(degToRad(angl))

	rt[0] = ca
	rt[1] = 0.0
	rt[2] = sa
	rt[3] = 0.0
	rt[4] = 1.0
	rt[5] = 0.0
	rt[6] = -sa
	rt[7] = 0.0
	rt[8] = ca

	return rt
}

func mtrx3Show(m mtrx3) {
	fmt.Printf("%5.2f %5.2f %5.2f\n", m[0], m[1], m[2])
	fmt.Printf("%5.2f %5.2f %5.2f\n", m[3], m[4], m[5])
	fmt.Printf("%5.2f %5.2f %5.2f\n", m[6], m[7], m[8])
}

func mtrx3Det(m mtrx3) float32 {
	return m[0]*m[4]*m[8] +
		m[6]*m[1]*m[5] +
		m[2]*m[3]*m[7] -
		m[0]*m[7]*m[5] -
		m[8]*m[3]*m[1]
}

func mtrx3Detlu(m mtrx3) (rt float32) {
	const (
		mrange int32 = 3
	)

	var (
		i          int32
		l, u       mtrx3
		lDet, uDet float32
	)

	l, u = mtrx3lu(m)

	lDet = l[0]
	uDet = u[0]

	for i = 1; i < mrange; i++ {
		lDet *= l[idRw(i, i, mrange)]
		uDet *= u[idRw(i, i, mrange)]
	}

	return lDet * uDet
}

func mtrx3Mult(a, b mtrx3) (rt mtrx3) {
	const (
		mrange int32 = 3
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

func mtrx3MultVec(m mtrx3, v vec3) (rt vec3) {
	const (
		mrange int32 = 3
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
	Где-то здесь ошибка, долго искал
	ничего не вышло и взял код из сети
*/
/*
func mtrx3LU(m mtrx3) (l, u mtrx3) {
	var (
		i, j, k int32
		lm, um  mtrx3
		sum     float32
	)

	for j = 0; j < 3; j++ {
		um[idRw(0, j, 3)] = m[idRw(0, j, 3)]
	}

	for j = 0; j < 3; j++ {
		lm[idRw(j, 0, 3)] = m[idRw(j, 0, 3)] / um[idRw(0, 0, 3)]
	}

	for i = 1; i < 3; i++ {
		for j = i; j < 3; j++ {
			sum = 0.0
			for k = 0; k < i; k++ {
				sum = sum + lm[idRw(i, k, 3)]*um[idRw(k, j, 3)]
			}
			um[idRw(i, j, 3)] = m[idRw(i, j, 3)] - sum
		}
	}

	for i = 1; i < 3; i++ {
		for j = i; j < 3; j++ {
			if i > j {
				lm[idRw(j, i, 3)] = 0.0
			} else {
				sum = 0.0
				for k = 0; k < i; k++ {
					sum = sum + lm[idRw(j, k, 3)]*um[idRw(k, i, 3)]
				}
				lm[idRw(j, i, 3)] = (1.0 / um[idRw(i, i, 3)]) * (m[idRw(j, i, 3)] - sum)
			}
		}
	}

	return lm, um
}
*/

/*
	Нижнетреугольная (L, lm) матрица имеет единицы по диагонали
*/
func mtrx3lu(m mtrx3) (lm, um mtrx3) {
	const (
		mrange int32 = 3
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

func mtrx3ldlt(m mtrx3) (lm mtrx3, dv vec3) {
	const (
		mrange int32 = 3
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
						fmt.Println("mtrx3ldlt(): mtrx is not positive deﬁnite")
						return mtrx3Idtt(), vec3Set(0.0, 0.0, 0.0)
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

func mtrx3Transpose(m mtrx3) (rt mtrx3) {
	const (
		mrange int32 = 3
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

func mtrx3Invert(m mtrx3) (rt mtrx3) {
	var (
		inverse     mtrx3
		det, invDet float32
	)

	inverse[0] = m[4]*m[8] - m[5]*m[7]
	inverse[3] = m[5]*m[6] - m[3]*m[8]
	inverse[6] = m[3]*m[7] - m[4]*m[6]

	det = m[0]*inverse[0] + m[1]*inverse[3] +
		m[2]*inverse[6]

	if fabs(det) < f_eps {
		fmt.Println("mtrxInvert(): determinant is a zero!")
		return mtrx3Idtt()
	}

	invDet = 1.0 / det

	inverse[1] = m[2]*m[7] - m[1]*m[8]
	inverse[2] = m[1]*m[5] - m[2]*m[4]
	inverse[4] = m[0]*m[8] - m[2]*m[6]
	inverse[5] = m[2]*m[3] - m[0]*m[5]
	inverse[7] = m[1]*m[6] - m[0]*m[7]
	inverse[8] = m[0]*m[4] - m[1]*m[3]

	rt[0] = inverse[0] * invDet
	rt[1] = inverse[1] * invDet
	rt[2] = inverse[2] * invDet

	rt[3] = inverse[3] * invDet
	rt[4] = inverse[4] * invDet
	rt[5] = inverse[5] * invDet

	rt[6] = inverse[6] * invDet
	rt[7] = inverse[7] * invDet
	rt[8] = inverse[8] * invDet

	return rt
}

func mtrx3SolveGauss(m mtrx3, v vec3) (rt vec3) {
	const (
		mrange int32 = 3
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

func mtrx3InsertCmn(m mtrx3, v vec3, cmn int32) (rt mtrx3) {
	const (
		mrange int32 = 3
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

func mtrx3SolveKramer(m mtrx3, v vec3) (rt vec3) {
	const (
		mrange int32 = 3
	)

	var (
		i      int32
		det    float32
		mtrxKr mtrx3
	)

	det = mtrx3Det(m)

	if fabs(det) < f_eps {
		fmt.Println("mtrx3SolveKramer(): system has no solve")
		return vec3Set(0.0, 0.0, 0.0)
	}

	for i = 0; i < mrange; i++ {
		mtrxKr = mtrx3InsertCmn(m, v, i)
		rt[i] = mtrx3Det(mtrxKr) / det
	}

	return rt
}
