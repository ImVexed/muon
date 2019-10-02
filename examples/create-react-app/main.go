//go:generate fileb0x b0x.yml
package main

import (
	"cra/webfiles"
	"http"
)

func main() {
	fileHandler := http.FileServer(webfiles.HTTP)

	cfg := &muon.Config{
		Title:  "Hello, World!",
		Height: 500,
		Width:  500,
		Hint:   2 | 4,
	}

	m := muon.New(cfg, fileHandler)

	m.Bind("add", add)

	if err := m.Start(); err != nil {
		panic(err)
	}
}

func add(a float64, b float64) float64 {
	return a + b
}
