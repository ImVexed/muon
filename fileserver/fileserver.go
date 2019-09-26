package fileserver

import (
	"log"
	"net"
	"net/http"
	"test/webfiles"
)

func Serve() (string, error) {
	ln, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		return "", err
	}

	go func() {
		log.Fatal(http.Serve(ln, http.FileServer(webfiles.HTTP)))
	}()

	return "http://" + ln.Addr().String(), nil
}
