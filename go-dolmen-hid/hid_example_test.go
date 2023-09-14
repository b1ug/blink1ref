package hid_test

import (
	"fmt"

	"github.com/dolmen-go/hid"
)

func ExampleSupported() {
	fmt.Println("Build configuration supported:", hid.Supported())
	// Output:
	// Build configuration supported: true
}
