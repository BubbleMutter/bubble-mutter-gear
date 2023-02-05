package main

import "fmt"

func maps() {
	type Vertex struct {
		Lat  float64
		Long float64
	}
	{
		// allocate nil map
		m := make(map[string]Vertex)
		fmt.Println(m)
	}
	{
		// allocate nil map
		m := map[string]Vertex{}
		m["Bell Labs"] = Vertex{
			Lat: 40.68433, Long: -74.39967,
		}
		fmt.Println(m["Bell Labs"])
	}
	{
		// initialization members
		m := map[string]Vertex{
			"Bell Labs": {
				Lat: 40.68433, Long: -74.39967,
			},
			"Google": {
				Lat: 37.42202, Long: -122.08408,
			},
		}
		fmt.Println(m["Bell Labs"])
	}
	{
		// delete member
		m := map[string]int{"answer": 42}
		{
			v, ok := m["Answer"]
			fmt.Println("The value:", v, "Present?", ok)
		}
		delete(m, "Answer")
		{
			v, ok := m["Answer"]
			fmt.Println("The value:", v, "Present?", ok)
		}
	}
}

func main() {
	maps()
}
