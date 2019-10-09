package muon

import (
	"net/http"
	"os"
	"reflect"
	"testing"
)

var w *Window

func TestMain(m *testing.M) {
	cfg := &Config{
		Height: 1,
		Width:  1,
	}

	w = New(cfg, http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {}))

	go func() {
		w.Start()
	}()

	os.Exit(m.Run())
}

type testObject struct {
	S1 string
	F1 float64
	B1 bool
}

func TestComplexType(t *testing.T) {

	w.Bind("complexTest", func(to *testObject) *testObject {
		return &testObject{
			S1: to.S1 + " World!",
			F1: to.F1 + 1,
			B1: !to.B1,
		}
	})

	res, err := w.Eval(`complexTest({S1: "Hello,", F1: 9000, B1: false})`, reflect.TypeOf(&testObject{}))

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

func t2(to *testObject) *testObject {
	return &testObject{
		S1: to.S1 + " World!",
		F1: to.F1 + 1,
		B1: !to.B1,
	}
}

func TestArrayType(t *testing.T) {

	w.Bind("arrayTest", func(strings []string) []float64 {
		if strings[0] != "Hello" {
			t.Errorf("strings[0] was not Hello, got %s", strings[0])
		}
		if strings[1] != "World!" {
			t.Errorf("strings[1] was not World!, got %s", strings[1])
		}
		return []float64{1, 2, 3}
	})

	res, err := w.Eval(`arrayTest(["Hello","World!"])`, reflect.TypeOf([]float64{}))

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

func TestEmptyType(t *testing.T) {

	w.Bind("emptyTypeTest", func(nullValue string, undefinedValue string) {
		if nullValue != "" {
			t.Errorf("nullType was not empty!")
		}
		if undefinedValue != "" {
			t.Errorf("undefinedType was not empty!")
		}
	})

	_, err := w.Eval(`emptyTypeTest(null, undefined)`, nil)

	if err != nil {
		t.Error(err)
	}
}

func TestMultipleFuncs(t *testing.T) {
	w.Bind("multiple1Test", func(value1 string) {})
	w.Bind("multiple2Test", func(value2 bool) {})

	_, err := w.Eval(`multiple1Test("Hello, World1")`, nil)

	if err != nil {
		t.Error(err)
	}

	_, err = w.Eval(`multiple2Test(true)`, nil)

	if err != nil {
		t.Error(err)
	}
}
