package muon

import (
	"net/http"
	"reflect"
	"testing"
)

type testObject struct {
	S1 string
	F1 float64
	B1 bool
}

func TestComplexType(t *testing.T) {
	cfg := &Config{
		Height: 1,
		Width:  1,
	}

	m := New(cfg, http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {}))

	m.Bind("test", func(to *testObject) *testObject {
		return &testObject{
			S1: to.S1 + " World!",
			F1: to.F1 + 1,
			B1: !to.B1,
		}
	})

	go func() {
		m.Start()
	}()

	res, err := m.Eval(`test({S1: "Hello,", F1: 9000, B1: false})`, reflect.TypeOf(&testObject{}))

	if err != nil {
		t.Error(err)
	}

	to := res.(*testObject)

	if to.S1 != "Hello, World!" {
		t.Errorf("to.S1 was not correct, got %s", to.S1)
	}

	if to.F1 != 9001 {
		t.Errorf("to.F1 was under 9000, got %f", to.F1)
	}

	if !to.B1 {
		t.Errorf("to.B1 was not True, got false")
	}
}

func TestArrayType(t *testing.T) {
	cfg := &Config{
		Height: 1,
		Width:  1,
	}

	m := New(cfg, http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {}))

	m.Bind("test", func(strings []string) []float64 {
		if strings[0] != "Hello" {
			t.Errorf("strings[0] was not Hello, got %s", strings[0])
		}
		if strings[1] != "World!" {
			t.Errorf("strings[1] was not World!, got %s", strings[1])
		}
		return []float64{1, 2, 3}
	})

	go func() {
		m.Start()
	}()

	res, err := m.Eval(`test(["Hello","World!"])`, reflect.TypeOf([]float64{}))

	if err != nil {
		t.Error(err)
	}

	nums := res.([]float64)

	if nums[0] != 1 {
		t.Errorf("nums[0] was not 1, got %f", nums[0])
	}

	if nums[1] != 2 {
		t.Errorf("nums[1] was not 2, got %f", nums[1])
	}

	if nums[2] != 3 {
		t.Errorf("nums[2] was not 3, got %f", nums[2])
	}
}
