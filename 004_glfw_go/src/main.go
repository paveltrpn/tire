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
	texture uint32

	base = [8]vec3{
		{1.0, 1.0, 1.0},
		{-1.0, 1.0, 1.0},
		{-1.0, -1.0, 1.0},
		{1.0, -1.0, 1.0},
		{1.0, 1.0, -1.0},
		{-1.0, 1.0, -1.0},
		{-1.0, -1.0, -1.0},
		{1.0, -1.0, -1.0}}

	baseNormal = [6]vec3{
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0},
		{0.0, 1.0, 0.0},
		{0.0, -1.0, 0.0},
		{1.0, 0.0, 0.0},
		{-1.0, 0.0, 0.0}}

	mRtn mtrx3 = mtrx3SetAxisAngl(vec3Set(1, 0.5, 0.1), 0.04)
)

func showBox() {
	gl.Begin(gl.QUADS) // Рисуем куб

	gl.Color3f(0.1, 0.6, 0.1)
	gl.Normal3fv(&baseNormal[0][0])
	gl.TexCoord2f(0.0, 0.0)
	gl.Vertex3fv(&base[0][0])
	gl.TexCoord2f(1.0, 0.0)
	gl.Vertex3fv(&base[1][0])
	gl.TexCoord2f(1.0, 1.0)
	gl.Vertex3fv(&base[2][0])
	gl.TexCoord2f(0.0, 1.0)
	gl.Vertex3fv(&base[3][0])

	gl.Color3f(0.8, 0.9, 0.1)
	gl.Normal3fv(&baseNormal[1][0])
	gl.TexCoord2f(0.0, 0.0)
	gl.Vertex3fv(&base[4][0])
	gl.TexCoord2f(1.0, 0.0)
	gl.Vertex3fv(&base[5][0])
	gl.TexCoord2f(1.0, 1.0)
	gl.Vertex3fv(&base[6][0])
	gl.TexCoord2f(0.0, 1.0)
	gl.Vertex3fv(&base[7][0])

	gl.Color3f(0.0, 1.0, 1.0)
	gl.Normal3fv(&baseNormal[2][0])
	gl.TexCoord2f(0.0, 0.0)
	gl.Vertex3fv(&base[0][0])
	gl.TexCoord2f(1.0, 0.0)
	gl.Vertex3fv(&base[1][0])
	gl.TexCoord2f(1.0, 1.0)
	gl.Vertex3fv(&base[5][0])
	gl.TexCoord2f(0.0, 1.0)
	gl.Vertex3fv(&base[4][0])

	gl.Color3f(0.93, 0.11, 0.23)
	gl.Normal3fv(&baseNormal[3][0])
	gl.TexCoord2f(0.0, 0.0)
	gl.Vertex3fv(&base[2][0])
	gl.TexCoord2f(1.0, 0.0)
	gl.Vertex3fv(&base[3][0])
	gl.TexCoord2f(1.0, 1.0)
	gl.Vertex3fv(&base[7][0])
	gl.TexCoord2f(0.0, 1.0)
	gl.Vertex3fv(&base[6][0])

	gl.Color3f(0.1, 0.15, 0.75)
	gl.Normal3fv(&baseNormal[4][0])
	gl.TexCoord2f(0.0, 0.0)
	gl.Vertex3fv(&base[0][0])
	gl.TexCoord2f(1.0, 0.0)
	gl.Vertex3fv(&base[4][0])
	gl.TexCoord2f(1.0, 1.0)
	gl.Vertex3fv(&base[7][0])
	gl.TexCoord2f(0.0, 1.0)
	gl.Vertex3fv(&base[3][0])

	gl.Color3f(0.8, 0.2, 0.98)
	gl.Normal3fv(&baseNormal[5][0])
	gl.TexCoord2f(0.0, 0.0)
	gl.Vertex3fv(&base[1][0])
	gl.TexCoord2f(1.0, 0.0)
	gl.Vertex3fv(&base[5][0])
	gl.TexCoord2f(1.0, 1.0)
	gl.Vertex3fv(&base[6][0])
	gl.TexCoord2f(0.0, 1.0)
	gl.Vertex3fv(&base[2][0])

	gl.End()
}

func updBox() {
	var (
		i int32
	)

	for i = 0; i < 8; i++ {
		base[i] = mtrx3MultVec(mRtn, base[i])
	}

	for i = 0; i < 6; i++ {
		baseNormal[i] = mtrx3MultVec(mRtn, baseNormal[i])
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

	// Включили VSync
	glfw.SwapInterval(1)

	if err := gl.Init(); err != nil {
		panic(err)
	}

	println(gl.GoStr(gl.GetString(gl.RENDERER)))
	println(gl.GoStr(gl.GetString(gl.VERSION)))
	println(gl.GoStr(gl.GetString(gl.SHADING_LANGUAGE_VERSION)))

	setupScene()

	for !window.ShouldClose() {
		frame()
		window.SwapBuffers()
		glfw.PollEvents()
	}

}

func setupScene() {
	var (
		bmp bitmapType
	)

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

	bmp.loadFromJpeg("assets/texture.jpg")
	gl.Enable(gl.TEXTURE_2D)
	gl.GenTextures(1, &texture)
	gl.ActiveTexture(gl.TEXTURE0)
	gl.BindTexture(gl.TEXTURE_2D, texture)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE)
	gl.TexImage2D(
		gl.TEXTURE_2D,
		0,
		gl.RGBA,
		bmp.width,
		bmp.height,
		0,
		gl.RGBA,
		gl.UNSIGNED_BYTE,
		gl.Ptr(bmp.data))
}

func destroyScene() {

}

func frame() {
	gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

	gl.MatrixMode(gl.MODELVIEW)
	gl.LoadIdentity()
	gl.Translatef(0, 0, -5.0)

	gl.Enable(gl.TEXTURE_2D)
	gl.BindTexture(gl.TEXTURE_2D, texture)
	showBox()

	updBox()
}
