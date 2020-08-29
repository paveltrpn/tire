// Copyright 2014 The go-gl Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Renders a textured spinning cube using GLFW 3 and OpenGL 2.1.
package main

import (
	_ "image/png"
	"log"
	"runtime"

	"github.com/go-gl/gl/v2.1/gl"
	"github.com/go-gl/glfw/v3.2/glfw"
)

var (
	texture   uint32
	rotationX float32
	rotationY float32

	geometry = [8]vec3_t{
		{1.0, 1.0, 1.0},
		{-1.0, 1.0, 1.0},
		{-1.0, -1.0, 1.0},
		{1.0, -1.0, 1.0},
		{1.0, 1.0, -1.0},
		{-1.0, 1.0, -1.0},
		{-1.0, -1.0, -1.0},
		{1.0, -1.0, -1.0}}

	x_ax          = vec3_t{1.0, 0.0, 0.0}
	y_ax          = vec3_t{0.0, 1.0, 0.0}
	z_ax          = vec3_t{0.0, 0.0, 1.0}
	m_rtn mtrx3_t = mtrx3SetAxisAngl(x_ax, 0.01)
)

func showBox() {
	gl.Begin(gl.QUADS) // Рисуем куб

	gl.Color3f(0.1, 0.6, 0.1)
	gl.Vertex3fv(&geometry[0][0])
	gl.Vertex3fv(&geometry[1][0])
	gl.Vertex3fv(&geometry[2][0])
	gl.Vertex3fv(&geometry[3][0])

	gl.Color3f(0.8, 0.9, 0.1)
	gl.Vertex3fv(&geometry[4][0])
	gl.Vertex3fv(&geometry[5][0])
	gl.Vertex3fv(&geometry[6][0])
	gl.Vertex3fv(&geometry[7][0])

	gl.Color3f(0.0, 1.0, 1.0)
	gl.Vertex3fv(&geometry[0][0])
	gl.Vertex3fv(&geometry[1][0])
	gl.Vertex3fv(&geometry[5][0])
	gl.Vertex3fv(&geometry[4][0])

	gl.Color3f(0.93, 0.11, 0.23)
	gl.Vertex3fv(&geometry[2][0])
	gl.Vertex3fv(&geometry[3][0])
	gl.Vertex3fv(&geometry[7][0])
	gl.Vertex3fv(&geometry[6][0])

	gl.Color3f(0.1, 0.15, 0.75)
	gl.Vertex3fv(&geometry[0][0])
	gl.Vertex3fv(&geometry[4][0])
	gl.Vertex3fv(&geometry[7][0])
	gl.Vertex3fv(&geometry[3][0])

	gl.Color3f(0.8, 0.2, 0.98)
	gl.Vertex3fv(&geometry[1][0])
	gl.Vertex3fv(&geometry[5][0])
	gl.Vertex3fv(&geometry[6][0])
	gl.Vertex3fv(&geometry[2][0])

	gl.End()
}

func updBox() {
	var (
		i int32
	)

	for i = 0; i < 8; i++ {
		geometry[i] = mtrx3MultVec(m_rtn, geometry[i])
		//geometry[i] = qtnn_transform_vec3(q_rtn, geometry[i]);
	}
}

func init() {
	// GLFW event handling must run on the main OS thread
	runtime.LockOSThread()
}

func main() {
	if err := glfw.Init(); err != nil {
		log.Fatalln("failed to initialize glfw:", err)
	}
	defer glfw.Terminate()

	glfw.WindowHint(glfw.Resizable, glfw.False)
	glfw.WindowHint(glfw.ContextVersionMajor, 2)
	glfw.WindowHint(glfw.ContextVersionMinor, 1)
	window, err := glfw.CreateWindow(800, 600, "Cube", nil, nil)
	if err != nil {
		panic(err)
	}
	window.MakeContextCurrent()

	if err := gl.Init(); err != nil {
		panic(err)
	}

	setupScene()

	for !window.ShouldClose() {
		frame()
		window.SwapBuffers()
		glfw.PollEvents()
	}
}

func setupScene() {
	gl.Enable(gl.DEPTH_TEST)
	gl.Enable(gl.LIGHTING)

	gl.ClearColor(0.5, 0.5, 0.5, 0.0)
	gl.ClearDepth(1)
	gl.DepthFunc(gl.LEQUAL)

	ambient := []float32{0.5, 0.5, 0.5, 1}
	diffuse := []float32{1, 1, 1, 1}
	lightPosition := []float32{-5, 5, 10, 0}
	gl.Lightfv(gl.LIGHT0, gl.AMBIENT, &ambient[0])
	gl.Lightfv(gl.LIGHT0, gl.DIFFUSE, &diffuse[0])
	gl.Lightfv(gl.LIGHT0, gl.POSITION, &lightPosition[0])
	gl.Enable(gl.LIGHT0)

	gl.MatrixMode(gl.PROJECTION)
	gl.LoadIdentity()
	gl.Frustum(-1, 1, -1, 1, 1.0, 10.0)
	gl.MatrixMode(gl.MODELVIEW)
	gl.LoadIdentity()
}

func destroyScene() {

}

func frame() {
	gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

	gl.MatrixMode(gl.MODELVIEW)
	gl.LoadIdentity()
	gl.Translatef(0, 0, -3.0)
	gl.Rotatef(rotationX, 1, 0, 0)
	gl.Rotatef(rotationY, 0, 1, 0)

	rotationX += 0.5
	rotationY += 0.5

	showBox()
	updBox()
}
